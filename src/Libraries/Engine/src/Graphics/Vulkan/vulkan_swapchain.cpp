#include "Kmplete/Graphics/Vulkan/vulkan_swapchain.h"
#include "Kmplete/Graphics/Vulkan/Utils/initializers.h"
#include "Kmplete/Graphics/Vulkan/Utils/result_description.h"
#include "Kmplete/Graphics/Vulkan/Utils/presets.h"
#include "Kmplete/Utils/vector_utils.h"
#include "Kmplete/Log/log.h"
#include "Kmplete/Profile/profiler.h"

#include <algorithm>
#include <stdexcept>


namespace Kmplete
{
    namespace Graphics
    {
        VulkanSwapchain::VulkanSwapchain(VkDevice device, const VulkanQueue& presentationQueue, VkSurfaceKHR surface, const VulkanContext& vulkanContext, const VkExtent2D& swapchainExtent,
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

            _imageCount = NumConcurrentFrames;
            if (_vulkanContext.surfaceCapabilities.maxImageCount > 0 && _imageCount > _vulkanContext.surfaceCapabilities.maxImageCount)
            {
                _imageCount = _vulkanContext.surfaceCapabilities.maxImageCount;
            }

            _CreateSwapchainObject(surface);

            _CreateSwapchainImages();
            _CreateSwapchainImageViews();

            SetMultisampling(_msaaSamples);
        }
        //--------------------------------------------------------------------------

        VulkanSwapchain::~VulkanSwapchain()
        {
            KMP_PROFILE_FUNCTION(ProfileLevelAlways);

            _DestroyMultisamplingAttachments();

            for (auto imageView : _swapchainImageViews)
            {
                vkDestroyImageView(_device, imageView, nullptr);
            }

            vkDestroySwapchainKHR(_device, _swapchain, nullptr);
        }
        //--------------------------------------------------------------------------

        void VulkanSwapchain::StartFrame(float /*frameTimestep*/)
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

            const auto result = AcquireNextImage();
            VulkanUtils::CheckResult(result, "VulkanSwapchain: failed to acquire next image");
        }
        //--------------------------------------------------------------------------

        void VulkanSwapchain::EndFrame()
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

