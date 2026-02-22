#include "Kmplete/Graphics/Vulkan/vulkan_logical_device.h"
#include "Kmplete/Graphics/Vulkan/vulkan_physical_device.h"
#include "Kmplete/Graphics/Vulkan/Utils/initializers.h"
#include "Kmplete/Graphics/Vulkan/Utils/function_utils.h"
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
        VulkanLogicalDevice::VulkanLogicalDevice(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, const PhysicalDeviceInfo& info, const Window& window, const UInt32& currentBufferIndex)
            : LogicalDevice()
            , _physicalDevice(physicalDevice)
            , _surface(surface)
            , _physicalDeviceInfo(info)
            , _window(window)
            , _currentBufferIndex(currentBufferIndex)
            , _device(nullptr)
            , _graphicsQueue(nullptr)
            , _presentQueue(nullptr)
            , _presentCompleteSemaphores()
            , _renderCompleteSemaphores()
            , _waitFences()
            , _depthStencilAttachment(nullptr)
            , _commandPool(nullptr)
            , _drawCommandBuffers()
            , _swapchain(nullptr)
            , _pipelineCache(VK_NULL_HANDLE)
            , _descriptorPool(VK_NULL_HANDLE)
            , _currentExtent(_UpdateExtent())
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
            _CreateDepthStencilAttachment();
            _CreatePipelineCache();
            _CreateDescriptorPool();
        }
        //--------------------------------------------------------------------------

        VulkanLogicalDevice::~VulkanLogicalDevice()
        {
            vkDestroyDescriptorPool(_device, _descriptorPool, nullptr);
            vkDestroyPipelineCache(_device, _pipelineCache, nullptr);

            _DeleteDepthStencilAttachment();
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

        void VulkanLogicalDevice::StartFrame(float frameTimestep)
        {
            vkWaitForFences(_device, 1, &_waitFences[_currentBufferIndex], VK_TRUE, UINT64_MAX);
            auto result = vkResetFences(_device, 1, &_waitFences[_currentBufferIndex]);
            VulkanUtils::CheckResult(result, "VulkanLogicalDevice: failed to reset wait fence");

            _swapchain->StartFrame(frameTimestep);

            vkResetCommandBuffer(_drawCommandBuffers[_currentBufferIndex], 0);
            auto commandBufferBeginInfo = VulkanUtils::InitVkCommandBufferBeginInfo();
            result = vkBeginCommandBuffer(_drawCommandBuffers[_currentBufferIndex], &commandBufferBeginInfo);
            VulkanUtils::CheckResult(result, "VulkanLogicalDevice: failed to begin command buffer");

            VulkanUtils::InsertImageMemoryBarrier(
                _drawCommandBuffers[_currentBufferIndex], 
                _swapchain->GetCurrentImage(), 
                0, 
                VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT, 
                VK_IMAGE_LAYOUT_UNDEFINED, 
                VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL, 
                VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, 
                VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, 
                VkImageSubresourceRange{ VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 }
            );
            VulkanUtils::InsertImageMemoryBarrier(
                _drawCommandBuffers[_currentBufferIndex],
                _depthStencilAttachment->GetImage(), 
                0, 
                VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT, 
                VK_IMAGE_LAYOUT_UNDEFINED, 
                VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL, 
                VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT, 
                VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT, 
                VkImageSubresourceRange{ VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT, 0, 1, 0, 1 }
            );

            auto colorAttachmentInfo = VulkanUtils::InitVkRenderingAttachmentInfo();
            colorAttachmentInfo.imageView = _swapchain->GetCurrentImageView();
            colorAttachmentInfo.imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
            colorAttachmentInfo.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
            colorAttachmentInfo.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
            colorAttachmentInfo.clearValue.color = { 0.0f, 0.0f, 0.2f, 0.0f }; //TODO: numbers

            auto depthStencilAttachmentInfo = VulkanUtils::InitVkRenderingAttachmentInfo();
            depthStencilAttachmentInfo.imageView = _depthStencilAttachment->GetImageView();
            depthStencilAttachmentInfo.imageLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
            depthStencilAttachmentInfo.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
            depthStencilAttachmentInfo.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
            depthStencilAttachmentInfo.clearValue.depthStencil = { 1.0f, 0 };

            auto renderingInfo = VulkanUtils::InitVkRenderingInfo();
            renderingInfo.renderArea = { 0, 0, _currentExtent.width, _currentExtent.height };
            renderingInfo.layerCount = 1;
            renderingInfo.colorAttachmentCount = 1;
            renderingInfo.pColorAttachments = &colorAttachmentInfo;
            renderingInfo.pDepthAttachment = &depthStencilAttachmentInfo;
            renderingInfo.pStencilAttachment = &depthStencilAttachmentInfo;

            vkCmdBeginRendering(_drawCommandBuffers[_currentBufferIndex], &renderingInfo);

            VkViewport viewport{ 0.0f, 0.0f, float(_currentExtent.width), float(_currentExtent.height) };
            vkCmdSetViewport(_drawCommandBuffers[_currentBufferIndex], 0, 1, &viewport);

            VkRect2D scissor{ 0, 0, _currentExtent.width, _currentExtent.height };
            vkCmdSetScissor(_drawCommandBuffers[_currentBufferIndex], 0, 1, &scissor);
        }
        //--------------------------------------------------------------------------

        void VulkanLogicalDevice::EndFrame()
        {
            vkCmdEndRendering(_drawCommandBuffers[_currentBufferIndex]);

            VulkanUtils::InsertImageMemoryBarrier(
                _drawCommandBuffers[_currentBufferIndex], 
                _swapchain->GetCurrentImage(), 
                VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT, 
                0, 
                VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL, 
                VK_IMAGE_LAYOUT_PRESENT_SRC_KHR, 
                VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, 
                VK_PIPELINE_STAGE_2_NONE, 
                VkImageSubresourceRange{ VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 }
            );

            auto result = vkEndCommandBuffer(_drawCommandBuffers[_currentBufferIndex]);
            VulkanUtils::CheckResult(result, "VulkanLogicalDevice: failed to end command buffer");

            VkPipelineStageFlags waitStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
            auto submitInfo = VulkanUtils::InitVkSubmitInfo();
            submitInfo.pWaitDstStageMask = &waitStageMask;
            submitInfo.pCommandBuffers = &_drawCommandBuffers[_currentBufferIndex];
            submitInfo.commandBufferCount = 1;
            submitInfo.pWaitSemaphores = &_presentCompleteSemaphores[_currentBufferIndex];
            submitInfo.waitSemaphoreCount = 1;
            submitInfo.pSignalSemaphores = &_renderCompleteSemaphores[_currentBufferIndex];
            submitInfo.signalSemaphoreCount = 1;

            result = vkQueueSubmit(_graphicsQueue, 1, &submitInfo, _waitFences[_currentBufferIndex]);
            VulkanUtils::CheckResult(result, "VulkanLogicalDevice: failed to submit commands to queue");

            _swapchain->EndFrame();

            if (_device != VK_NULL_HANDLE)
            {
                vkDeviceWaitIdle(_device);
            }
        }
        //--------------------------------------------------------------------------

        const CommandPool& VulkanLogicalDevice::GetCommandPool() const noexcept
        {
            return *_commandPool.get();
        }
        //--------------------------------------------------------------------------

        const Swapchain& VulkanLogicalDevice::GetSwapchain() const noexcept
        {
            return *_swapchain.get();
        }
        //--------------------------------------------------------------------------

        void VulkanLogicalDevice::CreateSwapchain()
        {
            _currentExtent = _UpdateExtent();
            _swapchain.reset(new VulkanSwapchain(_device, _graphicsQueue, _surface, _physicalDeviceInfo, _currentExtent, *_imageCreatorDelegate.get(), _currentBufferIndex, _presentCompleteSemaphores, _renderCompleteSemaphores));
        }
        //--------------------------------------------------------------------------

        void VulkanLogicalDevice::DeleteSwapchain()
        {
            _swapchain.reset();
        }
        //--------------------------------------------------------------------------

        void VulkanLogicalDevice::RecreateSwapchain()
        {
            vkDeviceWaitIdle(_device);

            _DeleteDepthStencilAttachment();
            _DeleteFences();
            _DeleteCommandBuffers();
            DeleteSwapchain();

            CreateSwapchain();
            _CreateCommandBuffers();
            _CreateFences();
            _CreateDepthStencilAttachment();
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

            VkPhysicalDeviceFeatures features{};
            features.samplerAnisotropy = VK_TRUE;

            VkPhysicalDeviceVulkan13Features features13{};
            features13.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES;
            features13.dynamicRendering = VK_TRUE;
            features13.synchronization2 = VK_TRUE;

            VkPhysicalDeviceFeatures2 features2{};
            features2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
            features2.features = features;
            features2.pNext = &features13;

            const auto& enabledDeviceExtensions = VulkanPhysicalDevice::GetEnabledDeviceExtensions();

            auto deviceCreateInfo = VulkanUtils::InitVkDeviceCreateInfo();
            deviceCreateInfo.queueCreateInfoCount = UInt32(queueCreateInfos.size());
            deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();
            deviceCreateInfo.pEnabledFeatures = nullptr;
            deviceCreateInfo.enabledExtensionCount = UInt32(enabledDeviceExtensions.size());
            deviceCreateInfo.ppEnabledExtensionNames = enabledDeviceExtensions.data();
            deviceCreateInfo.pNext = &features2;

            const auto result = vkCreateDevice(_physicalDevice, &deviceCreateInfo, nullptr, &_device);
            VulkanUtils::CheckResult(result, "VulkanLogicalDevice: failed to create logical device");
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
            auto semaphoreCreateInfo = VulkanUtils::InitVkSemaphoreCreateInfo();

            for (UInt32 i = 0; i < NumConcurrentFrames; i++)
            {
                auto result = vkCreateSemaphore(_device, &semaphoreCreateInfo, nullptr, &_presentCompleteSemaphores[i]);
                VulkanUtils::CheckResult(result, "VulkanLogicalDevice: failed to create presentation complete semaphore");

                result = vkCreateSemaphore(_device, &semaphoreCreateInfo, nullptr, &_renderCompleteSemaphores[i]);
                VulkanUtils::CheckResult(result, "VulkanLogicalDevice: failed to create rendering complete semaphore");
            }
        }
        //--------------------------------------------------------------------------

        void VulkanLogicalDevice::_CreateCommandBuffers()
        {
            auto commandBufferAllocateInfo = VulkanUtils::InitVkCommandBufferAllocateInfo();
            commandBufferAllocateInfo.commandPool = _commandPool->GetVkCommandPool();
            commandBufferAllocateInfo.commandBufferCount = UInt32(_drawCommandBuffers.size());

            const auto result = vkAllocateCommandBuffers(_device, &commandBufferAllocateInfo, _drawCommandBuffers.data());
            VulkanUtils::CheckResult(result, "VulkanLogicalDevice: failed to create command buffers");
        }
        //--------------------------------------------------------------------------

        void VulkanLogicalDevice::_DeleteCommandBuffers()
        {
            vkFreeCommandBuffers(_device, _commandPool->GetVkCommandPool(), UInt32(_drawCommandBuffers.size()), _drawCommandBuffers.data());
        }
        //--------------------------------------------------------------------------

        void VulkanLogicalDevice::_CreateFences()
        {
            auto createInfo = VulkanUtils::InitVkFenceCreateInfo();

            for (auto& fence : _waitFences)
            {
                const auto result = vkCreateFence(_device, &createInfo, nullptr, &fence);
                VulkanUtils::CheckResult(result, "VulkanLogicalDevice: failed to create wait fence");
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

        void VulkanLogicalDevice::_CreateDepthStencilAttachment()
        {
            _depthStencilAttachment.reset(new VulkanDepthStencilAttachment(_physicalDeviceInfo, _device, _currentExtent, _physicalDeviceInfo.defaultDepthFormat));
        }
        //--------------------------------------------------------------------------

        void VulkanLogicalDevice::_DeleteDepthStencilAttachment()
        {
            _depthStencilAttachment.reset();
        }
        //--------------------------------------------------------------------------

        void VulkanLogicalDevice::_CreatePipelineCache()
        {
            auto pipelineCacheCreateInfo = VulkanUtils::InitVkPipelineCacheCreateInfo();

            const auto result = vkCreatePipelineCache(_device, &pipelineCacheCreateInfo, nullptr, &_pipelineCache);
            VulkanUtils::CheckResult(result, "VulkanLogicalDevice: failed to create pipeline cache");
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

            auto poolInfo = VulkanUtils::InitVkDescriptorPoolCreateInfo();
            poolInfo.maxSets = 100;
            poolInfo.poolSizeCount = UInt32(std::size(poolSizes));
            poolInfo.pPoolSizes = poolSizes;

            const auto result = vkCreateDescriptorPool(_device, &poolInfo, nullptr, &_descriptorPool);
            VulkanUtils::CheckResult(result, "VulkanLogicalDevice: failed to create descriptor pool");
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
                auto queueCreateInfo = VulkanUtils::InitVkDeviceQueueCreateInfo();
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