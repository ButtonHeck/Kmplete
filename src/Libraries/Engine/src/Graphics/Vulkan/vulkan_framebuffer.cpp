#include "Kmplete/Graphics/Vulkan/vulkan_framebuffer.h"
#include "Kmplete/Log/log.h"


namespace Kmplete
{
    namespace Graphics
    {
        VulkanFramebuffer::VulkanFramebuffer(const VkDevice& device, const PhysicalDeviceProperties& properties, VkFormat colorFormat, VkFormat depthFormat, const VkExtent2D& swapchainExtent, UInt32 msaaSamples)
            : Framebuffer()
            , _device(device)
            , _properties(properties)
            , _colorFormat(colorFormat)
            , _depthFormat(depthFormat)
            , _colorImage(VK_NULL_HANDLE)
            , _colorImageMemory(VK_NULL_HANDLE)
            , _colorImageView(VK_NULL_HANDLE)
            , _depthImage(VK_NULL_HANDLE)
            , _depthImageMemory(VK_NULL_HANDLE)
            , _depthImageView(VK_NULL_HANDLE)
        {
            //TODO: extract to separate function
            auto msaaVulkanSamples = VK_SAMPLE_COUNT_1_BIT;
            if (msaaSamples == 64)
                msaaVulkanSamples = VK_SAMPLE_COUNT_64_BIT;
            else if (msaaSamples == 32)
                msaaVulkanSamples = VK_SAMPLE_COUNT_32_BIT;
            else if (msaaSamples == 16)
                msaaVulkanSamples = VK_SAMPLE_COUNT_16_BIT;
            else if (msaaSamples == 8)
                msaaVulkanSamples = VK_SAMPLE_COUNT_8_BIT;
            else if (msaaSamples == 4)
                msaaVulkanSamples = VK_SAMPLE_COUNT_4_BIT;
            else if (msaaSamples == 2)
                msaaVulkanSamples = VK_SAMPLE_COUNT_2_BIT;

            _CreateImage(swapchainExtent.width, swapchainExtent.height, 1, msaaVulkanSamples, _colorFormat, VK_IMAGE_TILING_OPTIMAL,
                         VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
                         VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, _colorImage, _colorImageMemory);
            _colorImageView = _CreateImageView(_colorImage, _colorFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1);

            _CreateImage(swapchainExtent.width, swapchainExtent.height, 1, msaaVulkanSamples, _depthFormat, VK_IMAGE_TILING_OPTIMAL,
                         VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
                         VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, _depthImage, _depthImageMemory);
            _depthImageView = _CreateImageView(_depthImage, _depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT, 1);

            //TODO: framebuffer object creation
        }
        //--------------------------------------------------------------------------

        VulkanFramebuffer::~VulkanFramebuffer()
        {
        }
        //--------------------------------------------------------------------------

        void VulkanFramebuffer::_CreateImage(UInt32 width, UInt32 height, UInt32 mipLevels, VkSampleCountFlagBits numSamples, VkFormat format, VkImageTiling tiling, 
                                             VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory)
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
            allocInfo.memoryTypeIndex = _FindMemoryType(memRequirements.memoryTypeBits, properties);

            if (vkAllocateMemory(_device, &allocInfo, nullptr, &imageMemory) != VK_SUCCESS)
            {
                throw std::runtime_error("failed to allocate image memory");
            }

            vkBindImageMemory(_device, image, imageMemory, 0);
        }
        //--------------------------------------------------------------------------

        //TODO: duplicate with swapchain
        VkImageView VulkanFramebuffer::_CreateImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, UInt32 mipLevels)
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
                throw std::runtime_error("VulkanFramebuffer: failed to create texture image view");
            }

            return imageView;
        }
        //--------------------------------------------------------------------------

        UInt32 VulkanFramebuffer::_FindMemoryType(UInt32 typeFilter, VkMemoryPropertyFlags properties)
        {
            const auto& memoryProperties = _properties.hardwareProperties.memoryProperties;

            for (UInt32 i = 0; i < memoryProperties.memoryTypeCount; i++)
            {
                if ((typeFilter & (1 << i)) && (memoryProperties.memoryTypes[i].propertyFlags & properties) == properties)
                {
                    return i;
                }
            }

            KMP_LOG_CRITICAL("failed to find suitable memory type");
            throw std::runtime_error("VulkanFramebuffer: failed to find suitable memory type");
        }
        //--------------------------------------------------------------------------
    }
}