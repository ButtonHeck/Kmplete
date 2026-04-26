#include "Kmplete/Graphics/Vulkan/vulkan_graphics_base.h"
#include "Kmplete/Graphics/Vulkan/vulkan_logical_device.h"
#include "Kmplete/Graphics/Vulkan/vulkan_physical_device.h"
#include "Kmplete/Graphics/Vulkan/vulkan_texture.h"
#include "Kmplete/Graphics/Vulkan/Utils/initializers.h"
#include "Kmplete/Graphics/Vulkan/Utils/result_description.h"
#include "Kmplete/Graphics/Vulkan/Utils/function_utils.h"
#include "Kmplete/Graphics/Vulkan/Utils/extension_functions.h"
#include "Kmplete/Graphics/Vulkan/Utils/presets.h"
#include "Kmplete/Graphics/Vulkan/Utils/bits_aliases.h"
#include "Kmplete/Graphics/image.h"
#include "Kmplete/Base/named_bool.h"
#include "Kmplete/Base/exception.h"
#include "Kmplete/Core/assertion.h"
#include "Kmplete/Window/window.h"
#include "Kmplete/Math/math.h"
#include "Kmplete/Math/geometry.h"
#include "Kmplete/Profile/profiler.h"
#include "Kmplete/Log/log.h"

#include <limits>


namespace Kmplete
{
    namespace Graphics
    {
        using namespace VKBits;


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
            , _pipelines()
            , _bufferCreatorDelegate(nullptr)
            , _currentExtent(_UpdateExtent())
            , _msaaSamples(VK_SampleCount_1)
            , _renderer(nullptr)
            , _shaderObjects()
            , _samplersStorage(nullptr)
            , _descriptorSetManager(nullptr)
        {
            KMP_PROFILE_FUNCTION(ProfileLevelAlways);

            _CreateLogicalDeviceObject();
            _CreateDeviceQueues();
            _CreateImageCreatorDelegate();
            _CreateSynchronizationObjects();
            _CreateSwapchain();
            _CreatePipelineCache();
            _CreateBufferCreatorDelegate();
            _CreateRenderer();
            _CreateSamplersStorage();
            _CreateDescriptorSetManager();
        }
        //--------------------------------------------------------------------------

        VulkanLogicalDevice::~VulkanLogicalDevice() KMP_PROFILING(ProfileLevelAlways)
        {
            _DeleteDescriptorSetManager();
            _DeleteSamplersStorage();
            _DeleteShaderObjects();
            _DeleteRenderer();
            _DeleteBufferCreatorDelegate();
            _DeletePipelines();
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
            KMP_ASSERT(_swapchain && _renderer);
            KMP_ASSERT(_currentBufferIndex < _waitFences.size());

            _waitFences[_currentBufferIndex].Wait();
            _waitFences[_currentBufferIndex].Reset();

            _swapchain->StartFrame(frameTimestep);

            _renderer->StartFrame();
            _renderer->TransitionColorAndDepthStencilImagesToWrite(_swapchain->GetCurrentImage(), _swapchain->GetMultisampledDepthStencilImage());
        }}
        //--------------------------------------------------------------------------

