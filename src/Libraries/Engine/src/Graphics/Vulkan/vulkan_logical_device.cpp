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
            _CreateImageCreatorDelegate();
            _CreateSynchronizationObjects();
            _CreateSwapchain();
            _CreatePipelineCache();
            _CreateDescriptorPool();
            _CreateBufferCreatorDelegate();
            _CreateRenderer();
        }
        //--------------------------------------------------------------------------

        VulkanLogicalDevice::~VulkanLogicalDevice() KMP_PROFILING(ProfileLevelAlways)
        {
            _DeleteRenderer();
            _DeleteBufferCreatorDelegate();
            _DeleteDescriptorSetsLayouts();
            _DeletePipelines();
            _DeleteDescriptorPool();
            _DeletePipelineCache();
            _DeleteSwapchain();
            _DeleteSyncronizationObjects();
            _DeleteImageCreatorDelegate();
            _DeleteDeviceQueues();
            _DeleteLogicalDeviceObject();
        }}
        //--------------------------------------------------------------------------

        void VulkanLogicalDevice::StartFrame(float frameTimestep) KMP_PROFILING(ProfileLevelImportant)
        {
            _waitFences[_currentBufferIndex].Wait();
            _waitFences[_currentBufferIndex].Reset();

            _swapchain->StartFrame(frameTimestep);

            _renderer->StartFrame();
            _renderer->TransitionColorAndDepthStencilImagesToWrite(_swapchain->GetCurrentImage(), _swapchain->GetMultisampledDepthStencilImage());
        }}
        //--------------------------------------------------------------------------

        void VulkanLogicalDevice::EndFrame() KMP_PROFILING(ProfileLevelImportant)
        {
            _renderer->TransitionColorAndDepthStencilImagesToPresent(_swapchain->GetCurrentImage());
            _renderer->EndFrame();
            _renderer->SubmitToQueue(*_graphicsQueue.get(), { _presentCompleteSemaphores[_currentBufferIndex] }, { _renderCompleteSemaphores[_currentBufferIndex] }, _waitFences[_currentBufferIndex].GetVkFence());

            _swapchain->EndFrame();

            WaitIdle();
        }}
        //--------------------------------------------------------------------------

        void VulkanLogicalDevice::HandleWindowResize() KMP_PROFILING(ProfileLevelImportant)
        {
            WaitIdle();

            _DeleteSwapchain();
            _DeleteSyncronizationObjects();

            _CreateSynchronizationObjects();
            _CreateSwapchain();
            _renderer->SetSwapchain(*_swapchain.get());
        }}
        //--------------------------------------------------------------------------

        void VulkanLogicalDevice::WaitIdle() const KMP_PROFILING(ProfileLevelImportantVerbose)
        {
            if (_device != VK_NULL_HANDLE)
            {
                vkDeviceWaitIdle(_device);
            }
        }}
        //--------------------------------------------------------------------------

        VkSampleCountFlagBits VulkanLogicalDevice::GetMultisampling() const noexcept
        {
            return _msaaSamples;
        }
        //--------------------------------------------------------------------------

        void VulkanLogicalDevice::SetMultisampling(VkSampleCountFlagBits samples) KMP_PROFILING(ProfileLevelImportant)
        {
            _msaaSamples = samples;
            if (_msaaSamples > _vulkanContext.supportedSampleCounts.top())
            {
                KMP_LOG_WARN("cannot set MSAA samples to {}, set to maximum supported {}", UInt32(samples), UInt32(_vulkanContext.supportedSampleCounts.top()));
                _msaaSamples = _vulkanContext.supportedSampleCounts.top();
            }

            _swapchain->SetMultisampling(_msaaSamples);
        }}
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

        const VkExtent2D& VulkanLogicalDevice::GetCurrentExtent() const noexcept
        {
            return _currentExtent;
        }
        //--------------------------------------------------------------------------

        void VulkanLogicalDevice::_CreateLogicalDeviceObject() KMP_PROFILING(ProfileLevelImportant)
        {
            const auto queueCreateInfos = _CreateQueueCreateInfos();

            auto vertexInputDynamicStateFeatures = VulkanUtils::InitVkPhysicalDeviceVertexInputDynamicStateFeaturesEXT();
            vertexInputDynamicStateFeatures.vertexInputDynamicState = VK_TRUE;

            auto dynamicStateFeatures2 = VulkanUtils::InitVkPhysicalDeviceExtendedDynamicState2FeaturesEXT();
            dynamicStateFeatures2.extendedDynamicState2LogicOp = VK_TRUE;
            dynamicStateFeatures2.pNext = &vertexInputDynamicStateFeatures;

            auto colorWriteEnableFeatures = VulkanUtils::InitVkPhysicalDeviceColorWriteEnableFeaturesEXT();
            colorWriteEnableFeatures.colorWriteEnable = VK_TRUE;
            colorWriteEnableFeatures.pNext = &dynamicStateFeatures2;

            auto depthClipEnableFeatures = VulkanUtils::InitVkPhysicalDeviceDepthClipEnableFeaturesEXT();
            depthClipEnableFeatures.depthClipEnable = VK_TRUE;
            depthClipEnableFeatures.pNext = &colorWriteEnableFeatures;

            auto dynamicStateFeatures3 = VulkanUtils::InitVkPhysicalDeviceExtendedDynamicState3FeaturesEXT();
            dynamicStateFeatures3.extendedDynamicState3ColorBlendEnable = VK_TRUE;
            dynamicStateFeatures3.extendedDynamicState3RasterizationSamples = VK_TRUE;
            dynamicStateFeatures3.extendedDynamicState3DepthClampEnable = VK_TRUE;
            dynamicStateFeatures3.extendedDynamicState3DepthClipEnable = VK_TRUE;
            dynamicStateFeatures3.extendedDynamicState3LineStippleEnable = VK_TRUE;
            dynamicStateFeatures3.extendedDynamicState3LineRasterizationMode = VK_TRUE;
            dynamicStateFeatures3.extendedDynamicState3SampleLocationsEnable = VK_TRUE;
            dynamicStateFeatures3.extendedDynamicState3SampleMask = VK_TRUE;
            dynamicStateFeatures3.extendedDynamicState3PolygonMode = VK_TRUE;
            dynamicStateFeatures3.extendedDynamicState3ColorBlendEquation = VK_TRUE;
            dynamicStateFeatures3.extendedDynamicState3ColorWriteMask = VK_TRUE;
            dynamicStateFeatures3.extendedDynamicState3AlphaToCoverageEnable = VK_TRUE;
            dynamicStateFeatures3.extendedDynamicState3AlphaToOneEnable = VK_TRUE;
            dynamicStateFeatures3.extendedDynamicState3LogicOpEnable = VK_TRUE;
            dynamicStateFeatures3.extendedDynamicState3ProvokingVertexMode = VK_TRUE;
            dynamicStateFeatures3.pNext = &depthClipEnableFeatures;

            VkPhysicalDeviceFeatures features{};
            features.samplerAnisotropy = VK_TRUE;
            features.independentBlend = VK_TRUE;
            features.depthClamp = VK_TRUE;
            features.depthBounds = VK_TRUE;
            features.fillModeNonSolid = VK_TRUE;
            features.alphaToOne = VK_TRUE;
            features.logicOp = VK_TRUE;

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
        }}
        //--------------------------------------------------------------------------

        void VulkanLogicalDevice::_DeleteLogicalDeviceObject() KMP_PROFILING(ProfileLevelImportant)
        {
            vkDestroyDevice(_device, nullptr);
        }}
        //--------------------------------------------------------------------------

        void VulkanLogicalDevice::_CreateDeviceQueues() KMP_PROFILING(ProfileLevelImportant)
        {
            const auto graphicsQueueSupportPresentation = (_vulkanContext.graphicsFamilyIndex == _vulkanContext.presentFamilyIndex);
            _graphicsQueue.reset(new VulkanQueue(_device, _vulkanContext.graphicsFamilyIndex, graphicsQueueSupportPresentation));
            _presentQueue.reset(new VulkanQueue(_device, _vulkanContext.presentFamilyIndex, "support presentation"_true));
        }}
        //--------------------------------------------------------------------------

        void VulkanLogicalDevice::_DeleteDeviceQueues() KMP_PROFILING(ProfileLevelImportant)
        {
            _graphicsQueue.reset();
            _presentQueue.reset();
        }}
        //--------------------------------------------------------------------------

        void VulkanLogicalDevice::_CreateImageCreatorDelegate() KMP_PROFILING(ProfileLevelImportant)
        {
            _imageCreatorDelegate.reset(new VulkanImageCreatorDelegate(_device, _memoryTypeDelegate));
        }}
        //--------------------------------------------------------------------------

        void VulkanLogicalDevice::_DeleteImageCreatorDelegate() KMP_PROFILING(ProfileLevelImportant)
        {
            _imageCreatorDelegate.reset();
        }}
        //--------------------------------------------------------------------------

        void VulkanLogicalDevice::_CreateSynchronizationObjects() KMP_PROFILING(ProfileLevelImportant)
        {
            auto semaphoreCreateInfo = VulkanUtils::InitVkSemaphoreCreateInfo();

            for (UInt32 i = 0; i < NumConcurrentFrames; i++)
            {
                auto result = vkCreateSemaphore(_device, &semaphoreCreateInfo, nullptr, &_presentCompleteSemaphores[i]);
                VulkanUtils::CheckResult(result, "VulkanLogicalDevice: failed to create presentation complete semaphore");

                result = vkCreateSemaphore(_device, &semaphoreCreateInfo, nullptr, &_renderCompleteSemaphores[i]);
                VulkanUtils::CheckResult(result, "VulkanLogicalDevice: failed to create rendering complete semaphore");

                _waitFences.emplace_back(_device, "signaled"_true);
            }
        }}
        //--------------------------------------------------------------------------

        void VulkanLogicalDevice::_DeleteSyncronizationObjects() KMP_PROFILING(ProfileLevelImportant)
        {
            _waitFences.clear();

            for (UInt32 i = 0; i < NumConcurrentFrames; i++)
            {
                vkDestroySemaphore(_device, _presentCompleteSemaphores[i], nullptr);
                vkDestroySemaphore(_device, _renderCompleteSemaphores[i], nullptr);
            }
        }}
        //--------------------------------------------------------------------------

        void VulkanLogicalDevice::_CreateSwapchain() KMP_PROFILING(ProfileLevelImportant)
        {
            _currentExtent = _UpdateExtent();
            _swapchain.reset(new VulkanSwapchain(_device, GetPresentationQueue(), _vulkanContext, _currentExtent, _msaaSamples, *_imageCreatorDelegate.get(), _currentBufferIndex, _presentCompleteSemaphores, _renderCompleteSemaphores));
        }}
        //--------------------------------------------------------------------------

        void VulkanLogicalDevice::_DeleteSwapchain() KMP_PROFILING(ProfileLevelImportant)
        {
            _swapchain.reset();
        }}
        //--------------------------------------------------------------------------

        void VulkanLogicalDevice::_CreatePipelineCache() KMP_PROFILING(ProfileLevelImportant)
        {
            auto pipelineCacheCreateInfo = VulkanUtils::InitVkPipelineCacheCreateInfo();

            const auto result = vkCreatePipelineCache(_device, &pipelineCacheCreateInfo, nullptr, &_pipelineCache);
            VulkanUtils::CheckResult(result, "VulkanLogicalDevice: failed to create pipeline cache");
        }}
        //--------------------------------------------------------------------------

        void VulkanLogicalDevice::_DeletePipelineCache() KMP_PROFILING(ProfileLevelImportant)
        {
            vkDestroyPipelineCache(_device, _pipelineCache, nullptr);
        }}
        //--------------------------------------------------------------------------

        void VulkanLogicalDevice::_CreateDescriptorPool() KMP_PROFILING(ProfileLevelImportant)
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
        }}
        //--------------------------------------------------------------------------

        void VulkanLogicalDevice::_DeleteDescriptorPool() KMP_PROFILING(ProfileLevelImportant)
        {
            vkDestroyDescriptorPool(_device, _descriptorPool, nullptr);
        }}
        //--------------------------------------------------------------------------

        void VulkanLogicalDevice::_DeleteDescriptorSetsLayouts() KMP_PROFILING(ProfileLevelImportant)
        {
            for (const auto& [sid, descriptorSetLayout] : _descriptorSetLayouts)
            {
                vkDestroyDescriptorSetLayout(_device, descriptorSetLayout, nullptr);
            }
            _descriptorSetLayouts.clear();
        }}
        //--------------------------------------------------------------------------

        void VulkanLogicalDevice::_DeletePipelines() KMP_PROFILING(ProfileLevelImportant)
        {
            _pipelines.clear();
        }}
        //--------------------------------------------------------------------------

        void VulkanLogicalDevice::_CreateBufferCreatorDelegate() KMP_PROFILING(ProfileLevelImportant)
        {
            _bufferCreatorDelegate.reset(new VulkanBufferCreatorDelegate(_device, _descriptorPool, _memoryTypeDelegate));
        }}
        //--------------------------------------------------------------------------

        void VulkanLogicalDevice::_DeleteBufferCreatorDelegate() KMP_PROFILING(ProfileLevelImportant)
        {
            _bufferCreatorDelegate.reset();
        }}
        //--------------------------------------------------------------------------

        void VulkanLogicalDevice::_CreateRenderer() KMP_PROFILING(ProfileLevelImportant)
        {
            _renderer.reset(new VulkanRenderer(_device, _currentBufferIndex, _pipelines, _vulkanContext.graphicsFamilyIndex, *_swapchain.get()));
        }}
        //--------------------------------------------------------------------------

        void VulkanLogicalDevice::_DeleteRenderer() KMP_PROFILING(ProfileLevelImportant)
        {
            _renderer.reset();
        }}
        //--------------------------------------------------------------------------

        Vector<VkDeviceQueueCreateInfo> VulkanLogicalDevice::_CreateQueueCreateInfos() const KMP_PROFILING(ProfileLevelImportant)
        {
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
        }}
        //--------------------------------------------------------------------------

        VkExtent2D VulkanLogicalDevice::_UpdateExtent() const KMP_PROFILING(ProfileLevelImportantVerbose)
        {
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
        }}
        //--------------------------------------------------------------------------

        VulkanGraphicsPipeline& VulkanLogicalDevice::AddGraphicsPipeline(StringID sid) KMP_PROFILING(ProfileLevelImportant)
        {
            if (_pipelines.contains(sid))
            {
                KMP_LOG_WARN("pipeline with sid '{}' has already been created", sid);
                return *_pipelines[sid].get();
            }

            const auto [iterator, hasEmplaced] = _pipelines.emplace(sid, CreateUPtr<VulkanGraphicsPipeline>(_device, sid));
            return *iterator->second.get();
        }}
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

        bool VulkanLogicalDevice::AddDescriptorSetLayout(StringID sid, const Vector<VkDescriptorSetLayoutBinding>& bindings) KMP_PROFILING(ProfileLevelImportant)
        {
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
        }}
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

        Nullable<VulkanTexture*> VulkanLogicalDevice::CreateTexture(const Image& image) const KMP_PROFILING(ProfileLevelImportant)
        {
            try
            {
                const auto textureVkFormat = ImageChannelsToVkFormat(ImageChannels(image.GetChannels()));
                const auto mipLevels = _formatDelegate.IsMipmapCompatible(textureVkFormat) ? image.GetMipLevels() : 1;
                auto imageBuffer = _imageCreatorDelegate->CreateStagingImageBuffer(image);
                const auto extent = VkExtent3D{
                    .width = UInt32(image.GetWidth()),
                    .height = UInt32(image.GetHeight()),
                    .depth = 1
                };

                auto commandBuffer = _renderer->CreateCommandBuffer();
                commandBuffer.Begin(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
                auto* texture = new VulkanTexture(textureVkFormat, mipLevels, _device, commandBuffer.GetVkCommandBuffer(), imageBuffer, extent, *_imageCreatorDelegate.get());
                commandBuffer.End();

                _graphicsQueue->SyncSubmit(commandBuffer);

                return texture;
            }
            catch (KMP_MB_UNUSED const std::runtime_error& e)
            {
                KMP_LOG_ERROR("failed to create a texture - {}", e.what());
            }

            return nullptr;
        }}
        //--------------------------------------------------------------------------

        VulkanFence VulkanLogicalDevice::CreateFence(bool signaled /*= true*/) const KMP_PROFILING(ProfileLevelImportant)
        {
            return VulkanFence(_device, signaled);
        }}
        //--------------------------------------------------------------------------

        VulkanShader VulkanLogicalDevice::CreateShader(const Filepath& filepath) const KMP_PROFILING(ProfileLevelImportant)
        {
            return VulkanShader(_device, filepath);
        }}
        //--------------------------------------------------------------------------
    }
}