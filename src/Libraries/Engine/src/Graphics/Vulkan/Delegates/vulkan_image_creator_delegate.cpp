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
            auto creationParameters = VulkanUtils::InitVkImageCreateInfo();
            creationParameters.imageType = VK_IMAGE_TYPE_2D;
            creationParameters.extent.width = width;
            creationParameters.extent.height = height;
            creationParameters.extent.depth = 1;
            creationParameters.mipLevels = mipLevels;
            creationParameters.arrayLayers = 1;
            creationParameters.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            creationParameters.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
            creationParameters.samples = numSamples;
            creationParameters.format = format;
            creationParameters.tiling = tiling;
            creationParameters.usage = usage;
            creationParameters.flags = 0;

            return CreateImage(creationParameters, memoryProperties);
        }
        //--------------------------------------------------------------------------

        VulkanImage VulkanImageCreatorDelegate::CreateImage(const VkImageCreateInfo& creationParameters, VkMemoryPropertyFlags memoryProperties) const
        {
            KMP_PROFILE_FUNCTION(ProfileLevelMinorFunctions);

            return VulkanImage(_device, _memoryTypeDelegate, creationParameters, memoryProperties);
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
            auto creationParameters = VulkanUtils::InitVkImageCreateInfo();
            creationParameters.imageType = VK_IMAGE_TYPE_2D;
            creationParameters.extent.width = width;
            creationParameters.extent.height = height;
            creationParameters.extent.depth = 1;
            creationParameters.mipLevels = mipLevels;
            creationParameters.arrayLayers = 1;
            creationParameters.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            creationParameters.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
            creationParameters.samples = numSamples;
            creationParameters.format = format;
            creationParameters.tiling = tiling;
            creationParameters.usage = usage;
            creationParameters.flags = 0;

            return CreateImagePtr(creationParameters, memoryProperties);
        }
        //--------------------------------------------------------------------------

        Nullable<VulkanImage*> VulkanImageCreatorDelegate::CreateImagePtr(const VkImageCreateInfo& creationParameters, VkMemoryPropertyFlags memoryProperties) const
        {
            KMP_PROFILE_FUNCTION(ProfileLevelMinorFunctions);

            return new VulkanImage(_device, _memoryTypeDelegate, creationParameters, memoryProperties);
        }
        //--------------------------------------------------------------------------


        VkImageView VulkanImageCreatorDelegate::CreateImageView(const VulkanImage& image, VkImageViewType viewType, VkFormat format, const VkImageSubresourceRange& subresourceRange) const
        {
            return CreateImageView(image.GetVkImage(), viewType, format, subresourceRange);
        }
        //--------------------------------------------------------------------------

        VkImageView VulkanImageCreatorDelegate::CreateImageView(VkImage image, VkImageViewType viewType, VkFormat format, const VkImageSubresourceRange& subresourceRange) const
        {
            auto creationParameters = VulkanUtils::InitVkImageViewCreateInfo();
            creationParameters.image = image;
            creationParameters.viewType = viewType;
            creationParameters.format = format;
            creationParameters.subresourceRange = subresourceRange;

            return CreateImageView(creationParameters);
        }
        //--------------------------------------------------------------------------

        VkImageView VulkanImageCreatorDelegate::CreateImageView(const VkImageViewCreateInfo& creationParameters) const
        {
            KMP_PROFILE_FUNCTION(ProfileLevelMinorFunctions);

            VkImageView imageView;
            const auto result = vkCreateImageView(_device, &creationParameters, nullptr, &imageView);
            VulkanUtils::CheckResult(result, "VulkanImageCreatorDelegate: failed to create texture image view");

            return imageView;
        }
        //--------------------------------------------------------------------------


        VkSampler VulkanImageCreatorDelegate::CreateSampler(const VkSamplerCreateInfo& creationParameters) const
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