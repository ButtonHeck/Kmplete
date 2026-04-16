#include "Kmplete/Graphics/Vulkan/vulkan_texture.h"
#include "Kmplete/Graphics/Vulkan/Delegates/vulkan_image_creator_delegate.h"
#include "Kmplete/Graphics/Vulkan/Utils/function_utils.h"
#include "Kmplete/Graphics/Vulkan/Utils/result_description.h"
#include "Kmplete/Graphics/Vulkan/Utils/initializers.h"
#include "Kmplete/Graphics/Vulkan/Utils/presets.h"
#include "Kmplete/Log/log.h"
#include "Kmplete/Profile/profiler.h"


namespace Kmplete
{
    namespace Graphics
    {
        VulkanTexture::VulkanTexture(VkFormat format, UInt32 mipLevels, VkDevice device, VkCommandBuffer commandBuffer, const VulkanBuffer& stagingBuffer, 
                                     const VkExtent3D& extent, const VulkanImageCreatorDelegate& imageCreatorDelegate)
            : _logicalDevice(device)
            , _image(nullptr)
            , _imageView(VK_NULL_HANDLE)
        {
            KMP_PROFILE_FUNCTION(ProfileLevelAlways);

            _InitializeImage(format, mipLevels, extent, imageCreatorDelegate);
            _TransitionImageLayout(mipLevels, commandBuffer);
            _CopyStagingBufferToImage(stagingBuffer, extent, commandBuffer);
            _GenerateMipmaps(extent, mipLevels, commandBuffer);
            _InitializeImageView(mipLevels, imageCreatorDelegate);
        }
        //--------------------------------------------------------------------------

        VulkanTexture::~VulkanTexture() KMP_PROFILING(ProfileLevelAlways)
        {
            vkDestroyImageView(_logicalDevice, _imageView, nullptr);

            _image.reset();
        }}
        //--------------------------------------------------------------------------

        VkImageView VulkanTexture::GetVkImageView() const noexcept
        {
            return _imageView;
        }
        //--------------------------------------------------------------------------

        void VulkanTexture::_InitializeImage(VkFormat format, UInt32 mipLevels, const VkExtent3D& extent, const VulkanImageCreatorDelegate& imageCreatorDelegate) KMP_PROFILING(ProfileLevelImportant)
        {
            const auto creationParameters = VulkanPresets::GetImageCI_2D_OptimalTiling_QueueExclusive_Layer1_NoLayout(format, extent, mipLevels, VK_SAMPLE_COUNT_1_BIT, VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT);

            _image.reset(imageCreatorDelegate.CreateVulkanImagePtr(creationParameters, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT));
        }}
        //--------------------------------------------------------------------------

        void VulkanTexture::_TransitionImageLayout(UInt32 mipLevels, VkCommandBuffer commandBuffer) KMP_PROFILING(ProfileLevelImportant)
        {
            const VulkanUtils::MemoryBarrierParameters barrierParameters = {
                .cmdbuffer = commandBuffer,
                .image = _image->GetVkImage(),
                .srcAccessMask = VK_ACCESS_NONE,
                .dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT,
                .oldImageLayout = VK_IMAGE_LAYOUT_UNDEFINED,
                .newImageLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                .srcStageMask = VK_PIPELINE_STAGE_HOST_BIT,
                .dstStageMask = VK_PIPELINE_STAGE_TRANSFER_BIT,
                .subresourceRange = VkImageSubresourceRange{ VK_IMAGE_ASPECT_COLOR_BIT, 0, mipLevels, 0, 1 }
            };
            VulkanUtils::InsertImageMemoryBarrier(barrierParameters);
        }}
        //--------------------------------------------------------------------------

        void VulkanTexture::_CopyStagingBufferToImage(const VulkanBuffer& stagingBuffer, const VkExtent3D& extent, VkCommandBuffer commandBuffer) KMP_PROFILING(ProfileLevelImportant)
        {
            VkBufferImageCopy region{};
            region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            region.imageSubresource.layerCount = 1;
            region.imageExtent = extent;
            vkCmdCopyBufferToImage(commandBuffer, stagingBuffer.GetVkBuffer(), _image->GetVkImage(), VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);
        }}
        //--------------------------------------------------------------------------

        void VulkanTexture::_GenerateMipmaps(const VkExtent3D& extent, UInt32 mipLevels, VkCommandBuffer commandBuffer) KMP_PROFILING(ProfileLevelImportant)
        {
            auto vulkanImage = _image->GetVkImage();

            auto imageBarrier = VulkanUtils::InitVkImageMemoryBarrier();
            imageBarrier.image = vulkanImage;
            imageBarrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            imageBarrier.subresourceRange.baseArrayLayer = 0;
            imageBarrier.subresourceRange.layerCount = 1;
            imageBarrier.subresourceRange.levelCount = 1;

            Int32 mipWidth = extent.width;
            Int32 mipHeight = extent.height;

            for (UInt32 mip = 1; mip < mipLevels; mip++)
            {
                imageBarrier.subresourceRange.baseMipLevel = mip - 1;
                imageBarrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
                imageBarrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
                imageBarrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
                imageBarrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

                vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0,
                    0, nullptr,
                    0, nullptr,
                    1, &imageBarrier);

                VkImageBlit blit{};
                blit.srcOffsets[0] = { 0, 0, 0 };
                blit.srcOffsets[1] = { mipWidth, mipHeight, 1};
                blit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
                blit.srcSubresource.mipLevel = mip - 1;
                blit.srcSubresource.baseArrayLayer = 0;
                blit.srcSubresource.layerCount = 1;
                blit.dstOffsets[0] = { 0, 0, 0 };
                blit.dstOffsets[1] = { mipWidth > 1 ? mipWidth / 2 : 1, mipHeight > 1 ? mipHeight / 2 : 1, 1 };
                blit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
                blit.dstSubresource.mipLevel = mip;
                blit.dstSubresource.baseArrayLayer = 0;
                blit.dstSubresource.layerCount = 1;

                vkCmdBlitImage(commandBuffer, vulkanImage, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, vulkanImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &blit, VK_FILTER_LINEAR);

                imageBarrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
                imageBarrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                imageBarrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
                imageBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

                vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,
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
            }

            imageBarrier.subresourceRange.baseMipLevel = mipLevels - 1;
            imageBarrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
            imageBarrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            imageBarrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            imageBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

            vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,
                0, nullptr,
                0, nullptr,
                1, &imageBarrier);
        }}
        //--------------------------------------------------------------------------

        void VulkanTexture::_InitializeImageView(UInt32 mipLevels, const VulkanImageCreatorDelegate& imageCreatorDelegate) KMP_PROFILING(ProfileLevelImportant)
        {
            auto imageViewParameters = VulkanUtils::InitVkImageViewCreateInfo();
            imageViewParameters.image = _image->GetVkImage();
            imageViewParameters.viewType = VK_IMAGE_VIEW_TYPE_2D;
            imageViewParameters.format = _image->GetVkFormat();
            imageViewParameters.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            imageViewParameters.subresourceRange.baseMipLevel = 0;
            imageViewParameters.subresourceRange.levelCount = mipLevels;
            imageViewParameters.subresourceRange.baseArrayLayer = 0;
            imageViewParameters.subresourceRange.layerCount = 1;

            _imageView = imageCreatorDelegate.CreateVkImageView(imageViewParameters);
        }}
        //--------------------------------------------------------------------------
    }
}