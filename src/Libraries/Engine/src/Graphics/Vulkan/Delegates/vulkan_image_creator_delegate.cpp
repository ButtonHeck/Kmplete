#include "Kmplete/Graphics/Vulkan/Delegates/vulkan_image_creator_delegate.h"
#include "Kmplete/Graphics/Vulkan/Utils/initializers.h"
#include "Kmplete/Graphics/Vulkan/Utils/function_utils.h"
#include "Kmplete/Log/log.h"
#include "Kmplete/Profile/profiler.h"


namespace Kmplete
{
    namespace Graphics
    {
        VulkanImageCreatorDelegate::VulkanImageCreatorDelegate(VkDevice device, const VulkanMemoryTypeDelegate& memoryTypeDelegate)
            : _memoryTypeDelegate(memoryTypeDelegate)
            , _device(device)
        {}
        //--------------------------------------------------------------------------

        VulkanImage VulkanImageCreatorDelegate::CreateImage(const VkExtent2D & extent, UInt32 mipLevels, VkSampleCountFlagBits numSamples, VkFormat format, 
                                                            VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags memoryProperties) const
        {
            return CreateImage(extent.width, extent.height, mipLevels, numSamples, format, tiling, usage, memoryProperties);
        }
        //--------------------------------------------------------------------------

        VulkanImage VulkanImageCreatorDelegate::CreateImage(UInt32 width, UInt32 height, UInt32 mipLevels, VkSampleCountFlagBits numSamples, VkFormat format, 
                                                            VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags memoryProperties) const
        {
            const VulkanUtils::ImageParameters creationParameters = {
                .width = width,
                .height = height,
                .mipLevels = mipLevels,
                .numSamples = numSamples,
                .format = format,
                .tiling = tiling,
                .usage = usage,
                .memoryProperties = memoryProperties
            };

            return CreateImage(creationParameters);
        }
        //--------------------------------------------------------------------------

        VulkanImage VulkanImageCreatorDelegate::CreateImage(const VulkanUtils::ImageParameters& creationParameters) const
        {
            KMP_PROFILE_FUNCTION(ProfileLevelMinorFunctions);

            return VulkanImage(_device, _memoryTypeDelegate, creationParameters);
        }
        //--------------------------------------------------------------------------

        Nullable<VulkanImage*> VulkanImageCreatorDelegate::CreateImagePtr(const VkExtent2D& extent, UInt32 mipLevels, VkSampleCountFlagBits numSamples, VkFormat format, 
                                                                          VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags memoryProperties) const
        {
            return CreateImagePtr(extent.width, extent.height, mipLevels, numSamples, format, tiling, usage, memoryProperties);
        }
        //--------------------------------------------------------------------------

        Nullable<VulkanImage*> VulkanImageCreatorDelegate::CreateImagePtr(UInt32 width, UInt32 height, UInt32 mipLevels, VkSampleCountFlagBits numSamples, VkFormat format, 
                                                                          VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags memoryProperties) const
        {
            const VulkanUtils::ImageParameters creationParameters = {
                .width = width,
                .height = height,
                .mipLevels = mipLevels,
                .numSamples = numSamples,
                .format = format,
                .tiling = tiling,
                .usage = usage,
                .memoryProperties = memoryProperties
            };

            return CreateImagePtr(creationParameters);
        }
        //--------------------------------------------------------------------------

        Nullable<VulkanImage*> VulkanImageCreatorDelegate::CreateImagePtr(const VulkanUtils::ImageParameters& creationParameters) const
        {
            KMP_PROFILE_FUNCTION(ProfileLevelMinorFunctions);

            return new VulkanImage(_device, _memoryTypeDelegate, creationParameters);
        }
        //--------------------------------------------------------------------------

        VkImageView VulkanImageCreatorDelegate::CreateImageView(const VulkanImage& image, VkFormat format, VkImageAspectFlags aspectFlags, UInt32 mipLevels) const
        {
            return CreateImageView(image.GetVkImage(), format, aspectFlags, mipLevels);
        }
        //--------------------------------------------------------------------------

        VkImageView VulkanImageCreatorDelegate::CreateImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, UInt32 mipLevels) const
        {
            KMP_PROFILE_FUNCTION(ProfileLevelMinorFunctions);

            auto viewCreateInfo = VulkanUtils::InitVkImageViewCreateInfo();
            viewCreateInfo.image = image;
            viewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
            viewCreateInfo.format = format;
            viewCreateInfo.subresourceRange.aspectMask = aspectFlags;
            viewCreateInfo.subresourceRange.baseMipLevel = 0;
            viewCreateInfo.subresourceRange.levelCount = mipLevels;
            viewCreateInfo.subresourceRange.baseArrayLayer = 0;
            viewCreateInfo.subresourceRange.layerCount = 1;

            VkImageView imageView;
            const auto result = vkCreateImageView(_device, &viewCreateInfo, nullptr, &imageView);
            VulkanUtils::CheckResult(result, "VulkanImageCreatorDelegate: failed to create texture image view");

            return imageView;
        }
        //--------------------------------------------------------------------------

        VkSampler VulkanImageCreatorDelegate::CreateSampler(const VulkanUtils::SamplerParameters& creationParameters) const
        {
            KMP_PROFILE_FUNCTION(ProfileLevelMinorFunctions);

            auto samplerCreateInfo = VulkanUtils::InitVkSamplerCreateInfo();
            samplerCreateInfo.magFilter = creationParameters.magnificationFilter;
            samplerCreateInfo.minFilter = creationParameters.minificationFilter;
            samplerCreateInfo.mipmapMode = creationParameters.mipmapMode;
            samplerCreateInfo.addressModeU = creationParameters.addressModeU;
            samplerCreateInfo.addressModeV = creationParameters.addressModeV;
            samplerCreateInfo.addressModeW = creationParameters.addressModeW;
            samplerCreateInfo.minLod = creationParameters.minLod;
            samplerCreateInfo.maxLod = creationParameters.maxLod;
            samplerCreateInfo.maxAnisotropy = creationParameters.maxAnisotropy;

            VkSampler sampler;
            const auto result = vkCreateSampler(_device, &samplerCreateInfo, nullptr, &sampler);
            VulkanUtils::CheckResult(result, "VulkanImageCreatorDelegate: failed to create sampler");

            return sampler;
        }
        //--------------------------------------------------------------------------
    }
}