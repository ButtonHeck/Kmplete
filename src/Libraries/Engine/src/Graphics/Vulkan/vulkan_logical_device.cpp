#include "Kmplete/Graphics/Vulkan/vulkan_logical_device.h"
#include "Kmplete/Graphics/Vulkan/vulkan_physical_device.h"
#include "Kmplete/Graphics/Vulkan/vulkan_command_pool.h"
#include "Kmplete/Graphics/Vulkan/vulkan_swapchain.h"
#include "Kmplete/Graphics/Vulkan/vulkan_result_description.h"
#include "Kmplete/Window/window.h"
#include "Kmplete/Math/math.h"
#include "Kmplete/Math/geometry.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Profile/profiler.h"
#include "Kmplete/Log/log.h"

#if defined (KMP_WINDOW_BACKEND_GLFW)
    #include <GLFW/glfw3.h>
#endif

#include <limits>
#include <stdexcept>


namespace Kmplete
{
    namespace Graphics
    {
        VulkanLogicalDevice::VulkanLogicalDevice(const VkPhysicalDevice& physicalDevice, const VkSurfaceKHR& surface, const PhysicalDeviceInfo& info, const Window& window)
            : LogicalDevice()
            , _physicalDevice(physicalDevice)
            , _surface(surface)
            , _physicalDeviceInfo(info)
            , _window(window)
            , _device(nullptr)
            , _graphicsQueue(nullptr)
            , _presentQueue(nullptr)
            , _presentCompleteSemaphore(VK_NULL_HANDLE)
            , _renderCompleteSemaphore(VK_NULL_HANDLE)
            , _submitPipelineStages(VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT)
            , _submitInfo()
            , _drawCommandBuffers()
            , _waitFences()
            , _currentExtent()
            , _imageCreatorDelegate(nullptr)
        {
            KMP_PROFILE_FUNCTION(ProfileLevelAlways);

            _CreateLogicalDeviceObject();
            _GetDeviceQueues();
            _CreateSemaphoreObjects();
            _InitializeSubmitInfo();

            _commandPool.reset(new VulkanCommandPool(_device, _physicalDeviceInfo.graphicsFamilyIndex));
            _imageCreatorDelegate.reset(new VulkanImageCreatorDelegate(_device, _physicalDeviceInfo));

            CreateSwapchain();

            _CreateCommandBuffers();
            _CreateFences();
        }
        //--------------------------------------------------------------------------

        VulkanLogicalDevice::~VulkanLogicalDevice()
        {
            _DeleteFences();
            _DeleteCommandBuffers();
            DeleteSwapchain();

            _imageCreatorDelegate.reset();
            _commandPool.reset();

            vkDestroySemaphore(_device, _presentCompleteSemaphore, nullptr);
            vkDestroySemaphore(_device, _renderCompleteSemaphore, nullptr);

            vkDestroyDevice(_device, nullptr);
        }
        //--------------------------------------------------------------------------

        void VulkanLogicalDevice::CreateSwapchain()
        {
            _currentExtent = _UpdateExtent();
            _swapchain.reset(new VulkanSwapchain(_device, _surface, _physicalDeviceInfo, _currentExtent, *_imageCreatorDelegate.get()));
        }
        //--------------------------------------------------------------------------

        void VulkanLogicalDevice::DeleteSwapchain()
        {
            _swapchain.reset();
        }
        //--------------------------------------------------------------------------

        void VulkanLogicalDevice::RecreateSwapchain()
        {
            while (_window.IsIconified())
            {
                glfwWaitEvents();
            }

            vkDeviceWaitIdle(_device);

            _DeleteFences();
            _DeleteCommandBuffers();
            DeleteSwapchain();

            CreateSwapchain();
            _CreateCommandBuffers();
            _CreateFences();
        }
        //--------------------------------------------------------------------------

        const VkDevice& VulkanLogicalDevice::GetImplDevice() const noexcept
        {
            return _device;
        }
        //--------------------------------------------------------------------------

        const VkQueue& VulkanLogicalDevice::GetGraphicsQueue() const noexcept
        {
            return _graphicsQueue;
        }
        //--------------------------------------------------------------------------

