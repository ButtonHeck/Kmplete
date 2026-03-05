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

        VkImageView VulkanImageCreatorDelegate::CreateImageView(const VulkanImage& image, VkImageViewType viewType, VkFormat format, VkImageSubresourceRange subresourceRange) const
        {
            return CreateImageView(image.GetVkImage(), viewType, format, subresourceRange);
        }
        //--------------------------------------------------------------------------

        VkImageView VulkanImageCreatorDelegate::CreateImageView(VkImage image, VkImageViewType viewType, VkFormat format, VkImageSubresourceRange subresourceRange) const
        {
            auto creationParameters = VulkanUtils::InitVkImageViewCreateInfo();
            creationParameters.image = image;
            creationParameters.viewType = viewType;
            creationParameters.format = format;
            creationParameters.subresourceRange = subresourceRange;

            return CreateImageView(creationParameters);
        }
        //--------------------------------------------------------------------------

        VkImageView VulkanImageCreatorDelegate::CreateImageView(VkImageViewCreateInfo creationParameters) const
        {
            KMP_PROFILE_FUNCTION(ProfileLevelMinorFunctions);

            VkImageView imageView;
            const auto result = vkCreateImageView(_device, &creationParameters, nullptr, &imageView);
            VulkanUtils::CheckResult(result, "VulkanImageCreatorDelegate: failed to create texture image view");

            return imageView;
        }
        //--------------------------------------------------------------------------

        VkSampler VulkanImageCreatorDelegate::CreateSampler(VkSamplerCreateInfo creationParameters) const
        {
            KMP_PROFILE_FUNCTION(ProfileLevelMinorFunctions);

            VkSampler sampler;
            const auto result = vkCreateSampler(_device, &creationParameters, nullptr, &sampler);
            VulkanUtils::CheckResult(result, "VulkanImageCreatorDelegate: failed to create sampler");

            return sampler;
        }
        //--------------------------------------------------------------------------
    }
}