#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/nullability.h"
#include "Kmplete/Graphics/Vulkan/Delegates/vulkan_memory_type_delegate.h"
#include "Kmplete/Graphics/Vulkan/vulkan_image.h"
#include "Kmplete/Graphics/Vulkan/Utils/helper_structs.h"
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

            KMP_NODISCARD KMP_API VulkanImage CreateImage(const VkExtent2D& extent, UInt32 mipLevels, VkSampleCountFlagBits numSamples, VkFormat format, 
                                                          VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags memoryProperties) const;
            KMP_NODISCARD KMP_API VulkanImage CreateImage(UInt32 width, UInt32 height, UInt32 mipLevels, VkSampleCountFlagBits numSamples, VkFormat format,
                                                          VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags memoryProperties) const;
            KMP_NODISCARD KMP_API VulkanImage CreateImage(const VulkanUtils::ImageParameters& creationParameters) const;

            KMP_NODISCARD KMP_API Nullable<VulkanImage*> CreateImagePtr(const VkExtent2D& extent, UInt32 mipLevels, VkSampleCountFlagBits numSamples, VkFormat format,
                                                                        VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags memoryProperties) const;
            KMP_NODISCARD KMP_API Nullable<VulkanImage*> CreateImagePtr(UInt32 width, UInt32 height, UInt32 mipLevels, VkSampleCountFlagBits numSamples, VkFormat format,
                                                                        VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags memoryProperties) const;
            KMP_NODISCARD KMP_API Nullable<VulkanImage*> CreateImagePtr(const VulkanUtils::ImageParameters& creationParameters) const;

            KMP_NODISCARD KMP_API VkImageView CreateImageView(const VulkanImage& image, VkImageViewType viewType, VkFormat format, VkImageSubresourceRange subresourceRange) const;
            KMP_NODISCARD KMP_API VkImageView CreateImageView(VkImage image, VkImageViewType viewType, VkFormat format, VkImageSubresourceRange subresourceRange) const;
            KMP_NODISCARD KMP_API VkImageView CreateImageView(VkImageViewCreateInfo creationParameters) const;

            KMP_NODISCARD KMP_API VkSampler CreateSampler(VkSamplerCreateInfo creationParameters) const;

        private:
            const VulkanMemoryTypeDelegate& _memoryTypeDelegate;

            VkDevice _device;
        };
        //--------------------------------------------------------------------------
    }
}