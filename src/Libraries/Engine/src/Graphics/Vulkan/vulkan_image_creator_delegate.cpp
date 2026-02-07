#include "Kmplete/Graphics/Vulkan/vulkan_image_creator_delegate.h"
#include "Kmplete/Graphics/Vulkan/vulkan_result_description.h"
#include "Kmplete/Log/log.h"


namespace Kmplete
{
    namespace Graphics
    {
        VulkanImageCreatorDelegate::VulkanImageCreatorDelegate(const VkDevice& device, const PhysicalDeviceInfo& physicalDeviceInfo)
            : _device(device)
            , _physicalDeviceInfo(physicalDeviceInfo)
        {}
        //--------------------------------------------------------------------------

        VkImage VulkanImageCreatorDelegate::CreateImage(const VkExtent2D & extent, UInt32 mipLevels, VkSampleCountFlagBits numSamples, VkFormat format, VkImageTiling tiling, 
                                                        VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkDeviceMemory& imageMemory) const
        {
            return CreateImage(extent.width, extent.height, mipLevels, numSamples, format, tiling, usage, properties, imageMemory);
        }
        //--------------------------------------------------------------------------

        VkImage VulkanImageCreatorDelegate::CreateImage(UInt32 width, UInt32 height, UInt32 mipLevels, VkSampleCountFlagBits numSamples, VkFormat format, VkImageTiling tiling, 
                                                        VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkDeviceMemory& imageMemory) const
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

            VkImage image{};
            auto result = vkCreateImage(_device, &imageInfo, nullptr, &image);
            if (result != VK_SUCCESS)
            {
                const auto resultDescription = VkResultToString(result);
                KMP_LOG_CRITICAL("failed to create image: {}", resultDescription);
                throw std::runtime_error(String("VulkanImageCreatorDelegate: failed to create image: ").append(resultDescription));
            }

            VkMemoryRequirements memRequirements;
            vkGetImageMemoryRequirements(_device, image, &memRequirements);

            VkMemoryAllocateInfo allocInfo{};
            allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
            allocInfo.allocationSize = memRequirements.size;
            allocInfo.memoryTypeIndex = _physicalDeviceInfo.FindMemoryType(memRequirements.memoryTypeBits, properties);

            result = vkAllocateMemory(_device, &allocInfo, nullptr, &imageMemory);
            if (result != VK_SUCCESS)
            {
                const auto resultDescription = VkResultToString(result);
                KMP_LOG_CRITICAL("failed to allocate image memory: {}", resultDescription);
                throw std::runtime_error(String("VulkanImageCreatorDelegate: failed to allocate image memory: ").append(resultDescription));
            }

            vkBindImageMemory(_device, image, imageMemory, 0);

            return image;
        }
        //--------------------------------------------------------------------------

        VkImageView VulkanImageCreatorDelegate::CreateImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, UInt32 mipLevels) const
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
            const auto result = vkCreateImageView(_device, &viewInfo, nullptr, &imageView);
            if (result != VK_SUCCESS)
            {
                const auto resultDescription = VkResultToString(result);
                KMP_LOG_CRITICAL("failed to create texture image view: {}", resultDescription);
                throw std::runtime_error(String("VulkanImageCreatorDelegate: failed to create texture image view: ").append(resultDescription));
            }

            return imageView;
        }
        //--------------------------------------------------------------------------
    }
}