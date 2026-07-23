#include "Kmplete/Graphics/Vulkan/Core/vulkan_graphics_base.h"
#include "Kmplete/Graphics/Vulkan/Core/vulkan_graphics_parameters.h"
#include "Kmplete/Graphics/Vulkan/Core/vulkan_logical_device.h"
#include "Kmplete/Graphics/Vulkan/Core/vulkan_physical_device.h"
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
              KMP_PROFILE_CONSTRUCTOR_START_DERIVED_CLASS()
            , _vulkanContext(vulkanContext)
            , _memoryTypeDelegate(memoryTypeDelegate)
            , _formatDelegate(formatDelegate)
            , _window(window)
            , _currentBufferIndex(currentBufferIndex)
            , _physicalDevice(physicalDevice)
            , _surface(surface)
            , _graphicsParameters(nullptr)
            , _device(nullptr)
            , _graphicsQueue(nullptr)
            , _presentQueue(nullptr)
            , _imageCreatorDelegate(nullptr)
            , _presentCompleteSemaphores()
            , _renderCompleteSemaphores()
            , _waitFences()
            , _swapchain(nullptr)
            , _descriptorSetManager(nullptr)
            , _bufferManager(nullptr)
            , _currentExtent(_UpdateExtent())
            , _msaaSamples(VK_SampleCount_1)
            , _vSync(true)
            , _samplersStorage(nullptr)
            , _pipelineManager(nullptr)
            , _textureAttachmentManager(nullptr)
            , _shaderManager(nullptr)
            , _renderer(nullptr)
            , _metricsManager(nullptr)
        {
            _CreateLogicalDeviceObject();
            _CreateDeviceQueues();
            _CreateImageCreatorDelegate();
            _CreateSynchronizationObjects();
            _CreateSwapchain();
            _CreateDescriptorSetManager();
            _CreateBufferManager();
            _CreateSamplersStorage();
            _CreatePipelineManager();
            _CreateTextureAttachmentManager();
            _CreateShaderManager();
            _CreateRenderer();
            _CreateMetricsManager();

            KMP_PROFILE_CONSTRUCTOR_END()
        }
        //--------------------------------------------------------------------------

        VulkanLogicalDevice::~VulkanLogicalDevice() KMP_PROFILING(ProfileLevelAlways)
        {
            WaitIdle();

            _DeleteMetricsManager();
            _DeleteRenderer();
            _DeleteShaderManager();
            _DeleteTextureAttachmentManager();
            _DeletePipelineManager();
            _DeleteSamplersStorage();
            _DeleteBufferManager();
            _DeleteDescriptorSetManager();
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

            VKUtils::MemoryBarrierParameters imageBarrierParameters = {
                .srcAccessMask = VK_Access_None,
                .dstAccessMask = VK_Access_ColorAttachmentWrite,
                .oldImageLayout = VK_ImageLayout_Undefined,
                .newImageLayout = VK_ImageLayout_AttachmentOptimal,
                .srcStageMask = VK_PipelineStage_ColorAttachmentOutput,
                .dstStageMask = VK_PipelineStage_ColorAttachmentOutput,
                .subresourceRange = VKPresets::ImageSubresourceRange_Color_Layer1_Level1
            };
            _renderer->InsertImageMemoryBarrier(_swapchain->GetCurrentImage(), imageBarrierParameters);
        }}
        //--------------------------------------------------------------------------

        void VulkanLogicalDevice::EndFrame() KMP_PROFILING(ProfileLevelImportant)
        {
            KMP_ASSERT(_swapchain && _renderer && _graphicsQueue);
            KMP_ASSERT(_currentBufferIndex < _waitFences.size());
            KMP_ASSERT(_currentBufferIndex < _presentCompleteSemaphores.size());
            KMP_ASSERT(_currentBufferIndex < _renderCompleteSemaphores.size());

            VKUtils::MemoryBarrierParameters memoryBarrierParameters = {
                .srcAccessMask = VK_Access_ColorAttachmentWrite,
                .dstAccessMask = VK_Access_None,
                .oldImageLayout = VK_ImageLayout_AttachmentOptimal,
                .newImageLayout = VK_ImageLayout_PresentKHR,
                .srcStageMask = VK_PipelineStage_ColorAttachmentOutput,
                .dstStageMask = VK_PipelineStage_BottomOfPipe,
                .subresourceRange = VKPresets::ImageSubresourceRange_Color_Layer1_Level1
            };
            _renderer->InsertImageMemoryBarrier(_swapchain->GetCurrentImage(), memoryBarrierParameters);
            _renderer->EndFrame();
            _renderer->SubmitToQueue(*_graphicsQueue.get(), { _presentCompleteSemaphores[_currentBufferIndex] }, { _renderCompleteSemaphores[_currentBufferIndex] }, _waitFences[_currentBufferIndex].GetVkFence());

            _swapchain->EndFrame();

            WaitIdle();
        }}
        //--------------------------------------------------------------------------

        void VulkanLogicalDevice::HandleWindowResize() KMP_PROFILING(ProfileLevelImportant)
        {
            _currentExtent = _UpdateExtent();

            _RecreateSwapchain();

            _textureAttachmentManager->RecreateTextureAttachmentsWithNewSize(VKUtils::Extent2Dto3D(_currentExtent));
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

            _textureAttachmentManager->RecreateTextureAttachmentsWithNewSamples(_msaaSamples);
        }}
        //--------------------------------------------------------------------------

        bool VulkanLogicalDevice::IsVSync() const noexcept
        {
            return _vSync;
        }
        //--------------------------------------------------------------------------

        void VulkanLogicalDevice::SetVSync(bool vSync) KMP_PROFILING(ProfileLevelImportant)
        {
            _vSync = vSync;
            _RecreateSwapchain();
        }}
        //--------------------------------------------------------------------------

        const VulkanSwapchain& VulkanLogicalDevice::GetSwapchain() const noexcept
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

        const VulkanPipelineManager& VulkanLogicalDevice::GetPipelineManager() const noexcept
        {
            KMP_ASSERT(_pipelineManager);

            return *_pipelineManager.get();
        }
        //--------------------------------------------------------------------------

        VulkanPipelineManager& VulkanLogicalDevice::GetPipelineManager() noexcept
        {
            KMP_ASSERT(_pipelineManager);

            return *_pipelineManager.get();
        }
        //--------------------------------------------------------------------------

        const VulkanTextureAttachmentManager& VulkanLogicalDevice::GetTextureAttachmentManager() const noexcept
        {
            KMP_ASSERT(_textureAttachmentManager);

            return *_textureAttachmentManager.get();
        }
        //--------------------------------------------------------------------------

        VulkanTextureAttachmentManager& VulkanLogicalDevice::GetTextureAttachmentManager() noexcept
        {
            KMP_ASSERT(_textureAttachmentManager);

            return *_textureAttachmentManager.get();
        }
        //--------------------------------------------------------------------------

        const VulkanShaderManager& VulkanLogicalDevice::GetShaderManager() const noexcept
        {
            KMP_ASSERT(_shaderManager);

            return *_shaderManager.get();
        }
        //--------------------------------------------------------------------------

        VulkanShaderManager& VulkanLogicalDevice::GetShaderManager() noexcept
        {
            KMP_ASSERT(_shaderManager);

            return *_shaderManager.get();
        }
        //--------------------------------------------------------------------------

        const VulkanBufferManager& VulkanLogicalDevice::GetBufferManager() const noexcept
        {
            KMP_ASSERT(_bufferManager);

            return *_bufferManager.get();
        }
        //--------------------------------------------------------------------------

        VulkanBufferManager& VulkanLogicalDevice::GetBufferManager() noexcept
        {
            KMP_ASSERT(_bufferManager);

            return *_bufferManager.get();
        }
        //--------------------------------------------------------------------------

        const VulkanMetricsManager& VulkanLogicalDevice::GetMetricsManager() const noexcept
        {
            KMP_ASSERT(_metricsManager);

            return *_metricsManager.get();
        }
        //--------------------------------------------------------------------------

        VulkanMetricsManager& VulkanLogicalDevice::GetMetricsManager() noexcept
        {
            KMP_ASSERT(_metricsManager);

            return *_metricsManager.get();
        }
        //--------------------------------------------------------------------------

        void VulkanLogicalDevice::_CreateLogicalDeviceObject() KMP_PROFILING(ProfileLevelImportant)
        {
            _graphicsParameters.reset(new VulkanGraphicsParameters());
            if (ClientInitializeGraphicsParametersFn == nullptr)
            {
                KMP_LOG_WARN("graphics parameters function pointer is not assigned");
            }
            else
            {
                ClientInitializeGraphicsParametersFn(*_graphicsParameters);
            }

            const auto queueCreateInfos = _CreateQueueCreateInfos();

            const auto& enabledDeviceExtensions = VulkanPhysicalDevice::GetEnabledDeviceExtensions();

            auto deviceCreateInfo = VKUtils::InitVkDeviceCreateInfo();
            deviceCreateInfo.queueCreateInfoCount = UInt32(queueCreateInfos.size());
            deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();
            deviceCreateInfo.pEnabledFeatures = nullptr;
            deviceCreateInfo.enabledExtensionCount = UInt32(enabledDeviceExtensions.size());
            deviceCreateInfo.ppEnabledExtensionNames = enabledDeviceExtensions.data();
            deviceCreateInfo.pNext = &_graphicsParameters->features2;

            const auto result = vkCreateDevice(_physicalDevice, &deviceCreateInfo, nullptr, &_device);
            VKUtils::CheckResult(result, "VulkanLogicalDevice: failed to create logical device");
            KMP_ASSERT(_device);

            KMP_LOG_INFO("logical device object successfully created");
        }}
        //--------------------------------------------------------------------------

        void VulkanLogicalDevice::_DeleteLogicalDeviceObject() KMP_PROFILING(ProfileLevelImportant)
        {
            KMP_ASSERT(_device && _graphicsParameters);

            _graphicsParameters.reset();
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

            _swapchain.reset(new VulkanSwapchain(_device, *_presentQueue.get(), _vulkanContext, _currentExtent, _vSync, *_imageCreatorDelegate.get(), _currentBufferIndex, _presentCompleteSemaphores, _renderCompleteSemaphores));
            KMP_ASSERT(_swapchain);
        }}
        //--------------------------------------------------------------------------

        void VulkanLogicalDevice::_DeleteSwapchain() KMP_PROFILING(ProfileLevelImportant)
        {
            KMP_ASSERT(_swapchain);

            _swapchain.reset();
        }}
        //--------------------------------------------------------------------------

        void VulkanLogicalDevice::_CreateDescriptorSetManager() KMP_PROFILING(ProfileLevelImportant)
        {
            KMP_ASSERT(_device);

            _descriptorSetManager.reset(new VulkanDescriptorSetManager(_device, _currentBufferIndex, _graphicsParameters->maxDescriptorSets, _graphicsParameters->descriptorPoolSizes));
            KMP_ASSERT(_descriptorSetManager);
        }}
        //--------------------------------------------------------------------------

        void VulkanLogicalDevice::_DeleteDescriptorSetManager() KMP_PROFILING(ProfileLevelImportant)
        {
            KMP_ASSERT(_descriptorSetManager);

            _descriptorSetManager.reset();
        }}
        //--------------------------------------------------------------------------

        void VulkanLogicalDevice::_CreateBufferManager() KMP_PROFILING(ProfileLevelImportant)
        {
            KMP_ASSERT(_device);

            _bufferManager.reset(new VulkanBufferManager(_device, _memoryTypeDelegate));
            KMP_ASSERT(_bufferManager);
        }}
        //--------------------------------------------------------------------------

        void VulkanLogicalDevice::_DeleteBufferManager() KMP_PROFILING(ProfileLevelImportant)
        {
            KMP_ASSERT(_bufferManager);

            _bufferManager.reset();
        }}
        //--------------------------------------------------------------------------

        void VulkanLogicalDevice::_CreateSamplersStorage() KMP_PROFILING(ProfileLevelImportant)
        {
            KMP_ASSERT(_device);

            _samplersStorage.reset(new VulkanSamplersStorage(_device));
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

        void VulkanLogicalDevice::_CreatePipelineManager() KMP_PROFILING(ProfileLevelImportant)
        {
            KMP_ASSERT(_device && _descriptorSetManager);

            _pipelineManager.reset(new VulkanPipelineManager(_device, _vulkanContext, *_descriptorSetManager.get()));
            KMP_ASSERT(_pipelineManager);
        }}
        //--------------------------------------------------------------------------

        void VulkanLogicalDevice::_DeletePipelineManager() KMP_PROFILING(ProfileLevelImportant)
        {
            KMP_ASSERT(_pipelineManager);

            _pipelineManager.reset();
        }}
        //--------------------------------------------------------------------------

        void VulkanLogicalDevice::_CreateTextureAttachmentManager() KMP_PROFILING(ProfileLevelImportant)
        {
            KMP_ASSERT(_device);

            _textureAttachmentManager.reset(new VulkanTextureAttachmentManager(_device, VKUtils::Extent2Dto3D(_currentExtent), _msaaSamples, *_imageCreatorDelegate.get(), *_swapchain.get()));
            KMP_ASSERT(_textureAttachmentManager);
        }}
        //--------------------------------------------------------------------------

        void VulkanLogicalDevice::_DeleteTextureAttachmentManager() KMP_PROFILING(ProfileLevelImportant)
        {
            KMP_ASSERT(_textureAttachmentManager);

            _textureAttachmentManager.reset();
        }}
        //--------------------------------------------------------------------------

        void VulkanLogicalDevice::_CreateShaderManager() KMP_PROFILING(ProfileLevelImportant)
        {
            KMP_ASSERT(_device);

            _shaderManager.reset(new VulkanShaderManager(_device, *_descriptorSetManager.get()));
            KMP_ASSERT(_shaderManager);
        }}
        //--------------------------------------------------------------------------

        void VulkanLogicalDevice::_DeleteShaderManager() KMP_PROFILING(ProfileLevelImportant)
        {
            KMP_ASSERT(_shaderManager);

            _shaderManager.reset();
        }}
        //--------------------------------------------------------------------------

        void VulkanLogicalDevice::_CreateRenderer() KMP_PROFILING(ProfileLevelImportant)
        {
            KMP_ASSERT(_device && _swapchain);

            _renderer.reset(new VulkanRenderer(_device, _currentBufferIndex, *_pipelineManager.get(), *_shaderManager.get(), _vulkanContext.graphicsFamilyIndex, *_swapchain.get()));
            KMP_ASSERT(_renderer);
        }}
        //--------------------------------------------------------------------------

        void VulkanLogicalDevice::_DeleteRenderer() KMP_PROFILING(ProfileLevelImportant)
        {
            KMP_ASSERT(_renderer);

            _renderer.reset();
        }}
        //--------------------------------------------------------------------------

        void VulkanLogicalDevice::_CreateMetricsManager()
        {
            KMP_ASSERT(_physicalDevice);

            _metricsManager.reset(new VulkanMetricsManager(_physicalDevice));
            KMP_ASSERT(_metricsManager);
        }
        //--------------------------------------------------------------------------

        void VulkanLogicalDevice::_DeleteMetricsManager()
        {
            KMP_ASSERT(_metricsManager);

            _metricsManager.reset();
        }
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

            const auto windowSize = _window.GetFramebufferSize();
            const auto actualExtent = VkExtent2D{
                Math::Clamp(UInt32(windowSize.x), capabilities.minImageExtent.width, capabilities.maxImageExtent.width),
                Math::Clamp(UInt32(windowSize.y), capabilities.minImageExtent.height, capabilities.maxImageExtent.height)
            };

            return actualExtent;
        }}
        //--------------------------------------------------------------------------

        void VulkanLogicalDevice::_RecreateSwapchain() KMP_PROFILING(ProfileLevelImportantVerbose)
        {
            KMP_ASSERT(_swapchain);

            WaitIdle();

            _DeleteSyncronizationObjects();
            _CreateSynchronizationObjects();

            _swapchain->Recreate(_currentExtent, _vSync, _presentCompleteSemaphores, _renderCompleteSemaphores);
        }}
        //--------------------------------------------------------------------------

        Nullable<VulkanTexture*> VulkanLogicalDevice::CreateTexture(const Image& image, Assets::TextureSubTypeMaskBits subTypeMask) const KMP_PROFILING(ProfileLevelImportant)
        {
            KMP_ASSERT(_device && _imageCreatorDelegate && _graphicsQueue);

            try
            {
                const auto isSRGB = subTypeMask & Assets::TextureSubTypeMaskBits::SRGB;
                const auto textureVkFormat = ImageChannelsToVkFormat(ImageChannels(image.GetChannels()), isSRGB);
                const auto mipLevels = _formatDelegate.IsMipmapCompatible(textureVkFormat) ? image.GetMipLevels() : 1;
                auto imageBuffer = _imageCreatorDelegate->CreateStagingImageBuffer(image);
                const auto extent = VkExtent3D{
                    .width = UInt32(image.GetWidth()),
                    .height = UInt32(image.GetHeight()),
                    .depth = 1
                };
                const auto imageType = extent.height > 1 ? VK_Image_2D : VK_Image_1D;

                auto commandBuffer = _renderer->CreateCommandBuffer();
                commandBuffer.Begin(VK_CommandBufferUsage_OneTimeSubmit);
                auto* texture = new VulkanTexture(imageType, textureVkFormat, mipLevels, _device, commandBuffer.GetVkCommandBuffer(), imageBuffer, extent, *_imageCreatorDelegate.get());
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
    }
}