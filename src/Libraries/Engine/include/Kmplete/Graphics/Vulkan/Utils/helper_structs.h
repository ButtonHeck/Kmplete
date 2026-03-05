#pragma once

#include "Kmplete/Base/types_aliases.h"

#include <vulkan/vulkan.h>


namespace Kmplete
{
    namespace Graphics
    {
        namespace VulkanUtils
        {
            //TODO: comments
            struct ImageParameters
            {
                UInt32 width;
                UInt32 height;
                UInt32 mipLevels;
                VkSampleCountFlagBits numSamples;
                VkFormat format;
                VkImageTiling tiling;
                VkImageUsageFlags usage;
                VkMemoryPropertyFlags memoryProperties;
            };
            //--------------------------------------------------------------------------


            //TODO: comments
            struct ImageViewParameters
            {
                VkImage image;
                VkImageViewType viewType;
                VkFormat format;
                VkImageAspectFlags aspectFlags;
                UInt32 baseMipLevel;
                UInt32 mipLevels;
                UInt32 baseArrayLayer;
                UInt32 layers;
            };
            //--------------------------------------------------------------------------


            //TODO: comments
            struct SamplerParameters
            {
                VkFilter magnificationFilter;
                VkFilter minificationFilter;
                VkSamplerMipmapMode mipmapMode;
                VkSamplerAddressMode addressModeU;
                VkSamplerAddressMode addressModeV;
                VkSamplerAddressMode addressModeW;
                float minLod;
                float maxLod;
                float maxAnisotropy;
            };
            //--------------------------------------------------------------------------
        }
    }
}