            QueuePresent();
        }
        //--------------------------------------------------------------------------

        VkResult VulkanSwapchain::AcquireNextImage()
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctionsVerbose);

            return vkAcquireNextImageKHR(_device, _swapchain, UINT64_MAX, _presentCompleteSemaphores[_currentBufferIndex], nullptr, &_imageIndex);
        }
        //--------------------------------------------------------------------------

        void VulkanSwapchain::QueuePresent()
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctionsVerbose);

            auto presentInfo = VulkanUtils::InitVkPresentInfoKHR();
            presentInfo.swapchainCount = 1;
            presentInfo.pSwapchains = &_swapchain;
            presentInfo.pImageIndices = &_imageIndex;
            presentInfo.waitSemaphoreCount = 1;
            presentInfo.pWaitSemaphores = &_renderCompleteSemaphores[_currentBufferIndex];

            _presentationQueue.Present(presentInfo);
        }
        //--------------------------------------------------------------------------

        VkSampleCountFlagBits VulkanSwapchain::GetMultisampling() const noexcept
        {
            return _msaaSamples;
        }
        //--------------------------------------------------------------------------

        void VulkanSwapchain::SetMultisampling(VkSampleCountFlagBits samples)
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

            _msaaSamples = samples;
            if (_msaaSamples > _vulkanContext.supportedSampleCounts.top())
            {
                KMP_LOG_WARN("cannot set MSAA samples to {}, set to maximum supported {}", UInt32(samples), UInt32(_vulkanContext.supportedSampleCounts.top()));
                _msaaSamples = _vulkanContext.supportedSampleCounts.top();
            }

            _DestroyMultisamplingAttachments();
            _CreateMultisampledAttachments();
        }
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
            return _swapchainImages[_imageIndex];
        }
        //--------------------------------------------------------------------------

        VkImageView VulkanSwapchain::GetCurrentImageView() const
        {
            return _swapchainImageViews[_imageIndex];
        }
        //--------------------------------------------------------------------------

        VkImage VulkanSwapchain::GetMultisampledColorImage() const
        {
            return _multisampledColorImage->GetVkImage();
        }
        //--------------------------------------------------------------------------

        VkImage VulkanSwapchain::GetMultisampledDepthStencilImage() const
        {
            return _multisampledDepthImage->GetVkImage();
        }
        //--------------------------------------------------------------------------

        VkImageView VulkanSwapchain::GetMultisampledColorImageView() const
        {
            return _multisampledColorImageView;
        }
        //--------------------------------------------------------------------------

        VkImageView VulkanSwapchain::GetMultisampledDepthStencilImageView() const
        {
            return _multisampledDepthImageView;
        }
        //--------------------------------------------------------------------------

        VkPresentModeKHR VulkanSwapchain::_ChoosePresentMode(const Vector<VkPresentModeKHR>& presentModes) const
        {
            if (presentModes.empty())
            {
                KMP_LOG_CRITICAL("unable to get available present mode");
                throw std::runtime_error("VulkanSwapchain: unable to get available present mode");
            }

            // TODO: add presentation option in a future, use FIFO by default right now
            //if (Utils::VectorContains(presentModes, VK_PRESENT_MODE_MAILBOX_KHR))
            //{
            //    return VK_PRESENT_MODE_MAILBOX_KHR;
            //}

            return VK_PRESENT_MODE_FIFO_KHR;
        }
        //--------------------------------------------------------------------------

        void VulkanSwapchain::_CreateSwapchainObject(VkSurfaceKHR surface)
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

            auto swapchainCreateInfo = VulkanUtils::InitVkSwapchainCreateInfoKHR();
            swapchainCreateInfo.surface = surface;
            swapchainCreateInfo.minImageCount = _imageCount;
            swapchainCreateInfo.imageFormat = _vulkanContext.surfaceFormat.format;
            swapchainCreateInfo.imageColorSpace = _vulkanContext.surfaceFormat.colorSpace;
            swapchainCreateInfo.imageExtent = _swapchainExtent;
            swapchainCreateInfo.imageArrayLayers = 1;
            swapchainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
            swapchainCreateInfo.preTransform = _vulkanContext.surfaceCapabilities.currentTransform;
            swapchainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
            swapchainCreateInfo.presentMode = _ChoosePresentMode(_vulkanContext.presentModes);
            swapchainCreateInfo.clipped = VK_TRUE;
            swapchainCreateInfo.oldSwapchain = VK_NULL_HANDLE;

            UInt32 indicesArray[] = { _vulkanContext.graphicsFamilyIndex, _vulkanContext.presentFamilyIndex };
            if (_vulkanContext.graphicsFamilyIndex != _vulkanContext.presentFamilyIndex)
            {
                KMP_LOG_WARN("due to different indices of graphics and presentation queues, image sharing mode set to concurrent");
                swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
                swapchainCreateInfo.queueFamilyIndexCount = 2;
                swapchainCreateInfo.pQueueFamilyIndices = indicesArray;
            }
            else
            {
                swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
            }

            const auto result = vkCreateSwapchainKHR(_device, &swapchainCreateInfo, nullptr, &_swapchain);
            VulkanUtils::CheckResult(result, "VulkanSwapchain: failed to create swapchain");
        }
        //--------------------------------------------------------------------------

        void VulkanSwapchain::_CreateSwapchainImages()
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

            vkGetSwapchainImagesKHR(_device, _swapchain, &_imageCount, nullptr);
            _swapchainImages.resize(_imageCount);
            vkGetSwapchainImagesKHR(_device, _swapchain, &_imageCount, _swapchainImages.data());

            for (const auto& swapchainImage : _swapchainImages)
            {
                if (swapchainImage == VK_NULL_HANDLE)
                {
                    KMP_LOG_CRITICAL("one of the swapchain images is invalid");
                    throw std::runtime_error("VulkanSwapchain: one of the swapchain images is invalid");
                }
            }
        }
        //--------------------------------------------------------------------------

        void VulkanSwapchain::_CreateSwapchainImageViews()
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

            _swapchainImageViews.resize(_swapchainImages.size());
            for (size_t i = 0; i < _swapchainImages.size(); i++)
            {
                const auto& subresourceRange = VulkanPresets::ImageSubresourceRange_Color_Layer1_Level1;
                _swapchainImageViews[i] = _imageCreatorDelegate.CreateVkImageView(_swapchainImages[i], VK_IMAGE_VIEW_TYPE_2D, _swapchainImageFormat, subresourceRange);
            }
        }
        //--------------------------------------------------------------------------

        void VulkanSwapchain::_CreateMultisampledAttachments()
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

            const VkExtent3D extent{
                .width = _swapchainExtent.width,
                .height = _swapchainExtent.height,
                .depth = 1
            };

            auto colorCreationParameters = VulkanPresets::GetImageCI_2D_OptimalTiling_QueueExclusive_Layer1(extent, 1, _msaaSamples);
            colorCreationParameters.format = _swapchainImageFormat;
            colorCreationParameters.usage = VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
            colorCreationParameters.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            colorCreationParameters.flags = 0;
            _multisampledColorImage.reset(_imageCreatorDelegate.CreateVulkanImagePtr(colorCreationParameters, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT));

            auto depthCreationParameters = VulkanPresets::GetImageCI_2D_OptimalTiling_QueueExclusive_Layer1(extent, 1, _msaaSamples);
            depthCreationParameters.format = _vulkanContext.defaultDepthFormat;
            depthCreationParameters.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
            _multisampledDepthImage.reset(_imageCreatorDelegate.CreateVulkanImagePtr(depthCreationParameters, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT));

            const auto& colorSubresourceRange = VulkanPresets::ImageSubresourceRange_Color_Layer1_Level1;
            _multisampledColorImageView = _imageCreatorDelegate.CreateVkImageView(*_multisampledColorImage.get(), VK_IMAGE_VIEW_TYPE_2D, _swapchainImageFormat, colorSubresourceRange);

            const auto& depthSubresourceRange = VulkanPresets::ImageSubresourceRange_DepthStencil_Layer1_Level1;
            _multisampledDepthImageView = _imageCreatorDelegate.CreateVkImageView(*_multisampledDepthImage.get(), VK_IMAGE_VIEW_TYPE_2D, _vulkanContext.defaultDepthFormat, depthSubresourceRange);
        }
        //--------------------------------------------------------------------------

        void VulkanSwapchain::_DestroyMultisamplingAttachments()
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctionsVerbose);

            if (_multisampledColorImageView != VK_NULL_HANDLE)
            {
                vkDestroyImageView(_device, _multisampledColorImageView, nullptr);
            }
            _multisampledColorImage.reset();

            if (_multisampledDepthImageView != VK_NULL_HANDLE)
            {
                vkDestroyImageView(_device, _multisampledDepthImageView, nullptr);
            }
            _multisampledDepthImage.reset();
        }
        //--------------------------------------------------------------------------
    }
}