        void VulkanLogicalDevice::EndFrame() KMP_PROFILING(ProfileLevelImportant)
        {
            KMP_ASSERT(_swapchain && _renderer && _graphicsQueue);
            KMP_ASSERT(_currentBufferIndex < _waitFences.size());
            KMP_ASSERT(_currentBufferIndex < _presentCompleteSemaphores.size());
            KMP_ASSERT(_currentBufferIndex < _renderCompleteSemaphores.size());

            _renderer->TransitionColorAndDepthStencilImagesToPresent(_swapchain->GetCurrentImage());
            _renderer->EndFrame();
            _renderer->SubmitToQueue(*_graphicsQueue.get(), { _presentCompleteSemaphores[_currentBufferIndex] }, { _renderCompleteSemaphores[_currentBufferIndex] }, _waitFences[_currentBufferIndex].GetVkFence());

            _swapchain->EndFrame();

            WaitIdle();
        }}
        //--------------------------------------------------------------------------

        void VulkanLogicalDevice::HandleWindowResize() KMP_PROFILING(ProfileLevelImportant)
        {
            KMP_ASSERT(_swapchain && _renderer);

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
            KMP_ASSERT(_swapchain);

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
            KMP_ASSERT(_swapchain);

            return *_swapchain.get();
        }
        //--------------------------------------------------------------------------

        VkDevice VulkanLogicalDevice::GetVkDevice() const noexcept
        {
            KMP_ASSERT(_device);

            return _device;
        }
        //--------------------------------------------------------------------------

        const VulkanQueue& VulkanLogicalDevice::GetGraphicsQueue() const noexcept
        {
            KMP_ASSERT(_graphicsQueue);

            return *_graphicsQueue.get();
        }
        //--------------------------------------------------------------------------

        const VulkanQueue& VulkanLogicalDevice::GetPresentationQueue() const noexcept
        {
            KMP_ASSERT(_presentQueue);

            return *_presentQueue.get();
        }
        //--------------------------------------------------------------------------

        const VulkanImageCreatorDelegate& VulkanLogicalDevice::GetVulkanImageCreatorDelegate() const noexcept
        {
            KMP_ASSERT(_imageCreatorDelegate);

            return *_imageCreatorDelegate.get();
        }
        //--------------------------------------------------------------------------

        const VulkanBufferCreatorDelegate& VulkanLogicalDevice::GetVulkanBufferCreatorDelegate() const noexcept
        {
            KMP_ASSERT(_bufferCreatorDelegate);

            return *_bufferCreatorDelegate.get();
        }
        //--------------------------------------------------------------------------

        const VulkanRenderer& VulkanLogicalDevice::GetRenderer() const noexcept
        {
            KMP_ASSERT(_renderer);

            return *_renderer.get();
        }
        //--------------------------------------------------------------------------

        const VkExtent2D& VulkanLogicalDevice::GetCurrentExtent() const noexcept
        {
            return _currentExtent;
        }
        //--------------------------------------------------------------------------

        const VulkanSamplersStorage& VulkanLogicalDevice::GetSamplersStorage() const noexcept
        {
            KMP_ASSERT(_samplersStorage);

            return *_samplersStorage.get();
        }
        //--------------------------------------------------------------------------

        const VulkanDescriptorSetManager& VulkanLogicalDevice::GetDescriptorSetManager() const noexcept
        {
            KMP_ASSERT(_descriptorSetManager);

            return *_descriptorSetManager.get();
        }
        //--------------------------------------------------------------------------

        VulkanDescriptorSetManager& VulkanLogicalDevice::GetDescriptorSetManager() noexcept
        {
            KMP_ASSERT(_descriptorSetManager);

            return *_descriptorSetManager.get();
        }
        //--------------------------------------------------------------------------

        void VulkanLogicalDevice::_CreateLogicalDeviceObject() KMP_PROFILING(ProfileLevelImportant)
        {
            const auto queueCreateInfos = _CreateQueueCreateInfos();

            auto vertexAttributeDivisorFeatures = VKUtils::InitVkPhysicalDeviceVertexAttributeDivisorFeatures();
            vertexAttributeDivisorFeatures.vertexAttributeInstanceRateDivisor = VK_TRUE;
            vertexAttributeDivisorFeatures.vertexAttributeInstanceRateZeroDivisor = VK_TRUE;

            auto lineRasterizationFeatures = VKUtils::InitVkPhysicalDeviceLineRasterizationFeaturesEXT();
            lineRasterizationFeatures.bresenhamLines = VK_TRUE;
            lineRasterizationFeatures.rectangularLines = VK_TRUE;
            lineRasterizationFeatures.smoothLines = VK_TRUE;
            lineRasterizationFeatures.stippledBresenhamLines = VK_TRUE;
            lineRasterizationFeatures.stippledRectangularLines = VK_TRUE;
            lineRasterizationFeatures.stippledSmoothLines = VK_TRUE;
            lineRasterizationFeatures.pNext = &vertexAttributeDivisorFeatures;

            auto shaderObjectFeatures = VKUtils::InitVkPhysicalDeviceShaderObjectFeaturesEXT();
            shaderObjectFeatures.shaderObject = VK_TRUE;
            shaderObjectFeatures.pNext = &lineRasterizationFeatures;

            auto vertexInputDynamicStateFeatures = VKUtils::InitVkPhysicalDeviceVertexInputDynamicStateFeaturesEXT();
            vertexInputDynamicStateFeatures.vertexInputDynamicState = VK_TRUE;
            vertexInputDynamicStateFeatures.pNext = &shaderObjectFeatures;

            auto dynamicStateFeatures2 = VKUtils::InitVkPhysicalDeviceExtendedDynamicState2FeaturesEXT();
            dynamicStateFeatures2.extendedDynamicState2LogicOp = VK_TRUE;
            dynamicStateFeatures2.pNext = &vertexInputDynamicStateFeatures;

            auto colorWriteEnableFeatures = VKUtils::InitVkPhysicalDeviceColorWriteEnableFeaturesEXT();
            colorWriteEnableFeatures.colorWriteEnable = VK_TRUE;
            colorWriteEnableFeatures.pNext = &dynamicStateFeatures2;

            auto depthClipEnableFeatures = VKUtils::InitVkPhysicalDeviceDepthClipEnableFeaturesEXT();
            depthClipEnableFeatures.depthClipEnable = VK_TRUE;
            depthClipEnableFeatures.pNext = &colorWriteEnableFeatures;

            auto dynamicStateFeatures3 = VKUtils::InitVkPhysicalDeviceExtendedDynamicState3FeaturesEXT();
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
            features.wideLines = VK_TRUE;

            auto features13 = VKUtils::InitVkPhysicalDeviceVulkan13Features();
            features13.dynamicRendering = VK_TRUE;
            features13.synchronization2 = VK_TRUE;
            features13.pNext = &dynamicStateFeatures3;

            auto features2 = VKUtils::InitVkPhysicalDeviceFeatures2();
            features2.features = features;
            features2.pNext = &features13;

            const auto& enabledDeviceExtensions = VulkanPhysicalDevice::GetEnabledDeviceExtensions();

            auto deviceCreateInfo = VKUtils::InitVkDeviceCreateInfo();
            deviceCreateInfo.queueCreateInfoCount = UInt32(queueCreateInfos.size());
            deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();
            deviceCreateInfo.pEnabledFeatures = nullptr;
            deviceCreateInfo.enabledExtensionCount = UInt32(enabledDeviceExtensions.size());
            deviceCreateInfo.ppEnabledExtensionNames = enabledDeviceExtensions.data();
            deviceCreateInfo.pNext = &features2;

            const auto result = vkCreateDevice(_physicalDevice, &deviceCreateInfo, nullptr, &_device);
            VKUtils::CheckResult(result, "VulkanLogicalDevice: failed to create logical device");
            KMP_ASSERT(_device);
        }}
        //--------------------------------------------------------------------------

        void VulkanLogicalDevice::_DeleteLogicalDeviceObject() KMP_PROFILING(ProfileLevelImportant)
        {
            KMP_ASSERT(_device);

            vkDestroyDevice(_device, nullptr);
        }}
        //--------------------------------------------------------------------------

        void VulkanLogicalDevice::_CreateDeviceQueues() KMP_PROFILING(ProfileLevelImportant)
        {
            KMP_ASSERT(_device);

            const auto graphicsQueueSupportPresentation = (_vulkanContext.graphicsFamilyIndex == _vulkanContext.presentFamilyIndex);
            _graphicsQueue.reset(new VulkanQueue(_device, _vulkanContext.graphicsFamilyIndex, graphicsQueueSupportPresentation));
            _presentQueue.reset(new VulkanQueue(_device, _vulkanContext.presentFamilyIndex, "support presentation"_true));
            KMP_ASSERT(_graphicsQueue && _presentQueue);
        }}
        //--------------------------------------------------------------------------

        void VulkanLogicalDevice::_DeleteDeviceQueues() KMP_PROFILING(ProfileLevelImportant)
        {
            KMP_ASSERT(_graphicsQueue && _presentQueue);

            _graphicsQueue.reset();
            _presentQueue.reset();
        }}
        //--------------------------------------------------------------------------

        void VulkanLogicalDevice::_CreateImageCreatorDelegate() KMP_PROFILING(ProfileLevelImportant)
        {
            KMP_ASSERT(_device);

            _imageCreatorDelegate.reset(new VulkanImageCreatorDelegate(_device, _memoryTypeDelegate));
            KMP_ASSERT(_imageCreatorDelegate);
        }}
        //--------------------------------------------------------------------------

        void VulkanLogicalDevice::_DeleteImageCreatorDelegate() KMP_PROFILING(ProfileLevelImportant)
        {
            KMP_ASSERT(_imageCreatorDelegate);
            _imageCreatorDelegate.reset();
        }}
        //--------------------------------------------------------------------------

        void VulkanLogicalDevice::_CreateSynchronizationObjects() KMP_PROFILING(ProfileLevelImportant)
        {
            KMP_ASSERT(_device);

            auto semaphoreCreateInfo = VKUtils::InitVkSemaphoreCreateInfo();

            for (UInt32 i = 0; i < NumConcurrentFrames; i++)
            {
                auto result = vkCreateSemaphore(_device, &semaphoreCreateInfo, nullptr, &_presentCompleteSemaphores[i]);
                VKUtils::CheckResult(result, "VulkanLogicalDevice: failed to create presentation complete semaphore");

                result = vkCreateSemaphore(_device, &semaphoreCreateInfo, nullptr, &_renderCompleteSemaphores[i]);
                VKUtils::CheckResult(result, "VulkanLogicalDevice: failed to create rendering complete semaphore");

                _waitFences.emplace_back(_device, "signaled"_true);
            }
        }}
        //--------------------------------------------------------------------------

        void VulkanLogicalDevice::_DeleteSyncronizationObjects() KMP_PROFILING(ProfileLevelImportant)
        {
            KMP_ASSERT(_device);

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
            KMP_ASSERT(_device && _imageCreatorDelegate);

            _currentExtent = _UpdateExtent();
            _swapchain.reset(new VulkanSwapchain(_device, GetPresentationQueue(), _vulkanContext, _currentExtent, _msaaSamples, *_imageCreatorDelegate.get(), _currentBufferIndex, _presentCompleteSemaphores, _renderCompleteSemaphores));
            KMP_ASSERT(_swapchain);
        }}
        //--------------------------------------------------------------------------

        void VulkanLogicalDevice::_DeleteSwapchain() KMP_PROFILING(ProfileLevelImportant)
        {
            KMP_ASSERT(_swapchain);

            _swapchain.reset();
        }}
        //--------------------------------------------------------------------------

        void VulkanLogicalDevice::_CreatePipelineCache() KMP_PROFILING(ProfileLevelImportant)
        {
            KMP_ASSERT(_device);

            auto pipelineCacheCreateInfo = VKUtils::InitVkPipelineCacheCreateInfo();

            const auto result = vkCreatePipelineCache(_device, &pipelineCacheCreateInfo, nullptr, &_pipelineCache);
            VKUtils::CheckResult(result, "VulkanLogicalDevice: failed to create pipeline cache");
            KMP_ASSERT(_pipelineCache);
        }}
        //--------------------------------------------------------------------------

        void VulkanLogicalDevice::_DeletePipelineCache() KMP_PROFILING(ProfileLevelImportant)
        {
            KMP_ASSERT(_device && _pipelineCache);

            vkDestroyPipelineCache(_device, _pipelineCache, nullptr);
        }}
        //--------------------------------------------------------------------------

        void VulkanLogicalDevice::_DeletePipelines() KMP_PROFILING(ProfileLevelImportant)
        {
            _pipelines.clear();
        }}
        //--------------------------------------------------------------------------

        void VulkanLogicalDevice::_DeleteShaderObjects() KMP_PROFILING(ProfileLevelImportant)
        {
            _shaderObjects.clear();
        }}
        //--------------------------------------------------------------------------

        void VulkanLogicalDevice::_CreateBufferCreatorDelegate() KMP_PROFILING(ProfileLevelImportant)
        {
            KMP_ASSERT(_device);

            _bufferCreatorDelegate.reset(new VulkanBufferCreatorDelegate(_device, _memoryTypeDelegate));
            KMP_ASSERT(_bufferCreatorDelegate);
        }}
        //--------------------------------------------------------------------------

        void VulkanLogicalDevice::_DeleteBufferCreatorDelegate() KMP_PROFILING(ProfileLevelImportant)
        {
            KMP_ASSERT(_bufferCreatorDelegate);

            _bufferCreatorDelegate.reset();
        }}
        //--------------------------------------------------------------------------

        void VulkanLogicalDevice::_CreateRenderer() KMP_PROFILING(ProfileLevelImportant)
        {
            KMP_ASSERT(_device && _swapchain);

            _renderer.reset(new VulkanRenderer(_device, _currentBufferIndex, _pipelines, _shaderObjects, _vulkanContext.graphicsFamilyIndex, *_swapchain.get()));
            KMP_ASSERT(_renderer);
        }}
        //--------------------------------------------------------------------------

        void VulkanLogicalDevice::_DeleteRenderer() KMP_PROFILING(ProfileLevelImportant)
        {
            KMP_ASSERT(_renderer);

            _renderer.reset();
        }}
        //--------------------------------------------------------------------------

        void VulkanLogicalDevice::_CreateSamplersStorage() KMP_PROFILING(ProfileLevelImportant)
        {
            KMP_ASSERT(_device && _imageCreatorDelegate);

            _samplersStorage.reset(new VulkanSamplersStorage(_device, *_imageCreatorDelegate.get()));
            KMP_ASSERT(_samplersStorage);

            auto sampler = _samplersStorage->AddSampler(SamplerDefaultNearestSid, VKPresets::SamplerCreateInfo_Nearest_MipNearest_Repeat_NoAnisotropy);
            if (sampler == VK_NULL_HANDLE)
            {
                KMP_LOG_CRITICAL("failed to create default nearest filtering sampler");
                throw RuntimeError("failed to create default nearest filtering sampler");
            }

            sampler = _samplersStorage->AddSampler(SamplerDefaultLinearSid, VKPresets::SamplerCreateInfo_Linear_MipLinear_Repeat_NoAnisotropy);
            if (sampler == VK_NULL_HANDLE)
            {
                KMP_LOG_CRITICAL("failed to create default linear filtering sampler");
                throw RuntimeError("failed to create default linear filtering sampler");
            }
        }}
        //--------------------------------------------------------------------------

        void VulkanLogicalDevice::_DeleteSamplersStorage() KMP_PROFILING(ProfileLevelImportant)
        {
            KMP_ASSERT(_samplersStorage);

            _samplersStorage.reset();
        }}
        //--------------------------------------------------------------------------

        void VulkanLogicalDevice::_CreateDescriptorSetManager() KMP_PROFILING(ProfileLevelImportant)
        {
            KMP_ASSERT(_device);

            _descriptorSetManager.reset(new VulkanDescriptorSetManager(_device, _currentBufferIndex));
            KMP_ASSERT(_descriptorSetManager);
        }}
        //--------------------------------------------------------------------------

        void VulkanLogicalDevice::_DeleteDescriptorSetManager() KMP_PROFILING(ProfileLevelImportant)
        {
            KMP_ASSERT(_descriptorSetManager);

            _descriptorSetManager.reset();
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
                auto queueCreateInfo = VKUtils::InitVkDeviceQueueCreateInfo();
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

        VulkanGraphicsPipeline& VulkanLogicalDevice::AddGraphicsPipeline(StringID sid, const VulkanGraphicsPipelineParameters& parameters) KMP_PROFILING(ProfileLevelImportant)
        {
            KMP_ASSERT(_device);

            if (_pipelines.contains(sid))
            {
                KMP_LOG_WARN("pipeline with sid '{}' has already been created", sid);
                return *_pipelines[sid].get();
            }

            const auto [iterator, hasEmplaced] = _pipelines.emplace(sid, CreateUPtr<VulkanGraphicsPipeline>(_device, sid, parameters));
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

        Nullable<VulkanTexture*> VulkanLogicalDevice::CreateTexture(const Image& image) const KMP_PROFILING(ProfileLevelImportant)
        {
            KMP_ASSERT(_device && _imageCreatorDelegate && _graphicsQueue);

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
                commandBuffer.Begin(VK_CommandBufferUsage_OneTimeSubmit);
                auto* texture = new VulkanTexture(textureVkFormat, mipLevels, _device, commandBuffer.GetVkCommandBuffer(), imageBuffer, extent, *_imageCreatorDelegate.get());
                commandBuffer.End();

                _graphicsQueue->SyncSubmit(commandBuffer);

                return texture;
            }
            catch (KMP_MB_UNUSED const RuntimeError& e)
            {
                KMP_LOG_ERROR("failed to create a texture - {}", e.what());
            }

            return nullptr;
        }}
        //--------------------------------------------------------------------------

        VulkanFence VulkanLogicalDevice::CreateFence(bool signaled /*= true*/) const KMP_PROFILING(ProfileLevelImportant)
        {
            KMP_ASSERT(_device);

            return VulkanFence(_device, signaled);
        }}
        //--------------------------------------------------------------------------

        VulkanShaderModule VulkanLogicalDevice::CreateShaderModule(const Filepath& filepath) const KMP_PROFILING(ProfileLevelImportant)
        {
            KMP_ASSERT(_device);

            return VulkanShaderModule(_device, filepath);
        }}
        //--------------------------------------------------------------------------

        bool VulkanLogicalDevice::AddShaderObject(StringID sid, const Filepath& filepath, VkShaderStageFlagBits stage, VkShaderStageFlags nextStage, bool linked, 
                                                  const Vector<VkDescriptorSetLayout>& descriptorSetsLayouts, const char* name) KMP_PROFILING(ProfileLevelImportant)
        {
            KMP_ASSERT(_device);

            if (_shaderObjects.contains(sid))
            {
                KMP_LOG_WARN("shader object with sid '{}' has already been created", sid);
                return true;
            }

            try
            {
                const auto [iterator, hasEmplaced] = _shaderObjects.emplace(sid, CreateUPtr<VulkanShaderObject>(_device, filepath, stage, nextStage, linked, descriptorSetsLayouts, name));
                return hasEmplaced;
            }
            catch (KMP_MB_UNUSED const RuntimeError& er)
            {
                KMP_LOG_ERROR("failed to create shader object with sid '{}' from '{}'", sid, filepath);
                return false;
            }
        }}
        //--------------------------------------------------------------------------

        bool VulkanLogicalDevice::AddShaderObject(StringID sid, const Filepath& filepath, VkShaderStageFlagBits stage, VkShaderStageFlags nextStage, bool linked,
                                                  const Vector<StringID>& descriptorSetsLayoutsSids, const char* name) KMP_PROFILING(ProfileLevelImportant)
        {
            KMP_ASSERT(_device && _descriptorSetManager);

            if (_shaderObjects.contains(sid))
            {
                KMP_LOG_WARN("shader object with sid '{}' has already been created", sid);
                return true;
            }

            try
            {
                const auto descriptorSetsLayouts = _descriptorSetManager->GetDescriptorSetLayouts(descriptorSetsLayoutsSids);
                const auto [iterator, hasEmplaced] = _shaderObjects.emplace(sid, CreateUPtr<VulkanShaderObject>(_device, filepath, stage, nextStage, linked, descriptorSetsLayouts, name));
                return hasEmplaced;
            }
            catch (KMP_MB_UNUSED const RuntimeError& er)
            {
                KMP_LOG_ERROR("failed to create shader object with sid '{}' from '{}'", sid, filepath);
                return false;
            }
        }}
        //--------------------------------------------------------------------------

        VkShaderEXT VulkanLogicalDevice::GetShaderObject(StringID sid) const noexcept
        {
            if (_shaderObjects.contains(sid))
            {
                const auto& shaderObject = _shaderObjects.at(sid);
                if (shaderObject)
                {
                    return shaderObject->GetVkShader();
                }
                else
                {
                    KMP_LOG_ERROR("shader object with sid '{}' found, but is nullptr", sid);
                    return VK_NULL_HANDLE;
                }
            }

            KMP_LOG_ERROR("shader object with sid '{}' not found", sid);
            return VK_NULL_HANDLE;
        }
        //--------------------------------------------------------------------------
    }
}