        const VkQueue& VulkanLogicalDevice::GetPresentQueue() const noexcept
        {
            return _presentQueue;
        }
        //--------------------------------------------------------------------------

        void VulkanLogicalDevice::_CreateLogicalDeviceObject()
        {
            const auto queueCreateInfos = _CreateQueueCreateInfos();

            VkPhysicalDeviceFeatures deviceFeatures{};
            deviceFeatures.samplerAnisotropy = VK_TRUE;

            VkPhysicalDeviceDynamicRenderingFeatures dynamicRenderingFeatures{};
            dynamicRenderingFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DYNAMIC_RENDERING_FEATURES_KHR;
            dynamicRenderingFeatures.dynamicRendering = VK_TRUE;

            const auto& enabledDeviceExtensions = VulkanPhysicalDevice::GetEnabledDeviceExtensions();

            VkDeviceCreateInfo createInfo{};
            createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
            createInfo.queueCreateInfoCount = UInt32(queueCreateInfos.size());
            createInfo.pQueueCreateInfos = queueCreateInfos.data();
            createInfo.pEnabledFeatures = &deviceFeatures;
            createInfo.enabledExtensionCount = UInt32(enabledDeviceExtensions.size());
            createInfo.ppEnabledExtensionNames = enabledDeviceExtensions.data();
            createInfo.pNext = &dynamicRenderingFeatures;

            const auto result = vkCreateDevice(_physicalDevice, &createInfo, nullptr, &_device);
            if (result != VK_SUCCESS)
            {
                const auto resultDescription = VkResultToString(result);
                KMP_LOG_CRITICAL("failed to create logical device: {}", resultDescription);
                throw std::runtime_error(String("VulkanLogicalDevice: failed to create logical device: ").append(resultDescription));
            }
        }
        //--------------------------------------------------------------------------

        void VulkanLogicalDevice::_GetDeviceQueues()
        {
            vkGetDeviceQueue(_device, _physicalDeviceInfo.graphicsFamilyIndex, 0, &_graphicsQueue);
            vkGetDeviceQueue(_device, _physicalDeviceInfo.presentFamilyIndex, 0, &_presentQueue);

            if (_graphicsQueue == nullptr)
            {
                KMP_LOG_CRITICAL("failed to get graphics queue from logical device");
                throw std::runtime_error("VulkanLogicalDevice: failed to get graphics queue from logical device");
            }

            if (_presentQueue == nullptr)
            {
                KMP_LOG_CRITICAL("failed to get present queue from logical device");
                throw std::runtime_error("VulkanLogicalDevice: failed to get present queue from logical device");
            }
        }
        //--------------------------------------------------------------------------

        void VulkanLogicalDevice::_CreateSemaphoreObjects()
        {
            VkSemaphoreCreateInfo semaphoreCreateInfo{};
            semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

            auto result = vkCreateSemaphore(_device, &semaphoreCreateInfo, nullptr, &_presentCompleteSemaphore);
            if (result != VK_SUCCESS)
            {
                const auto resultDescription = VkResultToString(result);
                KMP_LOG_CRITICAL("failed to create presentation complete semaphore: {}", resultDescription);
                throw std::runtime_error(String("VulkanLogicalDevice: failed to create presentation complete semaphore: ").append(resultDescription));
            }

            result = vkCreateSemaphore(_device, &semaphoreCreateInfo, nullptr, &_renderCompleteSemaphore);
            if (result != VK_SUCCESS)
            {
                const auto resultDescription = VkResultToString(result);
                KMP_LOG_CRITICAL("failed to create rendering complete semaphore: {}", resultDescription);
                throw std::runtime_error(String("VulkanLogicalDevice: failed to create rendering complete semaphore: ").append(resultDescription));
            }
        }
        //--------------------------------------------------------------------------

        void VulkanLogicalDevice::_InitializeSubmitInfo()
        {
            _submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
            _submitInfo.pWaitDstStageMask = &_submitPipelineStages;
            _submitInfo.waitSemaphoreCount = 1;
            _submitInfo.pWaitSemaphores = &_presentCompleteSemaphore;
            _submitInfo.signalSemaphoreCount = 1;
            _submitInfo.pSignalSemaphores = &_renderCompleteSemaphore;
        }
        //--------------------------------------------------------------------------

