#include "Kmplete/Graphics/Vulkan/vulkan_texture.h"
#include "Kmplete/Graphics/Vulkan/Delegates/vulkan_image_creator_delegate.h"
#include "Kmplete/Graphics/Vulkan/Delegates/vulkan_memory_type_delegate.h"
#include "Kmplete/Graphics/Vulkan/Delegates/vulkan_format_delegate.h"
#include "Kmplete/Graphics/Vulkan/Utils/helper_structs.h"
#include "Kmplete/Graphics/Vulkan/Utils/function_utils.h"
#include "Kmplete/Graphics/Vulkan/Utils/initializers.h"
#include "Kmplete/Graphics/image.h"
#include "Kmplete/Log/log.h"
#include "Kmplete/Profile/profiler.h"

#include <stdexcept>


namespace Kmplete
{
    namespace Graphics
    {
        VulkanTexture::VulkanTexture(VkDevice device, VkQueue graphicsQueue, const Image& image, const VulkanImageCreatorDelegate& imageCreatorDelegate, 
                                     const VulkanMemoryTypeDelegate& memoryTypeDelegate, VkCommandPool commandPool, const VulkanFormatDelegate& formatDelegate)
            : _logicalDevice(device)
            , _image(nullptr)
            , _imageView(VK_NULL_HANDLE)
            , _sampler(VK_NULL_HANDLE)
        {
            KMP_PROFILE_FUNCTION(ProfileLevelAlways);

            _InitializeImage(image, imageCreatorDelegate);
            _TransitionImageLayout(image.GetMipLevels(), commandPool, graphicsQueue);
            _CopyStagingBufferToImage(memoryTypeDelegate, image, commandPool, graphicsQueue);
            _GenerateMipmaps(image, formatDelegate, commandPool, graphicsQueue);
            _InitializeImageView(image, imageCreatorDelegate);
            _InitializeSampler(image, imageCreatorDelegate);
        }
        //--------------------------------------------------------------------------

        VulkanTexture::~VulkanTexture()
        {
            KMP_PROFILE_FUNCTION(ProfileLevelAlways);

            vkDestroySampler(_logicalDevice, _sampler, nullptr);
            vkDestroyImageView(_logicalDevice, _imageView, nullptr);

            _image.reset();
        }
        //--------------------------------------------------------------------------

        VkImageView VulkanTexture::GetVkImageView() const noexcept
        {
            return _imageView;
        }
        //--------------------------------------------------------------------------

        VkSampler VulkanTexture::GetVkSampler() const noexcept
        {
            return _sampler;
        }
        //--------------------------------------------------------------------------

        void VulkanTexture::_InitializeImage(const Image& image, const VulkanImageCreatorDelegate& imageCreatorDelegate)
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

            VulkanUtils::ImageParameters creationParameters = {
                .width = UInt32(image.GetWidth()),
                .height = UInt32(image.GetHeight()),
                .mipLevels = image.GetMipLevels(),
                .numSamples = VK_SAMPLE_COUNT_1_BIT,
                .format = VK_FORMAT_R8G8B8A8_UNORM,
                .tiling = VK_IMAGE_TILING_OPTIMAL,
                .usage = VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
                .memoryProperties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
            };
            _image.reset(imageCreatorDelegate.CreateImagePtr(creationParameters));
        }
        //--------------------------------------------------------------------------

        void VulkanTexture::_TransitionImageLayout(UInt32 mipLevels, VkCommandPool commandPool, VkQueue graphicsQueue)
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

            auto transitionCommandBuffer = VulkanUtils::StartSingleTimeCommandBuffer(_logicalDevice, commandPool);

            VulkanUtils::InsertImageMemoryBarrier(
                transitionCommandBuffer,
                _image->GetVkImage(),
                VK_ACCESS_NONE,
                VK_ACCESS_TRANSFER_WRITE_BIT,
                VK_IMAGE_LAYOUT_UNDEFINED,
                VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                VK_PIPELINE_STAGE_HOST_BIT,
                VK_PIPELINE_STAGE_TRANSFER_BIT,
                VkImageSubresourceRange{ VK_IMAGE_ASPECT_COLOR_BIT, 0, mipLevels, 0, 1 }
            );

