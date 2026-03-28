#include "Kmplete/Graphics/Vulkan/vulkan_graphics_base.h"
#include "Kmplete/Graphics/Vulkan/vulkan_logical_device.h"
#include "Kmplete/Graphics/Vulkan/vulkan_physical_device.h"
#include "Kmplete/Graphics/Vulkan/vulkan_texture.h"
#include "Kmplete/Graphics/Vulkan/Utils/initializers.h"
#include "Kmplete/Graphics/Vulkan/Utils/result_description.h"
#include "Kmplete/Graphics/Vulkan/Utils/function_utils.h"
#include "Kmplete/Graphics/Vulkan/Utils/extension_functions.h"
#include "Kmplete/Graphics/Vulkan/Utils/presets.h"
#include "Kmplete/Graphics/image.h"
#include "Kmplete/Base/named_bool.h"
#include "Kmplete/Window/window.h"
#include "Kmplete/Math/math.h"
#include "Kmplete/Math/geometry.h"
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
            , _pipelineCache(VK_NULL_HANDLE)
            , _descriptorPool(VK_NULL_HANDLE)
            , _pipelines()
            , _descriptorSetLayouts()
            , _bufferCreatorDelegate(nullptr)
            , _currentExtent(_UpdateExtent())
            , _msaaSamples(VK_SAMPLE_COUNT_1_BIT)
            , _renderer(nullptr)
        {
            KMP_PROFILE_FUNCTION(ProfileLevelAlways);

            _CreateLogicalDeviceObject();
            _CreateDeviceQueues();

            _imageCreatorDelegate.reset(new VulkanImageCreatorDelegate(_device, _memoryTypeDelegate));

            _CreateSynchronizationObjects();
            _CreateCommandPool();
            _CreateSwapchain();
            _CreatePipelineCache();
            _CreateDescriptorPool();

            _bufferCreatorDelegate.reset(new VulkanBufferCreatorDelegate(_device, _descriptorPool, _memoryTypeDelegate));
            _renderer.reset(new VulkanRenderer(_device, _commandPool->GetVkCommandPool(), _currentBufferIndex, _currentExtent));
        }
        //--------------------------------------------------------------------------

        VulkanLogicalDevice::~VulkanLogicalDevice()
        {
            KMP_PROFILE_FUNCTION(ProfileLevelAlways);

            _renderer.reset();
            _bufferCreatorDelegate.reset();

            for (const auto& [sid, descriptorSetLayout] : _descriptorSetLayouts)
            {
                vkDestroyDescriptorSetLayout(_device, descriptorSetLayout, nullptr);
            }
            _descriptorSetLayouts.clear();

            _pipelines.clear();
            _DeleteDescriptorPool();
            _DeletePipelineCache();
            _DeleteSwapchain();
            _DeleteCommandPool();
            _DeleteSyncronizationObjects();

            _imageCreatorDelegate.reset();
            _graphicsQueue.reset();
            _presentQueue.reset();

            _DeleteLogicalDeviceObject();
        }
        //--------------------------------------------------------------------------

        void VulkanLogicalDevice::StartFrame(float frameTimestep)
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

            _StartFrameRestartFence();

            _swapchain->StartFrame(frameTimestep);

            _renderer->StartFrame();
            _StartFrameTransitionColorAndDepthStencilImages();
            _StartFrameBeginRendering();
            _StartFrameSetupViewportAndScissor();
            _StartFrameSetupRasterizationSamples();
        }
        //--------------------------------------------------------------------------

        void VulkanLogicalDevice::EndFrame()
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

            _EndFrameEndRendering();
            _EndFrameTransitionColorAndDepthStencilImages();
            _renderer->EndFrame();
            _EndFrameQueueSubmit();

            _swapchain->EndFrame();

            WaitIdle();
        }
        //--------------------------------------------------------------------------

        void VulkanLogicalDevice::HandleWindowResize()
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

            WaitIdle();

            _DeleteSwapchain();
            _DeleteSyncronizationObjects();

            _CreateSynchronizationObjects();
            _CreateSwapchain();
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

        VkSampleCountFlagBits VulkanLogicalDevice::GetMultisampling() const noexcept
        {
            return _msaaSamples;
        }
        //--------------------------------------------------------------------------

        void VulkanLogicalDevice::SetMultisampling(VkSampleCountFlagBits samples)
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

            _msaaSamples = samples;
            if (_msaaSamples > _vulkanContext.supportedSampleCounts.top())
            {
                KMP_LOG_WARN("cannot set MSAA samples to {}, set to maximum supported {}", UInt32(samples), UInt32(_vulkanContext.supportedSampleCounts.top()));
                _msaaSamples = _vulkanContext.supportedSampleCounts.top();
            }

            _swapchain->SetMultisampling(_msaaSamples);
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

        const VulkanQueue& VulkanLogicalDevice::GetGraphicsQueue() const noexcept
        {
            return *_graphicsQueue.get();
        }
        //--------------------------------------------------------------------------

        const VulkanQueue& VulkanLogicalDevice::GetPresentationQueue() const noexcept
        {
            return *_presentQueue.get();
        }
        //--------------------------------------------------------------------------

        VkDescriptorPool VulkanLogicalDevice::GetVkDescriptorPool() const noexcept
        {
            return _descriptorPool;
        }
        //--------------------------------------------------------------------------

        const VulkanImageCreatorDelegate& VulkanLogicalDevice::GetVulkanImageCreatorDelegate() const noexcept
        {
            return *_imageCreatorDelegate.get();
        }
        //--------------------------------------------------------------------------

        const VulkanBufferCreatorDelegate& VulkanLogicalDevice::GetVulkanBufferCreatorDelegate() const noexcept
        {
            return *_bufferCreatorDelegate.get();
        }
        //--------------------------------------------------------------------------

        const VulkanRenderer& VulkanLogicalDevice::GetRenderer() const noexcept
        {
            return *_renderer.get();
        }
        //--------------------------------------------------------------------------

        void VulkanLogicalDevice::_CreateLogicalDeviceObject()
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

            const auto queueCreateInfos = _CreateQueueCreateInfos();

            auto dynamicStateFeatures3 = VulkanUtils::InitVkPhysicalDeviceExtendedDynamicState3FeaturesEXT();
            dynamicStateFeatures3.extendedDynamicState3ColorBlendEnable = VK_TRUE;
            dynamicStateFeatures3.extendedDynamicState3RasterizationSamples = VK_TRUE;

            VkPhysicalDeviceFeatures features{};
            features.samplerAnisotropy = VK_TRUE;
            features.independentBlend = VK_TRUE;

            auto features13 = VulkanUtils::InitVkPhysicalDeviceVulkan13Features();
            features13.dynamicRendering = VK_TRUE;
            features13.synchronization2 = VK_TRUE;
            features13.pNext = &dynamicStateFeatures3;

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

        void VulkanLogicalDevice::_CreateDeviceQueues()
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

            const auto graphicsQueueSupportPresentation = (_vulkanContext.graphicsFamilyIndex == _vulkanContext.presentFamilyIndex);
            _graphicsQueue.reset(new VulkanQueue(_device, _vulkanContext.graphicsFamilyIndex, graphicsQueueSupportPresentation));
            _presentQueue.reset(new VulkanQueue(_device, _vulkanContext.presentFamilyIndex, "support presentation"_true));
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

            for (UInt32 i = 0; i < NumConcurrentFrames; i++)
            {
                auto result = vkCreateSemaphore(_device, &semaphoreCreateInfo, nullptr, &_presentCompleteSemaphores[i]);
                VulkanUtils::CheckResult(result, "VulkanLogicalDevice: failed to create presentation complete semaphore");

                result = vkCreateSemaphore(_device, &semaphoreCreateInfo, nullptr, &_renderCompleteSemaphores[i]);
                VulkanUtils::CheckResult(result, "VulkanLogicalDevice: failed to create rendering complete semaphore");

                _waitFences.emplace_back(_device, "signaled"_true);
            }
        }
        //--------------------------------------------------------------------------

        void VulkanLogicalDevice::_DeleteSyncronizationObjects()
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

            _waitFences.clear();

            for (UInt32 i = 0; i < NumConcurrentFrames; i++)
            {
                vkDestroySemaphore(_device, _presentCompleteSemaphores[i], nullptr);
                vkDestroySemaphore(_device, _renderCompleteSemaphores[i], nullptr);
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
            _swapchain.reset(new VulkanSwapchain(_device, GetPresentationQueue(), _surface, _vulkanContext, _currentExtent, _msaaSamples, *_imageCreatorDelegate.get(), _currentBufferIndex, _presentCompleteSemaphores, _renderCompleteSemaphores));
        }
        //--------------------------------------------------------------------------

        void VulkanLogicalDevice::_DeleteSwapchain()
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

            _swapchain.reset();
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

            _waitFences[_currentBufferIndex].Wait();
            _waitFences[_currentBufferIndex].Reset();
        }
        //--------------------------------------------------------------------------

        void VulkanLogicalDevice::_StartFrameTransitionColorAndDepthStencilImages()
        {
            KMP_PROFILE_FUNCTION(ProfileLevelMinorFunctions);

            VulkanUtils::MemoryBarrierParameters barrierParameters = {
                .cmdbuffer = _renderer->GetCurrentCommandBuffer(),
                .image = _swapchain->GetCurrentImage(),
                .srcAccessMask = VK_ACCESS_NONE,
                .dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
                .oldImageLayout = VK_IMAGE_LAYOUT_UNDEFINED,
                .newImageLayout = VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL,
                .srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
                .dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
                .subresourceRange = VulkanPresets::ImageSubresourceRange_Color_Layer1_Level1
            };
            VulkanUtils::InsertImageMemoryBarrier(barrierParameters);

            barrierParameters = {
                .cmdbuffer = _renderer->GetCurrentCommandBuffer(),
                .image = _swapchain->GetMultisampledDepthStencilImage(),
                .srcAccessMask = VK_ACCESS_NONE,
                .dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,
                .oldImageLayout = VK_IMAGE_LAYOUT_UNDEFINED,
                .newImageLayout = VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL,
                .srcStageMask = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT,
                .dstStageMask = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT,
                .subresourceRange = VulkanPresets::ImageSubresourceRange_DepthStencil_Layer1_Level1
            };
            VulkanUtils::InsertImageMemoryBarrier(barrierParameters);
        }
        //--------------------------------------------------------------------------

        void VulkanLogicalDevice::_StartFrameBeginRendering()
        {
            KMP_PROFILE_FUNCTION(ProfileLevelMinorFunctions);

            auto colorAttachmentInfo = VulkanUtils::InitVkRenderingAttachmentInfo();
            colorAttachmentInfo.imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
            colorAttachmentInfo.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
            colorAttachmentInfo.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
            colorAttachmentInfo.clearValue.color = { 0.0f, 0.0f, 0.0f, 0.0f };
            if (_swapchain->GetMultisampling() == VK_SAMPLE_COUNT_1_BIT)
            {
                colorAttachmentInfo.imageView = _swapchain->GetCurrentImageView();
            }
            else
            {
                colorAttachmentInfo.imageView = _swapchain->GetMultisampledColorImageView();
                colorAttachmentInfo.resolveMode = VK_RESOLVE_MODE_AVERAGE_BIT;
                colorAttachmentInfo.resolveImageView = _swapchain->GetCurrentImageView();
                colorAttachmentInfo.resolveImageLayout = VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL_KHR;
            }

            auto depthStencilAttachmentInfo = VulkanUtils::InitVkRenderingAttachmentInfo();
            depthStencilAttachmentInfo.imageView = _swapchain->GetMultisampledDepthStencilImageView();
            depthStencilAttachmentInfo.imageLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
            depthStencilAttachmentInfo.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
            depthStencilAttachmentInfo.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
            depthStencilAttachmentInfo.clearValue.depthStencil = { 1.0f, 0 };

            auto renderingInfo = VulkanUtils::InitVkRenderingInfo();
            renderingInfo.renderArea = { 0, 0, _currentExtent.width, _currentExtent.height };
            renderingInfo.layerCount = 1;
            renderingInfo.colorAttachmentCount = 1; //TODO: should match the number of color attachments of currently bound pipeline
            renderingInfo.pColorAttachments = &colorAttachmentInfo;
            renderingInfo.pDepthAttachment = &depthStencilAttachmentInfo;
            renderingInfo.pStencilAttachment = &depthStencilAttachmentInfo;

            vkCmdBeginRendering(_renderer->GetCurrentCommandBuffer(), &renderingInfo);
        }
        //--------------------------------------------------------------------------

        void VulkanLogicalDevice::_StartFrameSetupViewportAndScissor()
        {
            KMP_PROFILE_FUNCTION(ProfileLevelMinorFunctions);

            VkViewport viewport{ 0.0f, 0.0f, float(_currentExtent.width), float(_currentExtent.height) };
            vkCmdSetViewport(_renderer->GetCurrentCommandBuffer(), 0, 1, &viewport);

            VkRect2D scissor{ 0, 0, _currentExtent.width, _currentExtent.height };
            vkCmdSetScissor(_renderer->GetCurrentCommandBuffer(), 0, 1, &scissor);
        }
        //--------------------------------------------------------------------------

        void VulkanLogicalDevice::_StartFrameSetupRasterizationSamples()
        {
            KMP_PROFILE_FUNCTION(ProfileLevelMinorFunctions);

            VulkanCommands::CmdSetRasterizationSamplesEXT(_renderer->GetCurrentCommandBuffer(), GetMultisampling());
        }
        //--------------------------------------------------------------------------

        void VulkanLogicalDevice::_EndFrameEndRendering()
        {
            KMP_PROFILE_FUNCTION(ProfileLevelMinorFunctions);

            vkCmdEndRendering(_renderer->GetCurrentCommandBuffer());
        }
        //--------------------------------------------------------------------------

        void VulkanLogicalDevice::_EndFrameTransitionColorAndDepthStencilImages()
        {
            KMP_PROFILE_FUNCTION(ProfileLevelMinorFunctions);

            VulkanUtils::MemoryBarrierParameters barrierParameters = {
                .cmdbuffer = _renderer->GetCurrentCommandBuffer(),
                .image = _swapchain->GetCurrentImage(),
                .srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
                .dstAccessMask = VK_ACCESS_NONE,
                .oldImageLayout = VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL,
                .newImageLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
                .srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
                .dstStageMask = VK_PIPELINE_STAGE_2_NONE,
                .subresourceRange = VulkanPresets::ImageSubresourceRange_Color_Layer1_Level1
            };
            VulkanUtils::InsertImageMemoryBarrier(barrierParameters);
        }
        //--------------------------------------------------------------------------

        void VulkanLogicalDevice::_EndFrameQueueSubmit()
        {
            KMP_PROFILE_FUNCTION(ProfileLevelMinorFunctions);

            const auto commandBuffer = _renderer->GetCurrentCommandBuffer();

            VkPipelineStageFlags waitStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
            auto submitInfo = VulkanUtils::InitVkSubmitInfo();
            submitInfo.pWaitDstStageMask = &waitStageMask;
            submitInfo.pCommandBuffers = &commandBuffer;
            submitInfo.commandBufferCount = 1;
            submitInfo.pWaitSemaphores = &_presentCompleteSemaphores[_currentBufferIndex];
            submitInfo.waitSemaphoreCount = 1;
            submitInfo.pSignalSemaphores = &_renderCompleteSemaphores[_currentBufferIndex];
            submitInfo.signalSemaphoreCount = 1;

            _graphicsQueue->Submit({submitInfo}, _waitFences[_currentBufferIndex].GetVkFence());
        }
        //--------------------------------------------------------------------------

        VulkanGraphicsPipeline& VulkanLogicalDevice::AddGraphicsPipeline(StringID sid)
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

            if (_pipelines.contains(sid))
            {
                KMP_LOG_WARN("pipeline with sid '{}' has already been created", sid);
                return *_pipelines[sid].get();
            }

            const auto [iterator, hasEmplaced] = _pipelines.emplace(sid, CreateUPtr<VulkanGraphicsPipeline>(_device, sid));
            return *iterator->second.get();
        }
        //--------------------------------------------------------------------------

        OptionalRef<VulkanGraphicsPipeline> VulkanLogicalDevice::GetGraphicsPipeline(StringID sid) const
        {
            if (_pipelines.contains(sid))
            {
                return *_pipelines.at(sid).get();
            }
            
            KMP_LOG_ERROR("graphics pipeline with sid '{}' not found", sid);
            return std::nullopt;
        }
        //--------------------------------------------------------------------------

        bool VulkanLogicalDevice::AddDescriptorSetLayout(StringID sid, const Vector<VkDescriptorSetLayoutBinding>& bindings)
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

            if (_descriptorSetLayouts.contains(sid))
            {
                KMP_LOG_WARN("descriptor set layout with sid '{}' has already been created", sid);
                return true;
            }

            try
            {
                auto descriptorSetLayoutCreateInfo = Graphics::VulkanUtils::InitVkDescriptorSetLayoutCreateInfo();
                descriptorSetLayoutCreateInfo.bindingCount = UInt32(bindings.size());
                descriptorSetLayoutCreateInfo.pBindings = bindings.empty() ? nullptr : bindings.data();
                VkDescriptorSetLayout layout = nullptr;
                const auto result = vkCreateDescriptorSetLayout(_device, &descriptorSetLayoutCreateInfo, nullptr, &layout);
                VulkanUtils::CheckResult(result, "VulkanLogicalDevice: failed to create descriptor set layout");

                const auto [iterator, hasEmplaced] = _descriptorSetLayouts.emplace(sid, layout);
                return iterator->second != nullptr;
            }
            catch (KMP_MB_UNUSED const std::runtime_error& er)
            {
                KMP_LOG_ERROR("failed to create descriptor set layout '{}'", sid);
                return false;
            }
        }
        //--------------------------------------------------------------------------

        VkDescriptorSetLayout VulkanLogicalDevice::GetDescriptorSetLayout(StringID sid) const noexcept
        {
            if (_descriptorSetLayouts.contains(sid))
            {
                return _descriptorSetLayouts.at(sid);
            }

            KMP_LOG_ERROR("descriptor set layout with sid '{}' not found", sid);
            return VK_NULL_HANDLE;
        }
        //--------------------------------------------------------------------------

        Nullable<VulkanTexture*> VulkanLogicalDevice::CreateTexture(const Image& image) const
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

            try
            {
                auto commandBuffer = CreateCommandBuffer();
                commandBuffer.Begin(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);

                const auto textureVkFormat = ImageChannelsToVkFormat(ImageChannels(image.GetChannels()));
                const auto mipLevels = _formatDelegate.IsMipmapCompatible(textureVkFormat) ? image.GetMipLevels() : 1;
                auto imageBuffer = _imageCreatorDelegate->CreateStagingImageBuffer(image);
                const auto extent = VkExtent3D{
                    .width = UInt32(image.GetWidth()),
                    .height = UInt32(image.GetHeight()),
                    .depth = 1
                };

                auto* texture = new VulkanTexture(textureVkFormat, mipLevels, _device, commandBuffer.GetVkCommandBuffer(), imageBuffer, extent, *_imageCreatorDelegate.get());

                commandBuffer.End();

                auto fence = CreateFence("signaled"_false);
                _graphicsQueue->Submit(commandBuffer, fence.GetVkFence());
                fence.Wait();

                return texture;
            }
            catch (KMP_MB_UNUSED const std::runtime_error& e)
            {
                KMP_LOG_ERROR("failed to create a texture - {}", e.what());
            }

            return nullptr;
        }
        //--------------------------------------------------------------------------

        VulkanCommandBuffer VulkanLogicalDevice::CreateCommandBuffer() const
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

            return VulkanCommandBuffer(_device, _commandPool->GetVkCommandPool());
        }
        //--------------------------------------------------------------------------

        VulkanFence VulkanLogicalDevice::CreateFence(bool signaled /*= true*/) const
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

            return VulkanFence(_device, signaled);
        }
        //--------------------------------------------------------------------------

        VulkanShader VulkanLogicalDevice::CreateShader(const Filepath& filepath) const
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

            return VulkanShader(_device, filepath);
        }
        //--------------------------------------------------------------------------
    }
}