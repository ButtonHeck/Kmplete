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
            , _presentCompleteSemaphores()
            , _renderCompleteSemaphores()
            , _waitFences()
            , _drawCommandBuffers()
            , _pipelineCache(VK_NULL_HANDLE)
            , _descriptorPool(VK_NULL_HANDLE)
            , _currentExtent()
            , _imageCreatorDelegate(nullptr)
        {
            KMP_PROFILE_FUNCTION(ProfileLevelAlways);

            _CreateLogicalDeviceObject();
            _GetDeviceQueues();
            _CreateSemaphoreObjects();

            _commandPool.reset(new VulkanCommandPool(_device, _physicalDeviceInfo.graphicsFamilyIndex));
            _imageCreatorDelegate.reset(new VulkanImageCreatorDelegate(_device, _physicalDeviceInfo));

            CreateSwapchain();

            _CreateCommandBuffers();
            _CreateFences();
            _CreatePipelineCache();
            _CreateDescriptorPool();
        }
        //--------------------------------------------------------------------------

        VulkanLogicalDevice::~VulkanLogicalDevice()
        {
            vkDestroyDescriptorPool(_device, _descriptorPool, nullptr);
            vkDestroyPipelineCache(_device, _pipelineCache, nullptr);

            _DeleteFences();
            _DeleteCommandBuffers();
            DeleteSwapchain();

            _imageCreatorDelegate.reset();
            _commandPool.reset();

            for (UInt32 i = 0; i < NumConcurrentFrames; i++)
            {
                vkDestroySemaphore(_device, _presentCompleteSemaphores[i], nullptr);
                vkDestroySemaphore(_device, _renderCompleteSemaphores[i], nullptr);
            }

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

        VkDevice VulkanLogicalDevice::GetVkDevice() const noexcept
        {
            return _device;
        }
        //--------------------------------------------------------------------------

        VkQueue VulkanLogicalDevice::GetVkGraphicsQueue() const noexcept
        {
            return _graphicsQueue;
        }
        //--------------------------------------------------------------------------

        VkQueue VulkanLogicalDevice::GetVkPresentQueue() const noexcept
        {
            return _presentQueue;
        }
        //--------------------------------------------------------------------------

        VkDescriptorPool VulkanLogicalDevice::GetVkDescriptorPool() const noexcept
        {
            return _descriptorPool;
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

            for (UInt32 i = 0; i < NumConcurrentFrames; i++)
            {
                auto result = vkCreateSemaphore(_device, &semaphoreCreateInfo, nullptr, &_presentCompleteSemaphores[i]);
                if (result != VK_SUCCESS)
                {
                    const auto resultDescription = VkResultToString(result);
                    KMP_LOG_CRITICAL("failed to create presentation complete semaphore: {}", resultDescription);
                    throw std::runtime_error(String("VulkanLogicalDevice: failed to create presentation complete semaphore: ").append(resultDescription));
                }

                result = vkCreateSemaphore(_device, &semaphoreCreateInfo, nullptr, &_renderCompleteSemaphores[i]);
                if (result != VK_SUCCESS)
                {
                    const auto resultDescription = VkResultToString(result);
                    KMP_LOG_CRITICAL("failed to create rendering complete semaphore: {}", resultDescription);
                    throw std::runtime_error(String("VulkanLogicalDevice: failed to create rendering complete semaphore: ").append(resultDescription));
                }
            }
        }
        //--------------------------------------------------------------------------

        void VulkanLogicalDevice::_CreateCommandBuffers()
        {
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

        void VulkanLogicalDevice::_CreatePipelineCache()
        {
            VkPipelineCacheCreateInfo cacheCreateInfo{};
            cacheCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;

            const auto result = vkCreatePipelineCache(_device, &cacheCreateInfo, nullptr, &_pipelineCache);
            if (result != VK_SUCCESS)
            {
                const auto resultDescription = VkResultToString(result);
                KMP_LOG_CRITICAL("failed to create pipeline cache: {}", resultDescription);
                throw std::runtime_error(String("VulkanLogicalDevice: failed to create pipeline cache: ").append(resultDescription));
            }
        }
        //--------------------------------------------------------------------------

        void VulkanLogicalDevice::_CreateDescriptorPool()
        {
            //TODO: fix numbers
            VkDescriptorPoolSize poolSizes[] = { 
                { VK_DESCRIPTOR_TYPE_SAMPLER, 100 },
                { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 100 },
                { VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 100 },
                { VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 100 },
                { VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 100 },
                { VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 100 },
                { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 100 },
                { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 100 },
                { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 100 },
                { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 100 },
                { VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 100 } };

            VkDescriptorPoolCreateInfo poolInfo{};
            poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
            poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
            poolInfo.maxSets = 100;
            poolInfo.poolSizeCount = UInt32(std::size(poolSizes));
            poolInfo.pPoolSizes = poolSizes;

            const auto result = vkCreateDescriptorPool(_device, &poolInfo, nullptr, &_descriptorPool);
            if (result != VK_SUCCESS)
            {
                const auto resultDescription = VkResultToString(result);
                KMP_LOG_CRITICAL("failed to create descriptor pool: {}", resultDescription);
                throw std::runtime_error(String("VulkanLogicalDevice: failed to create descriptor pool: ").append(resultDescription));
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