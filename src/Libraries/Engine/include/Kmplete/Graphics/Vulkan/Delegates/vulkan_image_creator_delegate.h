#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/nullability.h"
#include "Kmplete/Graphics/Vulkan/Delegates/vulkan_memory_type_delegate.h"
#include "Kmplete/Graphics/Vulkan/vulkan_image.h"
#include "Kmplete/Log/log_class_macro.h"

#include <vulkan/vulkan.h>


namespace Kmplete
{
    namespace Graphics
    {
        //TODO: comments
        class VulkanImageCreatorDelegate
        {
            KMP_DISABLE_COPY_MOVE(VulkanImageCreatorDelegate)
            KMP_LOG_CLASSNAME(VulkanImageCreatorDelegate)

        public:
            KMP_API VulkanImageCreatorDelegate(VkDevice device, const VulkanMemoryTypeDelegate& memoryTypeDelegate);

            KMP_NODISCARD KMP_API VkImage CreateVkImage(const VkImageCreateInfo& creationParameters) const;

            KMP_NODISCARD KMP_API VulkanImage CreateVulkanImage(const VkExtent2D& extent, UInt32 mipLevels, VkSampleCountFlagBits numSamples, VkFormat format, 
                                                                VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags memoryProperties) const;
            KMP_NODISCARD KMP_API VulkanImage CreateVulkanImage(UInt32 width, UInt32 height, UInt32 mipLevels, VkSampleCountFlagBits numSamples, VkFormat format,
                                                                VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags memoryProperties) const;
            KMP_NODISCARD KMP_API VulkanImage CreateVulkanImage(const VkImageCreateInfo& creationParameters, VkMemoryPropertyFlags memoryProperties) const;

            KMP_NODISCARD KMP_API Nullable<VulkanImage*> CreateVulkanImagePtr(const VkExtent2D& extent, UInt32 mipLevels, VkSampleCountFlagBits numSamples, VkFormat format,
                                                                              VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags memoryProperties) const;
            KMP_NODISCARD KMP_API Nullable<VulkanImage*> CreateVulkanImagePtr(UInt32 width, UInt32 height, UInt32 mipLevels, VkSampleCountFlagBits numSamples, VkFormat format,
                                                                              VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags memoryProperties) const;
            KMP_NODISCARD KMP_API Nullable<VulkanImage*> CreateVulkanImagePtr(const VkImageCreateInfo& creationParameters, VkMemoryPropertyFlags memoryProperties) const;

            KMP_NODISCARD KMP_API VkImageView CreateVkImageView(const VulkanImage& image, VkImageViewType viewType, VkFormat format, const VkImageSubresourceRange& subresourceRange) const;
            KMP_NODISCARD KMP_API VkImageView CreateVkImageView(VkImage image, VkImageViewType viewType, VkFormat format, const VkImageSubresourceRange& subresourceRange) const;
            KMP_NODISCARD KMP_API VkImageView CreateVkImageView(const VkImageViewCreateInfo& creationParameters) const;

            KMP_NODISCARD KMP_API VkSampler CreateVkSampler(const VkSamplerCreateInfo& creationParameters) const;

        private:
            const VulkanMemoryTypeDelegate& _memoryTypeDelegate;

            VkDevice _device;
        };
        //--------------------------------------------------------------------------
    }
}