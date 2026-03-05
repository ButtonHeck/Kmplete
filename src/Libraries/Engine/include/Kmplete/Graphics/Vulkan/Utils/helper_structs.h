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
        }
    }
}