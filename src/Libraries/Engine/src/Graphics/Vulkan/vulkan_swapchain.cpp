#include "Kmplete/Graphics/Vulkan/vulkan_swapchain.h"
#include "Kmplete/Window/window.h"
#include "Kmplete/Math/geometry.h"
#include "Kmplete/Math/math.h"
#include "Kmplete/Log/log.h"

#include <limits>
#include <algorithm>
#include <stdexcept>


namespace Kmplete
{
    namespace Graphics
    {
        VulkanSwapchain::VulkanSwapchain(const VkDevice& device, const VkSurfaceKHR& surface, const PhysicalDeviceProperties& properties, const Window& window)
            : Swapchain()
            , _device(device)
            , _surface(surface)
            , _properties(properties)
            , _window(window)
            , _swapchain(VK_NULL_HANDLE)
            , _swapchainImages()
            , _swapchainImageFormat()
            , _swapchainExtent()
            , _swapchainImageViews()
        {
            const auto& swapchainDetails = properties.swapChainSupportDetails;

            const auto surfaceFormat = _ChooseSurfaceFormat(swapchainDetails.surfaceFormats);
            const auto presentMode = _ChoosePresentMode(swapchainDetails.presentModes);
            const auto extent = _ChooseExtent(swapchainDetails.surfaceCapabilities);

            UInt32 imageCount = swapchainDetails.surfaceCapabilities.minImageCount + 1;
            if (swapchainDetails.surfaceCapabilities.maxImageCount > 0 && imageCount > swapchainDetails.surfaceCapabilities.maxImageCount)
            {
                imageCount = swapchainDetails.surfaceCapabilities.maxImageCount;
            }

            VkSwapchainCreateInfoKHR createInfo{};
            createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
            createInfo.surface = _surface;
            createInfo.minImageCount = imageCount;
            createInfo.imageFormat = surfaceFormat.format;
            createInfo.imageColorSpace = surfaceFormat.colorSpace;
            createInfo.imageExtent = extent;
            createInfo.imageArrayLayers = 1;
            createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

            const auto& queueFamiliesIndices = properties.queueFamiliesIndices;
            UInt32 indicesArray[] = { queueFamiliesIndices.graphicsFamilyIndex.value(), queueFamiliesIndices.presentFamilyIndex.value() };
            if (queueFamiliesIndices.graphicsFamilyIndex != queueFamiliesIndices.presentFamilyIndex)
            {
                createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
                createInfo.queueFamilyIndexCount = 2;
                createInfo.pQueueFamilyIndices = indicesArray;
            }
            else
            {
                createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
            }

            createInfo.preTransform = swapchainDetails.surfaceCapabilities.currentTransform;
            createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
            createInfo.presentMode = presentMode;
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

            _swapchainImageFormat = surfaceFormat.format;
            _swapchainExtent = extent;

            _CreateImageViews();
        }
        //--------------------------------------------------------------------------

        VulkanSwapchain::~VulkanSwapchain()
        {
            for (auto imageView : _swapchainImageViews)
            {
                vkDestroyImageView(_device, imageView, nullptr);
            }

            vkDestroySwapchainKHR(_device, _swapchain, nullptr);
        }
        //--------------------------------------------------------------------------

        VkSurfaceFormatKHR VulkanSwapchain::_ChooseSurfaceFormat(const Vector<VkSurfaceFormatKHR>& availableFormats) const
        {
            if (availableFormats.empty())
            {
                KMP_LOG_CRITICAL("unable to get available surface format");
                throw std::runtime_error("VulkanSwapchain: unable to get available surface format");
            }

            for (const auto& availableFormat : availableFormats)
            {
                if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
                {
                    return availableFormat;
                }
            }

            return availableFormats[0];
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

        VkExtent2D VulkanSwapchain::_ChooseExtent(const VkSurfaceCapabilitiesKHR& capabilities) const
        {
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

        void VulkanSwapchain::_CreateImageViews()
        {
            _swapchainImageViews.resize(_swapchainImages.size());
            for (size_t i = 0; i < _swapchainImages.size(); i++)
            {
                _swapchainImageViews[i] = _CreateImageView(_swapchainImages[i], _swapchainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1);
            }
        }
        //--------------------------------------------------------------------------

        VkImageView VulkanSwapchain::_CreateImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, UInt32 mipLevels)
        {
            VkImageViewCreateInfo viewInfo{};
            viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            viewInfo.image = image;
            viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
            viewInfo.format = format;
            viewInfo.subresourceRange.aspectMask = aspectFlags;
            viewInfo.subresourceRange.baseMipLevel = 0;
            viewInfo.subresourceRange.levelCount = mipLevels;
            viewInfo.subresourceRange.baseArrayLayer = 0;
            viewInfo.subresourceRange.layerCount = 1;

            VkImageView imageView;
            if (vkCreateImageView(_device, &viewInfo, nullptr, &imageView) != VK_SUCCESS)
            {
                KMP_LOG_CRITICAL("failed to create texture image view");
                throw std::runtime_error("VulkanSwapchain: failed to create texture image view");
            }

            return imageView;
        }
        //--------------------------------------------------------------------------
    }
}