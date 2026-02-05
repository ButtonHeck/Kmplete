#include "Kmplete/Graphics/Vulkan/vulkan_swapchain.h"
#include "Kmplete/Log/log.h"

#include <algorithm>
#include <stdexcept>


namespace Kmplete
{
    namespace Graphics
    {
        VulkanSwapchain::VulkanSwapchain(const VkDevice& device, const VkSurfaceKHR& surface, const PhysicalDeviceImplementationInfo& info, const VkExtent2D& swapchainExtent, 
                                         const VkSurfaceFormatKHR& surfaceFormat)
            : Swapchain()
            , _device(device)
            , _surface(surface)
            , _physicalDeviceImplementationInfo(info)
            , _swapchainExtent(swapchainExtent)
            , _swapchain(VK_NULL_HANDLE)
            , _swapchainImages()
            , _swapchainImageFormat(surfaceFormat.format)
            , _swapchainImageViews()
            , _colorImage(VK_NULL_HANDLE)
            , _colorImageMemory(VK_NULL_HANDLE)
            , _colorImageView(VK_NULL_HANDLE)
            , _depthImage(VK_NULL_HANDLE)
            , _depthImageMemory(VK_NULL_HANDLE)
            , _depthImageView(VK_NULL_HANDLE)
        {
            const auto& swapchainDetails = _physicalDeviceImplementationInfo.surfaceAndPresentModeProperties;

            const auto presentMode = _ChoosePresentMode(swapchainDetails.presentModes);

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
            createInfo.imageExtent = _swapchainExtent;
            createInfo.imageArrayLayers = 1;
            createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

            const auto& queueFamiliesIndices = _physicalDeviceImplementationInfo.queueFamiliesIndices;
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

            _CreateImageViews();

            const auto samplesCount = _physicalDeviceImplementationInfo.physicalDeviceProperties.MaximumSupportedSampleCount();
            const auto depthFormat = _physicalDeviceImplementationInfo.physicalDeviceProperties.defaultDepthFormat;

            _CreateImage(_swapchainExtent.width, _swapchainExtent.height, 1, samplesCount, _swapchainImageFormat, VK_IMAGE_TILING_OPTIMAL,
                VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
                VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, _colorImage, _colorImageMemory, info);
            _colorImageView = _CreateImageView(_colorImage, _swapchainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1);

            _CreateImage(_swapchainExtent.width, _swapchainExtent.height, 1, samplesCount, depthFormat, VK_IMAGE_TILING_OPTIMAL,
                VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
                VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, _depthImage, _depthImageMemory, info);
            _depthImageView = _CreateImageView(_depthImage, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT, 1);
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

        void VulkanSwapchain::_CreateImage(UInt32 width, UInt32 height, UInt32 mipLevels, VkSampleCountFlagBits numSamples, VkFormat format, VkImageTiling tiling,
                                           VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory, const PhysicalDeviceImplementationInfo& info)
        {
            VkImageCreateInfo imageInfo{};
            imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
            imageInfo.imageType = VK_IMAGE_TYPE_2D;
            imageInfo.extent.width = width;
            imageInfo.extent.height = height;
            imageInfo.extent.depth = 1;
            imageInfo.mipLevels = mipLevels;
            imageInfo.arrayLayers = 1;
            imageInfo.format = format;
            imageInfo.tiling = tiling;
            imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            imageInfo.usage = usage;
            imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
            imageInfo.samples = numSamples;
            imageInfo.flags = 0;

            if (vkCreateImage(_device, &imageInfo, nullptr, &image) != VK_SUCCESS)
            {
                throw std::runtime_error("failed to create image");
            }

            VkMemoryRequirements memRequirements;
            vkGetImageMemoryRequirements(_device, image, &memRequirements);

            VkMemoryAllocateInfo allocInfo{};
            allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
            allocInfo.allocationSize = memRequirements.size;
            allocInfo.memoryTypeIndex = info.physicalDeviceProperties.FindMemoryType(memRequirements.memoryTypeBits, properties);

            if (vkAllocateMemory(_device, &allocInfo, nullptr, &imageMemory) != VK_SUCCESS)
            {
                KMP_LOG_CRITICAL("failed to allocate image memory");
                throw std::runtime_error("VulkanSwapchain: failed to allocate image memory");
            }

            vkBindImageMemory(_device, image, imageMemory, 0);
        }
        //--------------------------------------------------------------------------
    }
}