#include "Kmplete/Graphics/Vulkan/vulkan_swapchain.h"
#include "Kmplete/Graphics/Vulkan/Utils/initializers.h"
#include "Kmplete/Graphics/Vulkan/Utils/result_description.h"
#include "Kmplete/Graphics/Vulkan/Utils/presets.h"
#include "Kmplete/Graphics/Vulkan/Utils/bits_aliases.h"
#include "Kmplete/Utils/vector_utils.h"
#include "Kmplete/Base/exception.h"
#include "Kmplete/Core/assertion.h"
#include "Kmplete/Log/log.h"
#include "Kmplete/Profile/profiler.h"

#include <algorithm>


namespace Kmplete
{
    namespace Graphics
    {
        using namespace VKBits;


        VulkanSwapchain::VulkanSwapchain(VkDevice device, const VulkanQueue& presentationQueue, const VulkanContext& vulkanContext, const VkExtent2D& swapchainExtent,
                                         VkSampleCountFlagBits msaaSamples, const VulkanImageCreatorDelegate& imageCreatorDelegate, const UInt32& currentBufferIndex,
                                         const Array<VkSemaphore, NumConcurrentFrames>& presentCompleteSemaphores, const Array<VkSemaphore, NumConcurrentFrames>& renderCompleteSemaphores)
            : Swapchain()
            , _vulkanContext(vulkanContext)
            , _swapchainExtent(swapchainExtent)
            , _imageCreatorDelegate(imageCreatorDelegate)
            , _currentBufferIndex(currentBufferIndex)
            , _presentCompleteSemaphores(presentCompleteSemaphores)
            , _renderCompleteSemaphores(renderCompleteSemaphores)
            , _presentationQueue(presentationQueue)
            , _device(device)
            , _imageIndex(0)
            , _imageCount(0)
            , _swapchain(VK_NULL_HANDLE)
            , _swapchainImages()
            , _swapchainImageFormat(_vulkanContext.surfaceFormat.format)
            , _swapchainImageViews()
            , _msaaSamples(msaaSamples)
            , _multisampledColorImage(nullptr)
            , _multisampledColorImageView(VK_NULL_HANDLE)
            , _multisampledDepthImage(nullptr)
            , _multisampledDepthImageView(VK_NULL_HANDLE)
        {
            KMP_PROFILE_FUNCTION(ProfileLevelAlways);

            KMP_ASSERT(_device);

            _imageCount = NumConcurrentFrames;
            if (_vulkanContext.surfaceCapabilities.maxImageCount > 0 && _imageCount > _vulkanContext.surfaceCapabilities.maxImageCount)
            {
                _imageCount = _vulkanContext.surfaceCapabilities.maxImageCount;
            }

            _CreateSwapchainObject(vulkanContext.surface);

            _CreateSwapchainImages();
            _CreateSwapchainImageViews();
            _CreateMultisampledAttachments();
        }
        //--------------------------------------------------------------------------

        VulkanSwapchain::~VulkanSwapchain() KMP_PROFILING(ProfileLevelAlways)
        {
            KMP_ASSERT(_device && _swapchain);

            _DestroyMultisamplingAttachments();

            for (auto imageView : _swapchainImageViews)
            {
                KMP_ASSERT(imageView);
                vkDestroyImageView(_device, imageView, nullptr);
            }

            vkDestroySwapchainKHR(_device, _swapchain, nullptr);
        }}
        //--------------------------------------------------------------------------

