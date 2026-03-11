#include "Kmplete/Graphics/Vulkan/vulkan_graphics_base.h"
#include "Kmplete/Graphics/Vulkan/vulkan_logical_device.h"
#include "Kmplete/Graphics/Vulkan/vulkan_physical_device.h"
#include "Kmplete/Graphics/Vulkan/vulkan_texture.h"
#include "Kmplete/Graphics/Vulkan/Utils/initializers.h"
#include "Kmplete/Graphics/Vulkan/Utils/result_description.h"
#include "Kmplete/Graphics/image.h"
#include "Kmplete/Window/window.h"
#include "Kmplete/Math/math.h"
#include "Kmplete/Math/geometry.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Filesystem/filesystem.h"
#include "Kmplete/Profile/profiler.h"
#include "Kmplete/Log/log.h"

#include <limits>
#include <stdexcept>


namespace Kmplete
{
    namespace Graphics
    {
        VulkanLogicalDevice::VulkanLogicalDevice(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, const VulkanContext& vulkanContext, const VulkanMemoryTypeDelegate& memoryTypeDelegate, 
                                                 const VulkanFormatDelegate& formatDelegate, const Window& window, const UInt32& currentBufferIndex)
            : LogicalDevice()
            , _vulkanContext(vulkanContext)
            , _memoryTypeDelegate(memoryTypeDelegate)
            , _formatDelegate(formatDelegate)
            , _window(window)
            , _currentBufferIndex(currentBufferIndex)
            , _physicalDevice(physicalDevice)
            , _surface(surface)
            , _device(nullptr)
            , _graphicsQueue(nullptr)
            , _presentQueue(nullptr)
            , _imageCreatorDelegate(nullptr)
            , _presentCompleteSemaphores()
            , _renderCompleteSemaphores()
            , _waitFences()
            , _commandPool(nullptr)
            , _swapchain(nullptr)
            , _drawCommandBuffers()
            , _depthStencilAttachment(nullptr)
            , _pipelineCache(VK_NULL_HANDLE)
            , _descriptorPool(VK_NULL_HANDLE)
            , _currentExtent(_UpdateExtent())
        {
            KMP_PROFILE_FUNCTION(ProfileLevelAlways);

            _CreateLogicalDeviceObject();
            _GetDeviceQueues();

            _imageCreatorDelegate.reset(new VulkanImageCreatorDelegate(_device, _memoryTypeDelegate));

            _CreateSynchronizationObjects();
            _CreateCommandPool();
            _CreateSwapchain();
            _CreateCommandBuffers();
            _CreateDepthStencilAttachment();
            _CreatePipelineCache();
            _CreateDescriptorPool();
        }
        //--------------------------------------------------------------------------

        VulkanLogicalDevice::~VulkanLogicalDevice()
        {
            KMP_PROFILE_FUNCTION(ProfileLevelAlways);

            _DeleteDescriptorPool();
            _DeletePipelineCache();
            _DeleteDepthStencilAttachment();
            _DeleteCommandBuffers();
            _DeleteSwapchain();
            _DeleteCommandPool();
            _DeleteSyncronizationObjects();

            _imageCreatorDelegate.reset();
            _DeleteLogicalDeviceObject();
        }
        //--------------------------------------------------------------------------

        void VulkanLogicalDevice::StartFrame(float frameTimestep)
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

            _StartFrameRestartFence();

            _swapchain->StartFrame(frameTimestep);

            _StartFrameRestartCommandBuffer();
            _StartFrameTransitionColorAndDepthStencilImages();
            _StartFrameBeginRendering();
            _StartFrameSetupViewportAndScissor();
        }
        //--------------------------------------------------------------------------

        void VulkanLogicalDevice::EndFrame()
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

            _EndFrameEndRendering();
            _EndFrameTransitionColorAndDepthStencilImages();
            _EndFrameEndCommandBuffer();
            _EndFrameQueueSubmit();

            _swapchain->EndFrame();

