#include "Kmplete/Graphics/Vulkan/vulkan_texture.h"
#include "Kmplete/Graphics/Vulkan/vulkan_graphics_base.h"
#include "Kmplete/Graphics/Vulkan/Utils/function_utils.h"
#include "Kmplete/Graphics/Vulkan/Utils/result_description.h"
#include "Kmplete/Graphics/Vulkan/Utils/initializers.h"
#include "Kmplete/Graphics/Vulkan/Utils/presets.h"
#include "Kmplete/Graphics/Vulkan/Utils/bits_aliases.h"
#include "Kmplete/Core/assertion.h"
#include "Kmplete/Log/log.h"
#include "Kmplete/Profile/profiler.h"


namespace Kmplete
{
    namespace Graphics
    {
        using namespace VKBits;


        VulkanTexture::VulkanTexture(VkImageType imageType, VkFormat format, UInt32 mipLevels, VkDevice device, VkCommandBuffer commandBuffer, 
                                     const VulkanBuffer& stagingBuffer, const VkExtent3D& extent, const VulkanImageCreatorDelegate& imageCreatorDelegate)
            : VulkanTextureBase(device, 
                VKPresets::GetImageCI_OptimalTiling_QueueExclusive_Layer1_NoLayout(imageType, format, extent, mipLevels, VK_SampleCount_1, VK_ImageUsage_TransferSrcAndDst | VK_ImageUsage_Sampled),
                VKPresets::GetImageViewCI_BaseMip0_BaseArray0_SingleLayer(VKUtils::ImageTypeToViewType(imageType), VK_ImageAspect_Color, mipLevels),
                imageCreatorDelegate, 
                VK_Memory_DeviceLocal)
        {
            KMP_PROFILE_FUNCTION(ProfileLevelAlways);

            _TransitionImageLayout(mipLevels, commandBuffer);
            _CopyStagingBufferToImage(stagingBuffer, extent, commandBuffer);
            _GenerateMipmaps(extent, mipLevels, commandBuffer);
        }
        //--------------------------------------------------------------------------

        void VulkanTexture::_TransitionImageLayout(UInt32 mipLevels, VkCommandBuffer commandBuffer) KMP_PROFILING(ProfileLevelImportant)
        {
            KMP_ASSERT(_image && commandBuffer);

            const VKUtils::MemoryBarrierParameters barrierParameters = {
                .cmdbuffer = commandBuffer,
                .image = _image->GetVkImage(),
                .srcAccessMask = VK_Access_None,
                .dstAccessMask = VK_Access_TransferWrite,
                .oldImageLayout = VK_ImageLayout_Undefined,
                .newImageLayout = VK_ImageLayout_TransferDstOptimal,
                .srcStageMask = VK_PipelineStage_Host,
                .dstStageMask = VK_PipelineStage_Transfer,
                .subresourceRange = VkImageSubresourceRange{ VK_ImageAspect_Color, 0, mipLevels, 0, 1 }
            };
            VKUtils::InsertImageMemoryBarrier(barrierParameters);
        }}
        //--------------------------------------------------------------------------