            VulkanUtils::EndSingleTimeCommandBuffer(_logicalDevice, transitionCommandBuffer, commandPool, graphicsQueue);
        }
        //--------------------------------------------------------------------------

        void VulkanTexture::_CopyStagingBufferToImage(const VulkanMemoryTypeDelegate& memoryTypeDelegate, const Image& image, VkCommandPool commandPool, VkQueue graphicsQueue)
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

            auto stagingBuffer = _InitializeStagingBuffer(memoryTypeDelegate, image);

            auto copyCommandBuffer = VulkanUtils::StartSingleTimeCommandBuffer(_logicalDevice, commandPool);

            VkBufferImageCopy region{};
            region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            region.imageSubresource.layerCount = 1;
            region.imageExtent.width = UInt32(image.GetWidth());
            region.imageExtent.height = UInt32(image.GetHeight());
            region.imageExtent.depth = 1;
            vkCmdCopyBufferToImage(copyCommandBuffer, stagingBuffer->GetVkBuffer(), _image->GetVkImage(), VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

            VulkanUtils::EndSingleTimeCommandBuffer(_logicalDevice, copyCommandBuffer, commandPool, graphicsQueue);
        }
        //--------------------------------------------------------------------------

        UPtr<VulkanBuffer> VulkanTexture::_InitializeStagingBuffer(const VulkanMemoryTypeDelegate& memoryTypeDelegate, const Image& image)
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

            auto buffer = CreateUPtr<VulkanBuffer>(memoryTypeDelegate, _logicalDevice, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, image.GetDataSize());

            if (!buffer)
            {
                KMP_LOG_ERROR("failed to create texture buffer");
                throw std::runtime_error("VulkanTexture: failed to create texture buffer");
            }

            auto result = buffer->Map();
            VulkanUtils::CheckResult(result, "VulkanTexture: failed to map texture buffer");

            buffer->CopyToMappedMemory(image.GetPixels(), image.GetDataSize());

            result = buffer->Flush();
            VulkanUtils::CheckResult(result, "VulkanTexture: failed to flush texture buffer");

            buffer->Unmap();

            return buffer;
        }
        //--------------------------------------------------------------------------

        void VulkanTexture::_GenerateMipmaps(const Image& image, const VulkanFormatDelegate& formatDelegate, VkCommandPool commandPool, VkQueue graphicsQueue)
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

            const auto formatProperties = formatDelegate.GetFormatProperties(VK_FORMAT_R8G8B8A8_UNORM);
            if (!(formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT))
            {
                KMP_LOG_ERROR("image format does not support linear blitting");
                throw std::runtime_error("VulkanTexture: image format does not support linear blitting");
            }

            auto commandBuffer = VulkanUtils::StartSingleTimeCommandBuffer(_logicalDevice, commandPool);
            auto vulkanImage = _image->GetVkImage();

            auto imageBarrier = VulkanUtils::InitVkImageMemoryBarrier();
            imageBarrier.image = vulkanImage;
            imageBarrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            imageBarrier.subresourceRange.baseArrayLayer = 0;
            imageBarrier.subresourceRange.layerCount = 1;
            imageBarrier.subresourceRange.levelCount = 1;

            Int32 mipWidth = Int32(image.GetWidth());
            Int32 mipHeight = Int32(image.GetHeight());

            for (UInt32 mip = 1; mip < image.GetMipLevels(); mip++)
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

            imageBarrier.subresourceRange.baseMipLevel = image.GetMipLevels() - 1;
            imageBarrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
            imageBarrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            imageBarrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            imageBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

            vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,
                0, nullptr,
                0, nullptr,
                1, &imageBarrier);

            VulkanUtils::EndSingleTimeCommandBuffer(_logicalDevice, commandBuffer, commandPool, graphicsQueue);
        }
        //--------------------------------------------------------------------------

        void VulkanTexture::_InitializeImageView(const Image& image, const VulkanImageCreatorDelegate& imageCreatorDelegate)
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

            auto imageViewParameters = VulkanUtils::InitVkImageViewCreateInfo();
            imageViewParameters.image = _image->GetVkImage();
            imageViewParameters.viewType = VK_IMAGE_VIEW_TYPE_2D;
            imageViewParameters.format = VK_FORMAT_R8G8B8A8_UNORM;
            imageViewParameters.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            imageViewParameters.subresourceRange.baseMipLevel = 0;
            imageViewParameters.subresourceRange.levelCount = image.GetMipLevels();
            imageViewParameters.subresourceRange.baseArrayLayer = 0;
            imageViewParameters.subresourceRange.layerCount = 1;

            _imageView = imageCreatorDelegate.CreateImageView(imageViewParameters);
        }
        //--------------------------------------------------------------------------

        void VulkanTexture::_InitializeSampler(const Image& image, const VulkanImageCreatorDelegate& imageCreatorDelegate)
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

            auto samplerParameters = VulkanUtils::InitVkSamplerCreateInfo();
            samplerParameters.magFilter = VK_FILTER_LINEAR;
            samplerParameters.minFilter = VK_FILTER_LINEAR;
            samplerParameters.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
            samplerParameters.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
            samplerParameters.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
            samplerParameters.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
            samplerParameters.minLod = 0.0f;
            samplerParameters.maxLod = float(image.GetMipLevels());
            samplerParameters.maxAnisotropy = 1.0f;

            _sampler = imageCreatorDelegate.CreateSampler(samplerParameters);
        }
        //--------------------------------------------------------------------------
    }
}