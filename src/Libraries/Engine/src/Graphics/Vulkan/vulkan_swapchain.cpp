#include "Kmplete/Graphics/Vulkan/vulkan_swapchain.h"
#include "Kmplete/Log/log.h"

#include <algorithm>
#include <stdexcept>


namespace Kmplete
{
    namespace Graphics
    {
        VulkanSwapchain::VulkanSwapchain(const VkDevice& device, const VkSurfaceKHR& surface, const PhysicalDeviceInfo& info, const VkExtent2D& swapchainExtent, const VulkanImageCreatorDelegate& imageCreatorDelegate)
            : Swapchain()
            , _device(device)
            , _physicalDeviceInfo(info)
            , _swapchainExtent(swapchainExtent)
            , _imageCreatorDelegate(imageCreatorDelegate)
            , _swapchain(VK_NULL_HANDLE)
            , _swapchainImages()
            , _swapchainImageFormat(_physicalDeviceInfo.surfaceFormat.format)
            , _swapchainImageViews()
            , _colorImage(VK_NULL_HANDLE)
            , _colorImageMemory(VK_NULL_HANDLE)
            , _colorImageView(VK_NULL_HANDLE)
            , _depthImage(VK_NULL_HANDLE)
            , _depthImageMemory(VK_NULL_HANDLE)
            , _depthImageView(VK_NULL_HANDLE)
        {
            UInt32 imageCount = _physicalDeviceInfo.surfaceCapabilities.minImageCount + 1;
            if (_physicalDeviceInfo.surfaceCapabilities.maxImageCount > 0 && imageCount > _physicalDeviceInfo.surfaceCapabilities.maxImageCount)
            {
                imageCount = _physicalDeviceInfo.surfaceCapabilities.maxImageCount;
            }

            VkSwapchainCreateInfoKHR createInfo{};
            createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
            createInfo.surface = surface;
            createInfo.minImageCount = imageCount;
            createInfo.imageFormat = _physicalDeviceInfo.surfaceFormat.format;
            createInfo.imageColorSpace = _physicalDeviceInfo.surfaceFormat.colorSpace;
            createInfo.imageExtent = _swapchainExtent;
            createInfo.imageArrayLayers = 1;
            createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

            UInt32 indicesArray[] = { _physicalDeviceInfo.graphicsFamilyIndex, _physicalDeviceInfo.presentFamilyIndex };
            if (_physicalDeviceInfo.graphicsFamilyIndex != _physicalDeviceInfo.presentFamilyIndex)
            {
                createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
                createInfo.queueFamilyIndexCount = 2;
                createInfo.pQueueFamilyIndices = indicesArray;
            }
            else
            {
                createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
            }

            createInfo.preTransform = _physicalDeviceInfo.surfaceCapabilities.currentTransform;
            createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
            createInfo.presentMode = _ChoosePresentMode(_physicalDeviceInfo.presentModes);
            createInfo.clipped = VK_TRUE;
            createInfo.oldSwapchain = VK_NULL_HANDLE;

            if (vkCreateSwapchainKHR(_device, &createInfo, nullptr, &_swapchain) != VK_SUCCESS)
            {
                KMP_LOG_CRITICAL("failed to create swapchain");
                throw std::runtime_error("VulkanSwapchain: failed to create swapchain");
            }

            vkGetSwapchainImagesKHR(_device, _swapchain, &imageCount, nullptr);
            _swapchainImages.resize(imageCount);
            vkGetSwapchainImagesKHR(_device, _swapchain, &imageCount, _swapchainImages.data());

            for (const auto& swapchainImage : _swapchainImages)
            {
                if (swapchainImage == VK_NULL_HANDLE)
                {
                    KMP_LOG_CRITICAL("one of the swapchain images is invalid");
                    throw std::runtime_error("VulkanSwapchain: one of the swapchain images is invalid");
                }
            }

            _CreateImageViews();

            const auto samplesCount = _physicalDeviceInfo.MaximumSupportedSampleCount();
            const auto depthFormat = _physicalDeviceInfo.defaultDepthFormat;

            _colorImage = _imageCreatorDelegate.CreateImage(_swapchainExtent.width, _swapchainExtent.height, 1, samplesCount, _swapchainImageFormat, VK_IMAGE_TILING_OPTIMAL,
                VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
                VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, _colorImageMemory, info);
            _colorImageView = _imageCreatorDelegate.CreateImageView(_colorImage, _swapchainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1);

            _depthImage = _imageCreatorDelegate.CreateImage(_swapchainExtent.width, _swapchainExtent.height, 1, samplesCount, depthFormat, VK_IMAGE_TILING_OPTIMAL,
                VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
                VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, _depthImageMemory, info);
            _depthImageView = _imageCreatorDelegate.CreateImageView(_depthImage, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT, 1);
        }
        //--------------------------------------------------------------------------

        VulkanSwapchain::~VulkanSwapchain()
        {
            vkDestroyImageView(_device, _colorImageView, nullptr);
            vkDestroyImage(_device, _colorImage, nullptr);
            vkFreeMemory(_device, _colorImageMemory, nullptr);

            vkDestroyImageView(_device, _depthImageView, nullptr);
            vkDestroyImage(_device, _depthImage, nullptr);
            vkFreeMemory(_device, _depthImageMemory, nullptr);

            for (auto imageView : _swapchainImageViews)
            {
                vkDestroyImageView(_device, imageView, nullptr);
            }

            vkDestroySwapchainKHR(_device, _swapchain, nullptr);
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

        void VulkanSwapchain::_CreateImageViews()
        {
            _swapchainImageViews.resize(_swapchainImages.size());
            for (size_t i = 0; i < _swapchainImages.size(); i++)
            {
                _swapchainImageViews[i] = _imageCreatorDelegate.CreateImageView(_swapchainImages[i], _swapchainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1);
            }
        }
        //--------------------------------------------------------------------------
    }
}