            WaitIdle();
        }
        //--------------------------------------------------------------------------

        void VulkanLogicalDevice::HandleWindowResize()
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

            WaitIdle();

            _DeleteDepthStencilAttachment();
            _DeleteCommandBuffers();
            _DeleteSwapchain();
            _DeleteSyncronizationObjects();

            _CreateSynchronizationObjects();
            _CreateSwapchain();
            _CreateCommandBuffers();
            _CreateDepthStencilAttachment();
        }
        //--------------------------------------------------------------------------

        void VulkanLogicalDevice::WaitIdle() const
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctionsVerbose);

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

        VkCommandBuffer VulkanLogicalDevice::GetCurrentVkCommandBuffer() const noexcept
        {
            return _drawCommandBuffers[_currentBufferIndex];
        }
        //--------------------------------------------------------------------------

        const VulkanImageCreatorDelegate& VulkanLogicalDevice::GetVulkanImageCreatorDelegate() const noexcept
        {
            return *_imageCreatorDelegate.get();
        }
        //--------------------------------------------------------------------------

        void VulkanLogicalDevice::_CreateLogicalDeviceObject()
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

            const auto queueCreateInfos = _CreateQueueCreateInfos();

            VkPhysicalDeviceFeatures features{};
            features.samplerAnisotropy = VK_TRUE;

            auto features13 = VulkanUtils::InitVkPhysicalDeviceVulkan13Features();
            features13.dynamicRendering = VK_TRUE;
            features13.synchronization2 = VK_TRUE;

            auto features2 = VulkanUtils::InitVkPhysicalDeviceFeatures2();
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
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

            vkGetDeviceQueue(_device, _vulkanContext.graphicsFamilyIndex, 0, &_graphicsQueue);
            vkGetDeviceQueue(_device, _vulkanContext.presentFamilyIndex, 0, &_presentQueue);

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

        void VulkanLogicalDevice::_DeleteLogicalDeviceObject()
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

            vkDestroyDevice(_device, nullptr);
        }
        //--------------------------------------------------------------------------

        void VulkanLogicalDevice::_CreateSynchronizationObjects()
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

            auto semaphoreCreateInfo = VulkanUtils::InitVkSemaphoreCreateInfo();
            auto fenceCreateInfo = VulkanUtils::InitVkFenceCreateInfo();

            for (UInt32 i = 0; i < NumConcurrentFrames; i++)
            {
                auto result = vkCreateSemaphore(_device, &semaphoreCreateInfo, nullptr, &_presentCompleteSemaphores[i]);
                VulkanUtils::CheckResult(result, "VulkanLogicalDevice: failed to create presentation complete semaphore");

                result = vkCreateSemaphore(_device, &semaphoreCreateInfo, nullptr, &_renderCompleteSemaphores[i]);
                VulkanUtils::CheckResult(result, "VulkanLogicalDevice: failed to create rendering complete semaphore");

                result = vkCreateFence(_device, &fenceCreateInfo, nullptr, &_waitFences[i]);
                VulkanUtils::CheckResult(result, "VulkanLogicalDevice: failed to create wait fence");
            }
        }
        //--------------------------------------------------------------------------

        void VulkanLogicalDevice::_DeleteSyncronizationObjects()
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

            for (UInt32 i = 0; i < NumConcurrentFrames; i++)
            {
                vkDestroySemaphore(_device, _presentCompleteSemaphores[i], nullptr);
                vkDestroySemaphore(_device, _renderCompleteSemaphores[i], nullptr);
                vkDestroyFence(_device, _waitFences[i], nullptr);
            }
        }
        //--------------------------------------------------------------------------

        void VulkanLogicalDevice::_CreateCommandPool()
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

            _commandPool.reset(new VulkanCommandPool(_device, _vulkanContext.graphicsFamilyIndex));
        }
        //--------------------------------------------------------------------------

        void VulkanLogicalDevice::_DeleteCommandPool()
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

            _commandPool.reset();
        }
        //--------------------------------------------------------------------------

        void VulkanLogicalDevice::_CreateSwapchain()
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

            _currentExtent = _UpdateExtent();
            _swapchain.reset(new VulkanSwapchain(_device, _graphicsQueue, _surface, _vulkanContext, _currentExtent, *_imageCreatorDelegate.get(), _currentBufferIndex, _presentCompleteSemaphores, _renderCompleteSemaphores));
        }
        //--------------------------------------------------------------------------

        void VulkanLogicalDevice::_DeleteSwapchain()
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

            _swapchain.reset();
        }
        //--------------------------------------------------------------------------

        void VulkanLogicalDevice::_CreateCommandBuffers()
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

            auto commandBufferAllocateInfo = VulkanUtils::InitVkCommandBufferAllocateInfo();
            commandBufferAllocateInfo.commandPool = _commandPool->GetVkCommandPool();
            commandBufferAllocateInfo.commandBufferCount = UInt32(_drawCommandBuffers.size());

            const auto result = vkAllocateCommandBuffers(_device, &commandBufferAllocateInfo, _drawCommandBuffers.data());
            VulkanUtils::CheckResult(result, "VulkanLogicalDevice: failed to create command buffers");
        }
        //--------------------------------------------------------------------------

        void VulkanLogicalDevice::_DeleteCommandBuffers()
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

            vkFreeCommandBuffers(_device, _commandPool->GetVkCommandPool(), UInt32(_drawCommandBuffers.size()), _drawCommandBuffers.data());
        }
        //--------------------------------------------------------------------------

        void VulkanLogicalDevice::_CreateDepthStencilAttachment()
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

            _depthStencilAttachment.reset(new VulkanDepthStencilAttachment(*_imageCreatorDelegate.get(), _device, _currentExtent, _vulkanContext.defaultDepthFormat));
        }
        //--------------------------------------------------------------------------

        void VulkanLogicalDevice::_DeleteDepthStencilAttachment()
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

            _depthStencilAttachment.reset();
        }
        //--------------------------------------------------------------------------

        void VulkanLogicalDevice::_CreatePipelineCache()
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

            auto pipelineCacheCreateInfo = VulkanUtils::InitVkPipelineCacheCreateInfo();

            const auto result = vkCreatePipelineCache(_device, &pipelineCacheCreateInfo, nullptr, &_pipelineCache);
            VulkanUtils::CheckResult(result, "VulkanLogicalDevice: failed to create pipeline cache");
        }
        //--------------------------------------------------------------------------

        void VulkanLogicalDevice::_DeletePipelineCache()
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

            vkDestroyPipelineCache(_device, _pipelineCache, nullptr);
        }
        //--------------------------------------------------------------------------

        void VulkanLogicalDevice::_CreateDescriptorPool()
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

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

        void VulkanLogicalDevice::_DeleteDescriptorPool()
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

            vkDestroyDescriptorPool(_device, _descriptorPool, nullptr);
        }
        //--------------------------------------------------------------------------

        Vector<VkDeviceQueueCreateInfo> VulkanLogicalDevice::_CreateQueueCreateInfos() const
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

            Vector<VkDeviceQueueCreateInfo> queueCreateInfos;
            Set<UInt32> queueFamiliesIndicesSet = {
                _vulkanContext.graphicsFamilyIndex,
                _vulkanContext.presentFamilyIndex
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
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctionsVerbose);

            const auto& capabilities = _vulkanContext.surfaceCapabilities;
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

        void VulkanLogicalDevice::_StartFrameRestartFence()
        {
            KMP_PROFILE_FUNCTION(ProfileLevelMinorFunctions);

            vkWaitForFences(_device, 1, &_waitFences[_currentBufferIndex], VK_TRUE, UINT64_MAX);
            auto result = vkResetFences(_device, 1, &_waitFences[_currentBufferIndex]);
            VulkanUtils::CheckResult(result, "VulkanLogicalDevice: failed to reset wait fence");
        }
        //--------------------------------------------------------------------------

        void VulkanLogicalDevice::_StartFrameRestartCommandBuffer()
        {
            KMP_PROFILE_FUNCTION(ProfileLevelMinorFunctions);

            vkResetCommandBuffer(_drawCommandBuffers[_currentBufferIndex], 0);
            auto commandBufferBeginInfo = VulkanUtils::InitVkCommandBufferBeginInfo();
            auto result = vkBeginCommandBuffer(_drawCommandBuffers[_currentBufferIndex], &commandBufferBeginInfo);
            VulkanUtils::CheckResult(result, "VulkanLogicalDevice: failed to begin command buffer");
        }

        void VulkanLogicalDevice::_StartFrameTransitionColorAndDepthStencilImages()
        {
            KMP_PROFILE_FUNCTION(ProfileLevelMinorFunctions);

            VulkanUtils::MemoryBarrierParameters barrierParameters = {
                .cmdbuffer = _drawCommandBuffers[_currentBufferIndex],
                .image = _swapchain->GetCurrentImage(),
                .srcAccessMask = VK_ACCESS_NONE,
                .dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
                .oldImageLayout = VK_IMAGE_LAYOUT_UNDEFINED,
                .newImageLayout = VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL,
                .srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
                .dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
                .subresourceRange = VkImageSubresourceRange{ VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 }
            };
            VulkanUtils::InsertImageMemoryBarrier(barrierParameters);

            barrierParameters = {
                .cmdbuffer = _drawCommandBuffers[_currentBufferIndex],
                .image = _depthStencilAttachment->GetImage(),
                .srcAccessMask = VK_ACCESS_NONE,
                .dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,
                .oldImageLayout = VK_IMAGE_LAYOUT_UNDEFINED,
                .newImageLayout = VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL,
                .srcStageMask = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT,
                .dstStageMask = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT,
                .subresourceRange = VkImageSubresourceRange{ VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT, 0, 1, 0, 1 }
            };
            VulkanUtils::InsertImageMemoryBarrier(barrierParameters);
        }
        //--------------------------------------------------------------------------

        void VulkanLogicalDevice::_StartFrameBeginRendering()
        {
            KMP_PROFILE_FUNCTION(ProfileLevelMinorFunctions);

            auto colorAttachmentInfo = VulkanUtils::InitVkRenderingAttachmentInfo();
            colorAttachmentInfo.imageView = _swapchain->GetCurrentImageView();
            colorAttachmentInfo.imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
            colorAttachmentInfo.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
            colorAttachmentInfo.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
            colorAttachmentInfo.clearValue.color = { 0.0f, 0.0f, 0.0f, 0.0f };

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
        }
        //--------------------------------------------------------------------------

        void VulkanLogicalDevice::_StartFrameSetupViewportAndScissor()
        {
            KMP_PROFILE_FUNCTION(ProfileLevelMinorFunctions);

            VkViewport viewport{ 0.0f, 0.0f, float(_currentExtent.width), float(_currentExtent.height) };
            vkCmdSetViewport(_drawCommandBuffers[_currentBufferIndex], 0, 1, &viewport);

            VkRect2D scissor{ 0, 0, _currentExtent.width, _currentExtent.height };
            vkCmdSetScissor(_drawCommandBuffers[_currentBufferIndex], 0, 1, &scissor);
        }
        //--------------------------------------------------------------------------

        void VulkanLogicalDevice::_EndFrameEndRendering()
        {
            KMP_PROFILE_FUNCTION(ProfileLevelMinorFunctions);

            vkCmdEndRendering(_drawCommandBuffers[_currentBufferIndex]);
        }
        //--------------------------------------------------------------------------

        void VulkanLogicalDevice::_EndFrameTransitionColorAndDepthStencilImages()
        {
            KMP_PROFILE_FUNCTION(ProfileLevelMinorFunctions);

            VulkanUtils::MemoryBarrierParameters barrierParameters = {
                .cmdbuffer = _drawCommandBuffers[_currentBufferIndex],
                .image = _swapchain->GetCurrentImage(),
                .srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
                .dstAccessMask = VK_ACCESS_NONE,
                .oldImageLayout = VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL,
                .newImageLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
                .srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
                .dstStageMask = VK_PIPELINE_STAGE_2_NONE,
                .subresourceRange = VkImageSubresourceRange{ VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 }
            };
            VulkanUtils::InsertImageMemoryBarrier(barrierParameters);
        }
        //--------------------------------------------------------------------------

        void VulkanLogicalDevice::_EndFrameEndCommandBuffer()
        {
            KMP_PROFILE_FUNCTION(ProfileLevelMinorFunctions);

            auto result = vkEndCommandBuffer(_drawCommandBuffers[_currentBufferIndex]);
            VulkanUtils::CheckResult(result, "VulkanLogicalDevice: failed to end command buffer");
        }
        //--------------------------------------------------------------------------

        void VulkanLogicalDevice::_EndFrameQueueSubmit()
        {
            KMP_PROFILE_FUNCTION(ProfileLevelMinorFunctions);

            VkPipelineStageFlags waitStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
            auto submitInfo = VulkanUtils::InitVkSubmitInfo();
            submitInfo.pWaitDstStageMask = &waitStageMask;
            submitInfo.pCommandBuffers = &_drawCommandBuffers[_currentBufferIndex];
            submitInfo.commandBufferCount = 1;
            submitInfo.pWaitSemaphores = &_presentCompleteSemaphores[_currentBufferIndex];
            submitInfo.waitSemaphoreCount = 1;
            submitInfo.pSignalSemaphores = &_renderCompleteSemaphores[_currentBufferIndex];
            submitInfo.signalSemaphoreCount = 1;

            const auto result = vkQueueSubmit(_graphicsQueue, 1, &submitInfo, _waitFences[_currentBufferIndex]);
            VulkanUtils::CheckResult(result, "VulkanLogicalDevice: failed to submit commands to queue");
        }
        //--------------------------------------------------------------------------

        Nullable<VulkanTexture*> VulkanLogicalDevice::CreateTexture(const Image& image) const
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

            auto commandBuffer = VulkanUtils::StartSingleTimeCommandBuffer(_device, _commandPool->GetVkCommandPool());
            try
            {
                const auto textureVkFormat = ImageChannelsToVkFormat(ImageChannels(image.GetChannels()));
                const auto mipmapEnabled = _formatDelegate.IsMipmapCompatible(textureVkFormat);
                auto imageBuffer = _imageCreatorDelegate->CreateStagingImageBuffer(image);
                auto* texture = new VulkanTexture(textureVkFormat, mipmapEnabled, _device, commandBuffer, imageBuffer, image, *_imageCreatorDelegate.get());
                VulkanUtils::EndSingleTimeCommandBuffer(_device, commandBuffer, _commandPool->GetVkCommandPool(), _graphicsQueue);
                return texture;
            }
            catch (KMP_MB_UNUSED const std::runtime_error& e)
            {
                KMP_LOG_ERROR("failed to create a texture - {}", e.what());
                vkFreeCommandBuffers(_device, _commandPool->GetVkCommandPool(), 1, &commandBuffer);
            }

            return nullptr;
        }
        //--------------------------------------------------------------------------

        VulkanBuffer VulkanLogicalDevice::CreateBuffer(VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags memoryPropertyFlags, VkDeviceSize size) const
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

            return VulkanBuffer(_memoryTypeDelegate, _device, usageFlags, memoryPropertyFlags, size);
        }
        //--------------------------------------------------------------------------

        Nullable<VulkanBuffer*> VulkanLogicalDevice::CreateBufferPtr(VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags memoryPropertyFlags, VkDeviceSize size) const
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

            return new VulkanBuffer(_memoryTypeDelegate, _device, usageFlags, memoryPropertyFlags, size);
        }
        //--------------------------------------------------------------------------

        VkShaderModule VulkanLogicalDevice::CreateShaderModule(const Filepath& filename) const
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

            if (!Filesystem::FilepathExists(filename))
            {
                KMP_LOG_ERROR("shader file not found '{}'", filename);
                return VK_NULL_HANDLE;
            }

            const auto shaderBinary = Filesystem::ReadFileAsBinary(filename);
            if (shaderBinary.empty())
            {
                KMP_LOG_ERROR("failed to load shader binary from '{}'", filename);
                return VK_NULL_HANDLE;
            }

            if (shaderBinary.size() % 4 != 0)
            {
                KMP_LOG_ERROR("shader binary size is not multiple of four '{}'", filename);
                return VK_NULL_HANDLE;
            }

            auto shaderModuleCreateInfo = Graphics::VulkanUtils::InitVkShaderModuleCreateInfo();
            shaderModuleCreateInfo.codeSize = shaderBinary.size();
            shaderModuleCreateInfo.pCode = reinterpret_cast<const UInt32*>(shaderBinary.data());

            VkShaderModule shaderModule;
            auto result = vkCreateShaderModule(_device, &shaderModuleCreateInfo, nullptr, &shaderModule);
            Graphics::VulkanUtils::CheckResult(result, "VulkanLogicalDevice: failed to create shader module");

            return shaderModule;
        }
        //--------------------------------------------------------------------------

        Vector<VkPipelineShaderStageCreateInfo> VulkanLogicalDevice::CreateShadersInitializers(const Vector<VulkanUtils::ShaderCreateInfo>& shaderInfos) const
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

            Vector<VkPipelineShaderStageCreateInfo> initializers;
            initializers.reserve(shaderInfos.size());

            for (const auto& shaderInfo : shaderInfos)
            {
                auto createInfo = VulkanUtils::InitVkPipelineShaderStageCreateInfo(shaderInfo.stage);
                createInfo.module = CreateShaderModule(shaderInfo.filepath);
                createInfo.pName = shaderInfo.entryPointName;

                initializers.push_back(createInfo);
            }

            return initializers;
        }
        //--------------------------------------------------------------------------
    }
}