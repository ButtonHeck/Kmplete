#include "Kmplete/Graphics/Vulkan/Utils/function_utils.h"
#include "Kmplete/Graphics/Vulkan/Utils/initializers.h"
#include "Kmplete/Profile/profiler.h"


namespace Kmplete
{
    namespace Graphics
    {
        namespace VKUtils
        {
            void InsertImageMemoryBarrier(const MemoryBarrierParameters& barrierParameters) KMP_PROFILING(ProfileLevelImportant)
            {
                auto imageMemoryBarrier = InitVkImageMemoryBarrier();
                imageMemoryBarrier.srcAccessMask = barrierParameters.srcAccessMask;
                imageMemoryBarrier.dstAccessMask = barrierParameters.dstAccessMask;
                imageMemoryBarrier.oldLayout = barrierParameters.oldImageLayout;
                imageMemoryBarrier.newLayout = barrierParameters.newImageLayout;
                imageMemoryBarrier.image = barrierParameters.image;
                imageMemoryBarrier.subresourceRange = barrierParameters.subresourceRange;

                vkCmdPipelineBarrier(
                    barrierParameters.cmdbuffer,
                    barrierParameters.srcStageMask,
                    barrierParameters.dstStageMask,
                    0,
                    0, nullptr,
                    0, nullptr,
                    1, &imageMemoryBarrier);
            }}
            //--------------------------------------------------------------------------

            VkExtent3D Extent2Dto3D(const VkExtent2D& extent, UInt32 depth /*= 1*/)
            {
                return VkExtent3D{
                    .width = extent.width,
                    .height = extent.height,
                    .depth = depth
                };
            }
            //--------------------------------------------------------------------------

            VkExtent2D Extent3Dto2D(const VkExtent3D& extent)
            {
                return VkExtent2D{
                    .width = extent.width,
                    .height = extent.height
                };
            }
            //--------------------------------------------------------------------------
        }
    }
}