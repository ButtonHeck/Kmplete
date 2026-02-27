#include "Kmplete/Graphics/Vulkan/vulkan_swapchain.h"
#include "Kmplete/Graphics/Vulkan/Utils/initializers.h"
#include "Kmplete/Graphics/Vulkan/Utils/function_utils.h"
#include "Kmplete/Graphics/Vulkan/Utils/helper_structs.h"
#include "Kmplete/Log/log.h"
#include "Kmplete/Profile/profiler.h"

#include <algorithm>
#include <stdexcept>


namespace Kmplete
{
    namespace Graphics
    {
        VulkanSwapchain::VulkanSwapchain(VkDevice device, VkQueue graphicsQueue, VkSurfaceKHR surface, const VulkanContext& vulkanContext, const VkExtent2D& swapchainExtent,
                                         const VulkanImageCreatorDelegate& imageCreatorDelegate, const UInt32& currentBufferIndex,
                                         const Array<VkSemaphore, NumConcurrentFrames>& presentCompleteSemaphores, const Array<VkSemaphore, NumConcurrentFrames>& renderCompleteSemaphores)
            : Swapchain()
            , _vulkanContext(vulkanContext)
            , _swapchainExtent(swapchainExtent)
            , _imageCreatorDelegate(imageCreatorDelegate)
            , _currentBufferIndex(currentBufferIndex)
            , _presentCompleteSemaphores(presentCompleteSemaphores)
            , _renderCompleteSemaphores(renderCompleteSemaphores)
            , _device(device)
            , _graphicsQueue(graphicsQueue)
            , _imageIndex(0)
            , _imageCount(0)
            , _swapchain(VK_NULL_HANDLE)
            , _swapchainImages()
            , _swapchainImageFormat(_vulkanContext.surfaceFormat.format)
            , _swapchainImageViews()
            , _colorImage(nullptr)
            , _colorImageView(VK_NULL_HANDLE)
            , _depthImage(nullptr)
            , _depthImageView(VK_NULL_HANDLE)
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

            _CreateAttachmentImages();
            _CreateAttachmentImagesViews();
        }
        //--------------------------------------------------------------------------

        VulkanSwapchain::~VulkanSwapchain()
        {
            KMP_PROFILE_FUNCTION(ProfileLevelAlways);

            vkDestroyImageView(_device, _colorImageView, nullptr);
            _colorImage.reset();

            vkDestroyImageView(_device, _depthImageView, nullptr);
            _depthImage.reset();

            for (auto imageView : _swapchainImageViews)
            {
                vkDestroyImageView(_device, imageView, nullptr);
            }

            vkDestroySwapchainKHR(_device, _swapchain, nullptr);
        }

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

            const auto result = QueuePresent();
            VulkanUtils::CheckResult(result, "VulkanSwapchain: failed to present swapchain image");
        }
        //--------------------------------------------------------------------------

        VkResult VulkanSwapchain::AcquireNextImage()
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctionsVerbose);

            return vkAcquireNextImageKHR(_device, _swapchain, UINT64_MAX, _presentCompleteSemaphores[_currentBufferIndex], nullptr, &_imageIndex);
        }
        //--------------------------------------------------------------------------

        VkResult VulkanSwapchain::QueuePresent()
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctionsVerbose);

            auto presentInfo = VulkanUtils::InitVkPresentInfoKHR();
            presentInfo.swapchainCount = 1;
            presentInfo.pSwapchains = &_swapchain;
            presentInfo.pImageIndices = &_imageIndex;
            presentInfo.waitSemaphoreCount = 1;
            presentInfo.pWaitSemaphores = &_renderCompleteSemaphores[_currentBufferIndex];

            return vkQueuePresentKHR(_graphicsQueue, &presentInfo);
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

        VkPresentModeKHR VulkanSwapchain::_ChoosePresentMode(const Vector<VkPresentModeKHR>& presentModes) const
        {
            if (presentModes.empty())
            {
                KMP_LOG_CRITICAL("unable to get available present mode");
                throw std::runtime_error("VulkanSwapchain: unable to get available present mode");
            }

            // TODO: add presentation option in a future, use FIFO by default right now
            //if (std::find(presentModes.begin(), presentModes.end(), VK_PRESENT_MODE_MAILBOX_KHR) != presentModes.end())
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

            UInt32 indicesArray[] = { _vulkanContext.graphicsFamilyIndex, _vulkanContext.presentFamilyIndex };
            if (_vulkanContext.graphicsFamilyIndex != _vulkanContext.presentFamilyIndex)
            {
                swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
                swapchainCreateInfo.queueFamilyIndexCount = 2;
                swapchainCreateInfo.pQueueFamilyIndices = indicesArray;
            }
            else
            {
                swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
            }

            swapchainCreateInfo.preTransform = _vulkanContext.surfaceCapabilities.currentTransform;
            swapchainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
            swapchainCreateInfo.presentMode = _ChoosePresentMode(_vulkanContext.presentModes);
            swapchainCreateInfo.clipped = VK_TRUE;
            swapchainCreateInfo.oldSwapchain = VK_NULL_HANDLE;

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
                _swapchainImageViews[i] = _imageCreatorDelegate.CreateImageView(_swapchainImages[i], _swapchainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1);
            }
        }
        //--------------------------------------------------------------------------

        void VulkanSwapchain::_CreateAttachmentImages()
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

            const auto sampleCount = _vulkanContext.supportedSampleCounts.top();

            VulkanUtils::ImageParameters creationParameters = {
                .width = _swapchainExtent.width,
                .height = _swapchainExtent.height,
                .mipLevels = 1,
                .numSamples = sampleCount,
                .format = _swapchainImageFormat,
                .tiling = VK_IMAGE_TILING_OPTIMAL,
                .usage = VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
                .memoryProperties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
            };
            _colorImage.reset(_imageCreatorDelegate.CreateImagePtr(creationParameters)); //TODO: need this?

            creationParameters = {
                .width = _swapchainExtent.width,
                .height = _swapchainExtent.height,
                .mipLevels = 1,
                .numSamples = sampleCount,
                .format = _vulkanContext.defaultDepthFormat,
                .tiling = VK_IMAGE_TILING_OPTIMAL,
                .usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
                .memoryProperties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
            };
            _depthImage.reset(_imageCreatorDelegate.CreateImagePtr(creationParameters)); //TODO: need this?
        }
        //--------------------------------------------------------------------------

        void VulkanSwapchain::_CreateAttachmentImagesViews()
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

            //TODO: need this?
            _colorImageView = _imageCreatorDelegate.CreateImageView(*_colorImage.get(), _swapchainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1);
            _depthImageView = _imageCreatorDelegate.CreateImageView(*_depthImage.get(), _vulkanContext.defaultDepthFormat, VK_IMAGE_ASPECT_DEPTH_BIT, 1);
        }
        //--------------------------------------------------------------------------
    }
}