        void VulkanSwapchain::StartFrame(float /*frameTimestep*/) KMP_PROFILING(ProfileLevelImportant)
        {
            const auto result = AcquireNextImage();
            VKUtils::CheckResult(result, "VulkanSwapchain: failed to acquire next image");
        }}
        //--------------------------------------------------------------------------

        void VulkanSwapchain::EndFrame() KMP_PROFILING(ProfileLevelImportant)
        {
            QueuePresent();
        }}
        //--------------------------------------------------------------------------

        VkResult VulkanSwapchain::AcquireNextImage() KMP_PROFILING(ProfileLevelImportantVerbose)
        {
            KMP_ASSERT(_device && _swapchain && _currentBufferIndex < _presentCompleteSemaphores.size());

            return vkAcquireNextImageKHR(_device, _swapchain, UINT64_MAX, _presentCompleteSemaphores[_currentBufferIndex], nullptr, &_imageIndex);
        }}
        //--------------------------------------------------------------------------

        void VulkanSwapchain::QueuePresent() KMP_PROFILING(ProfileLevelImportantVerbose)
        {
            KMP_ASSERT(_swapchain && _currentBufferIndex < _renderCompleteSemaphores.size());

            auto presentInfo = VKUtils::InitVkPresentInfoKHR();
            presentInfo.swapchainCount = 1;
            presentInfo.pSwapchains = &_swapchain;
            presentInfo.pImageIndices = &_imageIndex;
            presentInfo.waitSemaphoreCount = 1;
            presentInfo.pWaitSemaphores = &_renderCompleteSemaphores[_currentBufferIndex];

            _presentationQueue.Present(presentInfo);
        }}
        //--------------------------------------------------------------------------

        VkSampleCountFlagBits VulkanSwapchain::GetMultisampling() const noexcept
        {
            return _msaaSamples;
        }
        //--------------------------------------------------------------------------

        void VulkanSwapchain::SetMultisampling(VkSampleCountFlagBits samples) KMP_PROFILING(ProfileLevelImportant)
        {
            if (_msaaSamples == samples)
            {
                KMP_LOG_INFO("swapchain already contains {} samples", int(samples));
                return;
            }

            _msaaSamples = samples;
            if (_msaaSamples > _vulkanContext.supportedSampleCounts.top())
            {
                KMP_LOG_WARN("cannot set MSAA samples to {}, set to maximum supported {}", UInt32(samples), UInt32(_vulkanContext.supportedSampleCounts.top()));
                _msaaSamples = _vulkanContext.supportedSampleCounts.top();
            }

            _DestroyMultisamplingAttachments();
            _CreateMultisampledAttachments();
        }}
        //--------------------------------------------------------------------------

        UInt32 VulkanSwapchain::GetImageIndex() const noexcept
        {
            return _imageIndex;
        }
        //--------------------------------------------------------------------------

        UInt32 VulkanSwapchain::GetImageCount() const noexcept
        {
            return _imageCount;
        }
        //--------------------------------------------------------------------------

        VkImage VulkanSwapchain::GetCurrentImage() const
        {
            KMP_ASSERT(_imageIndex < _swapchainImages.size());

            return _swapchainImages[_imageIndex];
        }
        //--------------------------------------------------------------------------

        VkImageView VulkanSwapchain::GetCurrentImageView() const
        {
            KMP_ASSERT(_imageIndex < _swapchainImageViews.size());

            return _swapchainImageViews[_imageIndex];
        }
        //--------------------------------------------------------------------------

        VkImage VulkanSwapchain::GetMultisampledColorImage() const
        {
            KMP_ASSERT(_multisampledColorImage);

            return _multisampledColorImage->GetVkImage();
        }
        //--------------------------------------------------------------------------

        VkImage VulkanSwapchain::GetMultisampledDepthStencilImage() const
        {
            KMP_ASSERT(_multisampledDepthImage);

            return _multisampledDepthImage->GetVkImage();
        }
        //--------------------------------------------------------------------------

        VkImageView VulkanSwapchain::GetMultisampledColorImageView() const
        {
            KMP_ASSERT(_multisampledColorImageView);

            return _multisampledColorImageView;
        }
        //--------------------------------------------------------------------------

        VkImageView VulkanSwapchain::GetMultisampledDepthStencilImageView() const
        {
            KMP_ASSERT(_multisampledDepthImageView);

            return _multisampledDepthImageView;
        }
        //--------------------------------------------------------------------------

        VkRenderingAttachmentInfo VulkanSwapchain::GetRenderingColorAttachmentInfo(bool clearPrevious /*= true*/) const
        {
            auto colorAttachmentInfo = clearPrevious ? VKPresets::RenderingAttachmentInfo_Color_ClearStore : VKPresets::RenderingAttachmentInfo_Color_LoadStore;
            if (GetMultisampling() == VK_SampleCount_1)
            {
                colorAttachmentInfo.imageView = GetCurrentImageView();
            }
            else
            {
                colorAttachmentInfo.imageView = GetMultisampledColorImageView();
                colorAttachmentInfo.resolveMode = VK_Resolve_Average;
                colorAttachmentInfo.resolveImageView = GetCurrentImageView();
                colorAttachmentInfo.resolveImageLayout = VK_ImageLayout_AttachmentOptimal;
            }

            return colorAttachmentInfo;
        }
        //--------------------------------------------------------------------------

        VkRenderingAttachmentInfo VulkanSwapchain::GetRenderingDepthStencilAttachmentInfo(bool clearPrevious /*= true*/) const
        {
            auto depthStencilAttachmentInfo = clearPrevious ? VKPresets::RenderingAttachmentInfo_DepthStencil_ClearStore : VKPresets::RenderingAttachmentInfo_DepthStencil_LoadStore;
            depthStencilAttachmentInfo.imageView = GetMultisampledDepthStencilImageView();

            return depthStencilAttachmentInfo;
        }
        //--------------------------------------------------------------------------

        VkPresentModeKHR VulkanSwapchain::_ChoosePresentMode(const Vector<VkPresentModeKHR>& presentModes) const
        {
            if (presentModes.empty())
            {
                KMP_LOG_CRITICAL("unable to get available present mode");
                throw RuntimeError("VulkanSwapchain: unable to get available present mode");
            }

            // TODO: add presentation option in a future, use FIFO by default right now
            //if (Utils::VectorContains(presentModes, VK_PresentMode_Mailbox))
            //{
            //    return VK_PresentMode_Mailbox;
            //}

            return VK_PresentMode_FIFO;
        }
        //--------------------------------------------------------------------------

        void VulkanSwapchain::_CreateSwapchainObject(VkSurfaceKHR surface) KMP_PROFILING(ProfileLevelImportant)
        {
            auto swapchainCreateInfo = VKUtils::InitVkSwapchainCreateInfoKHR();
            swapchainCreateInfo.surface = surface;
            swapchainCreateInfo.minImageCount = _imageCount;
            swapchainCreateInfo.imageFormat = _vulkanContext.surfaceFormat.format;
            swapchainCreateInfo.imageColorSpace = _vulkanContext.surfaceFormat.colorSpace;
            swapchainCreateInfo.imageExtent = _swapchainExtent;
            swapchainCreateInfo.imageArrayLayers = 1;
            swapchainCreateInfo.imageUsage = VK_ImageUsage_ColorAttachment;
            swapchainCreateInfo.preTransform = _vulkanContext.surfaceCapabilities.currentTransform;
            swapchainCreateInfo.compositeAlpha = VK_CompositeAlpha_Opaque;
            swapchainCreateInfo.presentMode = _ChoosePresentMode(_vulkanContext.presentModes);
            swapchainCreateInfo.clipped = VK_TRUE;
            swapchainCreateInfo.oldSwapchain = VK_NULL_HANDLE;

            UInt32 indicesArray[] = { _vulkanContext.graphicsFamilyIndex, _vulkanContext.presentFamilyIndex };
            if (_vulkanContext.graphicsFamilyIndex != _vulkanContext.presentFamilyIndex)
            {
                KMP_LOG_WARN("due to different indices of graphics and presentation queues, image sharing mode set to concurrent");
                swapchainCreateInfo.imageSharingMode = VK_Sharing_Concurrent;
                swapchainCreateInfo.queueFamilyIndexCount = 2;
                swapchainCreateInfo.pQueueFamilyIndices = indicesArray;
            }
            else
            {
                swapchainCreateInfo.imageSharingMode = VK_Sharing_Exclusive;
            }

            const auto result = vkCreateSwapchainKHR(_device, &swapchainCreateInfo, nullptr, &_swapchain);
            VKUtils::CheckResult(result, "VulkanSwapchain: failed to create swapchain");
            KMP_ASSERT(_swapchain);
        }}
        //--------------------------------------------------------------------------

        void VulkanSwapchain::_CreateSwapchainImages() KMP_PROFILING(ProfileLevelImportant)
        {
            vkGetSwapchainImagesKHR(_device, _swapchain, &_imageCount, nullptr);
            _swapchainImages.resize(_imageCount);
            vkGetSwapchainImagesKHR(_device, _swapchain, &_imageCount, _swapchainImages.data());

            for (const auto& swapchainImage : _swapchainImages)
            {
                if (swapchainImage == VK_NULL_HANDLE)
                {
                    KMP_LOG_CRITICAL("one of the swapchain images is invalid");
                    throw RuntimeError("VulkanSwapchain: one of the swapchain images is invalid");
                }
            }
        }}
        //--------------------------------------------------------------------------

        void VulkanSwapchain::_CreateSwapchainImageViews() KMP_PROFILING(ProfileLevelImportant)
        {
            _swapchainImageViews.resize(_swapchainImages.size());
            for (size_t i = 0; i < _swapchainImages.size(); i++)
            {
                const auto& subresourceRange = VKPresets::ImageSubresourceRange_Color_Layer1_Level1;
                _swapchainImageViews[i] = _imageCreatorDelegate.CreateVkImageView(_swapchainImages[i], VK_ImageView_2D, _swapchainImageFormat, subresourceRange);
            }
        }}
        //--------------------------------------------------------------------------

        void VulkanSwapchain::_CreateMultisampledAttachments() KMP_PROFILING(ProfileLevelImportant)
        {
            const VkExtent3D extent{
                .width = _swapchainExtent.width,
                .height = _swapchainExtent.height,
                .depth = 1
            };

            const auto colorCreationParameters = VKPresets::GetImageCI_2D_OptimalTiling_QueueExclusive_Layer1_NoLayout(_swapchainImageFormat, extent, 1, _msaaSamples, VK_ImageUsage_TransientAttachment | VK_ImageUsage_ColorAttachment);
            _multisampledColorImage.reset(_imageCreatorDelegate.CreateVulkanImagePtr(colorCreationParameters, VK_Memory_DeviceLocal));
            KMP_ASSERT(_multisampledColorImage);

            const auto depthCreationParameters = VKPresets::GetImageCI_2D_OptimalTiling_QueueExclusive_Layer1_NoLayout(_vulkanContext.defaultDepthFormat, extent, 1, _msaaSamples, VK_ImageUsage_DepthStencilAttachment);
            _multisampledDepthImage.reset(_imageCreatorDelegate.CreateVulkanImagePtr(depthCreationParameters, VK_Memory_DeviceLocal));
            KMP_ASSERT(_multisampledDepthImage);

            const auto& colorSubresourceRange = VKPresets::ImageSubresourceRange_Color_Layer1_Level1;
            _multisampledColorImageView = _imageCreatorDelegate.CreateVkImageView(*_multisampledColorImage.get(), VK_ImageView_2D, _swapchainImageFormat, colorSubresourceRange);
            KMP_ASSERT(_multisampledColorImageView);

            const auto& depthSubresourceRange = VKPresets::ImageSubresourceRange_DepthStencil_Layer1_Level1;
            _multisampledDepthImageView = _imageCreatorDelegate.CreateVkImageView(*_multisampledDepthImage.get(), VK_ImageView_2D, _vulkanContext.defaultDepthFormat, depthSubresourceRange);
            KMP_ASSERT(_multisampledDepthImageView);
        }}
        //--------------------------------------------------------------------------

        void VulkanSwapchain::_DestroyMultisamplingAttachments() KMP_PROFILING(ProfileLevelImportantVerbose)
        {
            KMP_ASSERT(_device && _multisampledColorImageView && _multisampledColorImage && _multisampledDepthImageView && _multisampledDepthImage);

            vkDestroyImageView(_device, _multisampledColorImageView, nullptr);
            _multisampledColorImage.reset();

            vkDestroyImageView(_device, _multisampledDepthImageView, nullptr);
            _multisampledDepthImage.reset();
        }}
        //--------------------------------------------------------------------------
    }
}