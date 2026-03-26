#include "Kmplete/Graphics/Vulkan/Delegates/vulkan_image_creator_delegate.h"
#include "Kmplete/Graphics/Vulkan/Utils/initializers.h"
#include "Kmplete/Graphics/Vulkan/Utils/result_description.h"
#include "Kmplete/Graphics/Vulkan/Utils/presets.h"
#include "Kmplete/Graphics/image.h"
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


        VkImage VulkanImageCreatorDelegate::CreateVkImage(const VkImageCreateInfo& creationParameters) const
        {
            KMP_PROFILE_FUNCTION(ProfileLevelMinorFunctions);

            VkImage image;
            const auto result = vkCreateImage(_device, &creationParameters, nullptr, &image);
            VulkanUtils::CheckResult(result, "VulkanImageCreatorDelegate: failed to create image");

            return image;
        }
        //--------------------------------------------------------------------------


        VulkanImage VulkanImageCreatorDelegate::CreateVulkanImage(const VkExtent2D & extent, UInt32 mipLevels, VkSampleCountFlagBits numSamples, VkFormat format, 
                                                                  VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags memoryProperties) const
        {
            return CreateVulkanImage(extent.width, extent.height, mipLevels, numSamples, format, tiling, usage, memoryProperties);
        }
        //--------------------------------------------------------------------------

        VulkanImage VulkanImageCreatorDelegate::CreateVulkanImage(UInt32 width, UInt32 height, UInt32 mipLevels, VkSampleCountFlagBits numSamples, VkFormat format, 
                                                                  VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags memoryProperties) const
        {
            auto creationParameters = VulkanPresets::GetImageCI_2D_OptimalTiling_Layer1(VkExtent3D{.width = width, .height = height, .depth = 1}, mipLevels, numSamples);
            creationParameters.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            creationParameters.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
            creationParameters.format = format;
            creationParameters.tiling = tiling;
            creationParameters.usage = usage;
            creationParameters.flags = 0;

            return CreateVulkanImage(creationParameters, memoryProperties);
        }
        //--------------------------------------------------------------------------

        VulkanImage VulkanImageCreatorDelegate::CreateVulkanImage(const VkImageCreateInfo& creationParameters, VkMemoryPropertyFlags memoryProperties) const
        {
            KMP_PROFILE_FUNCTION(ProfileLevelMinorFunctions);

            return VulkanImage(_device, creationParameters, _memoryTypeDelegate, memoryProperties);
        }
        //--------------------------------------------------------------------------


        Nullable<VulkanImage*> VulkanImageCreatorDelegate::CreateVulkanImagePtr(const VkExtent2D& extent, UInt32 mipLevels, VkSampleCountFlagBits numSamples, VkFormat format, 
                                                                                VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags memoryProperties) const
        {
            return CreateVulkanImagePtr(extent.width, extent.height, mipLevels, numSamples, format, tiling, usage, memoryProperties);
        }
        //--------------------------------------------------------------------------

        Nullable<VulkanImage*> VulkanImageCreatorDelegate::CreateVulkanImagePtr(UInt32 width, UInt32 height, UInt32 mipLevels, VkSampleCountFlagBits numSamples, VkFormat format, 
                                                                                VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags memoryProperties) const
        {
            auto creationParameters = VulkanPresets::GetImageCI_2D_OptimalTiling_Layer1(VkExtent3D{ .width = width, .height = height, .depth = 1 }, mipLevels, numSamples);
            creationParameters.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            creationParameters.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
            creationParameters.format = format;
            creationParameters.tiling = tiling;
            creationParameters.usage = usage;
            creationParameters.flags = 0;

            return CreateVulkanImagePtr(creationParameters, memoryProperties);
        }
        //--------------------------------------------------------------------------

        Nullable<VulkanImage*> VulkanImageCreatorDelegate::CreateVulkanImagePtr(const VkImageCreateInfo& creationParameters, VkMemoryPropertyFlags memoryProperties) const
        {
            KMP_PROFILE_FUNCTION(ProfileLevelMinorFunctions);

            return new VulkanImage(_device, creationParameters, _memoryTypeDelegate, memoryProperties);
        }
        //--------------------------------------------------------------------------


        VkImageView VulkanImageCreatorDelegate::CreateVkImageView(const VulkanImage& image, VkImageViewType viewType, VkFormat format, const VkImageSubresourceRange& subresourceRange) const
        {
            return CreateVkImageView(image.GetVkImage(), viewType, format, subresourceRange);
        }
        //--------------------------------------------------------------------------

        VkImageView VulkanImageCreatorDelegate::CreateVkImageView(VkImage image, VkImageViewType viewType, VkFormat format, const VkImageSubresourceRange& subresourceRange) const
        {
            auto creationParameters = VulkanUtils::InitVkImageViewCreateInfo();
            creationParameters.image = image;
            creationParameters.viewType = viewType;
            creationParameters.format = format;
            creationParameters.subresourceRange = subresourceRange;

            return CreateVkImageView(creationParameters);
        }
        //--------------------------------------------------------------------------

        VkImageView VulkanImageCreatorDelegate::CreateVkImageView(const VkImageViewCreateInfo& creationParameters) const
        {
            KMP_PROFILE_FUNCTION(ProfileLevelMinorFunctions);

            VkImageView imageView;
            const auto result = vkCreateImageView(_device, &creationParameters, nullptr, &imageView);
            VulkanUtils::CheckResult(result, "VulkanImageCreatorDelegate: failed to create texture image view");

            return imageView;
        }
        //--------------------------------------------------------------------------


        VkSampler VulkanImageCreatorDelegate::CreateVkSampler(const VkSamplerCreateInfo& creationParameters) const
        {
            KMP_PROFILE_FUNCTION(ProfileLevelMinorFunctions);

            VkSampler sampler;
            const auto result = vkCreateSampler(_device, &creationParameters, nullptr, &sampler);
            VulkanUtils::CheckResult(result, "VulkanImageCreatorDelegate: failed to create sampler");

            return sampler;
        }
        //--------------------------------------------------------------------------

        VulkanBuffer VulkanImageCreatorDelegate::CreateStagingImageBuffer(const Image& image) const
        {
            KMP_PROFILE_FUNCTION(ProfileLevelMinorFunctions);

            auto buffer = VulkanBuffer(_memoryTypeDelegate, _device, {VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, image.GetDataSize()});

            auto result = buffer.Map();
            VulkanUtils::CheckResult(result, "VulkanImageCreatorDelegate: failed to map texture buffer");

            buffer.CopyToMappedMemory(0, image.GetPixels(), image.GetDataSize());

            result = buffer.Flush();
            VulkanUtils::CheckResult(result, "VulkanImageCreatorDelegate: failed to flush texture buffer");

            buffer.Unmap();

            return buffer;
        }
        //--------------------------------------------------------------------------
    }
}