        void VulkanLogicalDevice::_CreateCommandBuffers()
        {
            _drawCommandBuffers.resize(dynamic_cast<VulkanSwapchain*>(_swapchain.get())->GetImageCount());

            VkCommandBufferAllocateInfo allocateInfo{};
            allocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
            allocateInfo.commandPool = dynamic_cast<VulkanCommandPool*>(_commandPool.get())->GetPool();
            allocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
            allocateInfo.commandBufferCount = UInt32(_drawCommandBuffers.size());

            const auto result = vkAllocateCommandBuffers(_device, &allocateInfo, _drawCommandBuffers.data());
            if (result != VK_SUCCESS)
            {
                const auto resultDescription = VkResultToString(result);
                KMP_LOG_CRITICAL("failed to create command buffers: {}", resultDescription);
                throw std::runtime_error(String("VulkanLogicalDevice: failed to create command buffers: ").append(resultDescription));
            }
        }
        //--------------------------------------------------------------------------

        void VulkanLogicalDevice::_DeleteCommandBuffers()
        {
            vkFreeCommandBuffers(_device, dynamic_cast<VulkanCommandPool*>(_commandPool.get())->GetPool(), UInt32(_drawCommandBuffers.size()), _drawCommandBuffers.data());
        }
        //--------------------------------------------------------------------------

        void VulkanLogicalDevice::_CreateFences()
        {
            VkFenceCreateInfo createInfo{};
            createInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
            createInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

            _waitFences.resize(UInt32(_drawCommandBuffers.size()));
            for (auto& fence : _waitFences)
            {
                const auto result = vkCreateFence(_device, &createInfo, nullptr, &fence);
                if (result != VK_SUCCESS)
                {
                    const auto resultDescription = VkResultToString(result);
                    KMP_LOG_CRITICAL("failed to create wait fence: {}", resultDescription);
                    throw std::runtime_error(String("VulkanLogicalDevice: failed to create wait fence: ").append(resultDescription));
                }
            }
        }
        //--------------------------------------------------------------------------

        void VulkanLogicalDevice::_DeleteFences()
        {
            for (auto& fence : _waitFences)
            {
                vkDestroyFence(_device, fence, nullptr);
            }
        }
        //--------------------------------------------------------------------------

        Vector<VkDeviceQueueCreateInfo> VulkanLogicalDevice::_CreateQueueCreateInfos() const
        {
            Vector<VkDeviceQueueCreateInfo> queueCreateInfos;
            Set<UInt32> queueFamiliesIndicesSet = {
                _physicalDeviceInfo.graphicsFamilyIndex,
                _physicalDeviceInfo.presentFamilyIndex
            };
            const auto queuePriority = 1.0f;

            for (auto queueFamilyIndex : queueFamiliesIndicesSet)
            {
                VkDeviceQueueCreateInfo queueCreateInfo{};
                queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
                queueCreateInfo.queueFamilyIndex = queueFamilyIndex;
                queueCreateInfo.queueCount = 1;
                queueCreateInfo.pQueuePriorities = &queuePriority;
                queueCreateInfos.push_back(queueCreateInfo);
            }

            return queueCreateInfos;
        }
        //--------------------------------------------------------------------------

        VkExtent2D VulkanLogicalDevice::_UpdateExtent() const
        {
            const auto& capabilities = _physicalDeviceInfo.surfaceCapabilities;
            if (capabilities.currentExtent.width != std::numeric_limits<UInt32>::max())
            {
                return capabilities.currentExtent;
            }

            const auto windowSize = _window.GetSize();
            const auto actualExtent = VkExtent2D{
                Math::Clamp(UInt32(windowSize.x), capabilities.minImageExtent.width, capabilities.maxImageExtent.width),
                Math::Clamp(UInt32(windowSize.y), capabilities.minImageExtent.height, capabilities.maxImageExtent.height)
            };

            return actualExtent;
        }
        //--------------------------------------------------------------------------
    }
}