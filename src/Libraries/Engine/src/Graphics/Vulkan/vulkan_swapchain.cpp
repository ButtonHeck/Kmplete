#include "Kmplete/Graphics/Vulkan/vulkan_swapchain.h"
#include "Kmplete/Graphics/Vulkan/vulkan_result_description.h"
#include "Kmplete/Graphics/Vulkan/vulkan_utils.h"
#include "Kmplete/Log/log.h"

#include <algorithm>
#include <stdexcept>


namespace Kmplete
{
    namespace Graphics
    {
        VulkanSwapchain::VulkanSwapchain(VkDevice device, VkSurfaceKHR surface, const PhysicalDeviceInfo& info, const VkExtent2D& swapchainExtent, const VulkanImageCreatorDelegate& imageCreatorDelegate)
            : Swapchain()
            , _device(device)
            , _physicalDeviceInfo(info)
            , _swapchainExtent(swapchainExtent)
            , _imageCreatorDelegate(imageCreatorDelegate)
            , _imageCount(0)
            , _swapchain(VK_NULL_HANDLE)
            , _swapchainImages()
            , _swapchainImageFormat(_physicalDeviceInfo.surfaceFormat.format)
            , _swapchainImageViews()
            , _colorImage(nullptr)
            , _colorImageView(VK_NULL_HANDLE)
            , _depthImage(nullptr)
            , _depthImageView(VK_NULL_HANDLE)
        {
            _imageCount = _physicalDeviceInfo.surfaceCapabilities.minImageCount + 1;
            if (_physicalDeviceInfo.surfaceCapabilities.maxImageCount > 0 && _imageCount > _physicalDeviceInfo.surfaceCapabilities.maxImageCount)
            {
                _imageCount = _physicalDeviceInfo.surfaceCapabilities.maxImageCount;
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
        //--------------------------------------------------------------------------

        UInt32 VulkanSwapchain::GetImageCount() const
        {
            return _imageCount;
        }
        //--------------------------------------------------------------------------

        VkPresentModeKHR VulkanSwapchain::_ChoosePresentMode(const Vector<VkPresentModeKHR>& presentModes) const
        {
            if (presentModes.empty())
            {
                KMP_LOG_CRITICAL("unable to get available present mode");
                throw std::runtime_error("VulkanSwapchain: unable to get available present mode");
            }

            if (std::find(presentModes.begin(), presentModes.end(), VK_PRESENT_MODE_MAILBOX_KHR) != presentModes.end())
            {
                return VK_PRESENT_MODE_MAILBOX_KHR;
            }

            return VK_PRESENT_MODE_FIFO_KHR;
        }
        //--------------------------------------------------------------------------

        void VulkanSwapchain::_CreateSwapchainObject(VkSurfaceKHR surface)
        {
            auto swapchainCreateInfo = VulkanUtils::GetVkSwapchainCreateInfoKHR();
            swapchainCreateInfo.surface = surface;
            swapchainCreateInfo.minImageCount = _imageCount;
            swapchainCreateInfo.imageFormat = _physicalDeviceInfo.surfaceFormat.format;
            swapchainCreateInfo.imageColorSpace = _physicalDeviceInfo.surfaceFormat.colorSpace;
            swapchainCreateInfo.imageExtent = _swapchainExtent;
            swapchainCreateInfo.imageArrayLayers = 1;
            swapchainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

            UInt32 indicesArray[] = { _physicalDeviceInfo.graphicsFamilyIndex, _physicalDeviceInfo.presentFamilyIndex };
            if (_physicalDeviceInfo.graphicsFamilyIndex != _physicalDeviceInfo.presentFamilyIndex)
            {
                swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
                swapchainCreateInfo.queueFamilyIndexCount = 2;
                swapchainCreateInfo.pQueueFamilyIndices = indicesArray;
            }
            else
            {
                swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
            }

            swapchainCreateInfo.preTransform = _physicalDeviceInfo.surfaceCapabilities.currentTransform;
            swapchainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
            swapchainCreateInfo.presentMode = _ChoosePresentMode(_physicalDeviceInfo.presentModes);
            swapchainCreateInfo.clipped = VK_TRUE;
            swapchainCreateInfo.oldSwapchain = VK_NULL_HANDLE;

            const auto result = vkCreateSwapchainKHR(_device, &swapchainCreateInfo, nullptr, &_swapchain);
            if (result != VK_SUCCESS)
            {
                const auto resultDescription = VkResultToString(result);
                KMP_LOG_CRITICAL("failed to create swapchain: {}", resultDescription);
                throw std::runtime_error(String("VulkanSwapchain: failed to create swapchain: ").append(resultDescription));
            }
        }
        //--------------------------------------------------------------------------

        void VulkanSwapchain::_CreateSwapchainImages()
        {
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
            _swapchainImageViews.resize(_swapchainImages.size());
            for (size_t i = 0; i < _swapchainImages.size(); i++)
            {
                _swapchainImageViews[i] = _imageCreatorDelegate.CreateImageView(_swapchainImages[i], _swapchainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1);
            }
        }
        //--------------------------------------------------------------------------

        void VulkanSwapchain::_CreateAttachmentImages()
        {
            const auto sampleCount = _physicalDeviceInfo.MaximumSupportedSampleCount();

            VulkanImage::Parameters creationParameters = {
                .width = _swapchainExtent.width,
                .height = _swapchainExtent.height,
                .mipLevels = 1,
                .numSamples = sampleCount,
                .format = _swapchainImageFormat,
                .tiling = VK_IMAGE_TILING_OPTIMAL,
                .usage = VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
                .memoryProperties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
            };
            _colorImage.reset(_imageCreatorDelegate.CreateImagePtr(creationParameters));

            creationParameters = {
                .width = _swapchainExtent.width,
                .height = _swapchainExtent.height,
                .mipLevels = 1,
                .numSamples = sampleCount,
                .format = _physicalDeviceInfo.defaultDepthFormat,
                .tiling = VK_IMAGE_TILING_OPTIMAL,
                .usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
                .memoryProperties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
            };
            _depthImage.reset(_imageCreatorDelegate.CreateImagePtr(creationParameters));
        }
        //--------------------------------------------------------------------------

        void VulkanSwapchain::_CreateAttachmentImagesViews()
        {
            _colorImageView = _imageCreatorDelegate.CreateImageView(*_colorImage.get(), _swapchainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1);
            _depthImageView = _imageCreatorDelegate.CreateImageView(*_depthImage.get(), _physicalDeviceInfo.defaultDepthFormat, VK_IMAGE_ASPECT_DEPTH_BIT, 1);
        }
        //--------------------------------------------------------------------------
    }
}