        void VulkanTexture::_CopyStagingBufferToImage(const VulkanBuffer& stagingBuffer, const VkExtent3D& extent, VkCommandBuffer commandBuffer) KMP_PROFILING(ProfileLevelImportant)
        {
            KMP_ASSERT(_image && commandBuffer);

            VkBufferImageCopy region{};
            region.imageSubresource.aspectMask = VK_ImageAspect_Color;
            region.imageSubresource.layerCount = 1;
            region.imageExtent = extent;
            vkCmdCopyBufferToImage(commandBuffer, stagingBuffer.GetVkBuffer(), _image->GetVkImage(), VK_ImageLayout_TransferDstOptimal, 1, &region);
        }}
        //--------------------------------------------------------------------------

        void VulkanTexture::_GenerateMipmaps(const VkExtent3D& extent, UInt32 mipLevels, VkCommandBuffer commandBuffer) KMP_PROFILING(ProfileLevelImportant)
        {
            KMP_ASSERT(_image && commandBuffer);

            auto vulkanImage = _image->GetVkImage();
            KMP_ASSERT(vulkanImage);

            auto imageBarrier = VKUtils::InitVkImageMemoryBarrier();
            imageBarrier.image = vulkanImage;
            imageBarrier.subresourceRange.aspectMask = VK_ImageAspect_Color;
            imageBarrier.subresourceRange.baseArrayLayer = 0;
            imageBarrier.subresourceRange.layerCount = 1;
            imageBarrier.subresourceRange.levelCount = 1;

            Int32 mipWidth = extent.width;
            Int32 mipHeight = extent.height;

            for (UInt32 mip = 1; mip < mipLevels; mip++)
            {
                _GenerateMipmapLevel(imageBarrier, mip, mipWidth, mipHeight, vulkanImage, commandBuffer);
            }

            imageBarrier.subresourceRange.baseMipLevel = mipLevels - 1;
            imageBarrier.oldLayout = VK_ImageLayout_TransferDstOptimal;
            imageBarrier.newLayout = VK_ImageLayout_ShaderReadOnlyOptimal;
            imageBarrier.srcAccessMask = VK_Access_TransferWrite;
            imageBarrier.dstAccessMask = VK_Access_ShaderRead;

            vkCmdPipelineBarrier(commandBuffer, VK_PipelineStage_Transfer, VK_PipelineStage_FragmentShader, 0,
                0, nullptr,
                0, nullptr,
                1, &imageBarrier);
        }}
        //--------------------------------------------------------------------------

        void VulkanTexture::_GenerateMipmapLevel(VkImageMemoryBarrier& imageBarrier, UInt32 mipLevel, Int32& mipWidth, Int32& mipHeight, VkImage image, VkCommandBuffer commandBuffer) KMP_PROFILING(ProfileLevelImportantVerbose)
        {
            imageBarrier.subresourceRange.baseMipLevel = mipLevel - 1;
            imageBarrier.oldLayout = VK_ImageLayout_TransferDstOptimal;
            imageBarrier.newLayout = VK_ImageLayout_TransferSrcOptimal;
            imageBarrier.srcAccessMask = VK_Access_TransferWrite;
            imageBarrier.dstAccessMask = VK_Access_TransferRead;

            vkCmdPipelineBarrier(commandBuffer, VK_PipelineStage_Transfer, VK_PipelineStage_Transfer, 0,
                0, nullptr,
                0, nullptr,
                1, &imageBarrier);

            VkImageBlit blit{};
            blit.srcOffsets[0] = { 0, 0, 0 };
            blit.srcOffsets[1] = { mipWidth, mipHeight, 1 };
            blit.srcSubresource.aspectMask = VK_ImageAspect_Color;
            blit.srcSubresource.mipLevel = mipLevel - 1;
            blit.srcSubresource.baseArrayLayer = 0;
            blit.srcSubresource.layerCount = 1;
            blit.dstOffsets[0] = { 0, 0, 0 };
            blit.dstOffsets[1] = { mipWidth > 1 ? mipWidth / 2 : 1, mipHeight > 1 ? mipHeight / 2 : 1, 1 };
            blit.dstSubresource.aspectMask = VK_ImageAspect_Color;
            blit.dstSubresource.mipLevel = mipLevel;
            blit.dstSubresource.baseArrayLayer = 0;
            blit.dstSubresource.layerCount = 1;

            vkCmdBlitImage(commandBuffer, image, VK_ImageLayout_TransferSrcOptimal, image, VK_ImageLayout_TransferDstOptimal, 1, &blit, VK_Filter_Linear);

            imageBarrier.oldLayout = VK_ImageLayout_TransferSrcOptimal;
            imageBarrier.newLayout = VK_ImageLayout_ShaderReadOnlyOptimal;
            imageBarrier.srcAccessMask = VK_Access_TransferRead;
            imageBarrier.dstAccessMask = VK_Access_ShaderRead;

            vkCmdPipelineBarrier(commandBuffer, VK_PipelineStage_Transfer, VK_PipelineStage_FragmentShader, 0,
                0, nullptr,
                0, nullptr,
                1, &imageBarrier);

            if (mipWidth > 1)
            {
                mipWidth /= 2;
            }
            if (mipHeight > 1)
            {
                mipHeight /= 2;
            }
        }}
        //--------------------------------------------------------------------------
    }
}