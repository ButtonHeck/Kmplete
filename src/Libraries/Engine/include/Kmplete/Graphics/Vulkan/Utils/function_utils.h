#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"

#include <vulkan/vulkan.h>


namespace Kmplete
{
    namespace Graphics
    {
        class VulkanBuffer;


        namespace VKUtils
        {
            // TODO: comments
            struct MemoryBarrierParameters
            {
                VkCommandBuffer cmdbuffer;
                VkImage image;
                VkAccessFlags srcAccessMask;
                VkAccessFlags dstAccessMask;
                VkImageLayout oldImageLayout;
                VkImageLayout newImageLayout;
                VkPipelineStageFlags srcStageMask;
                VkPipelineStageFlags dstStageMask;
                VkImageSubresourceRange subresourceRange;
            };
            //--------------------------------------------------------------------------


            //TODO: comments
            struct BufferCopyParameters
            {
                VulkanBuffer& destinationBuffer;
                VkDeviceSize srcOfset;
                VkDeviceSize dstOfset;
                VkDeviceSize size;
            };
            //--------------------------------------------------------------------------

            KMP_API void InsertImageMemoryBarrier(const MemoryBarrierParameters& barrierParameters);

            KMP_NODISCARD KMP_API VkImageViewType ImageTypeToViewType(VkImageType imageType, bool array = false) noexcept;

            KMP_NODISCARD KMP_API VkExtent3D Extent2Dto3D(const VkExtent2D& extent, UInt32 depth = 1);
            KMP_NODISCARD KMP_API VkExtent2D Extent3Dto2D(const VkExtent3D& extent);
        }
    }
}


inline bool operator==(const VkExtent2D& extentA, const VkExtent2D& extentB)
{
    return extentA.width == extentB.width &&
           extentA.height == extentB.height;
}
//--------------------------------------------------------------------------

inline bool operator==(const VkExtent3D& extentA, const VkExtent3D& extentB)
{
    return extentA.width == extentB.width &&
           extentA.height == extentB.height &&
           extentA.depth == extentB.depth;
}
//--------------------------------------------------------------------------