#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/nullability.h"
#include "Kmplete/Graphics/Vulkan/Delegates/vulkan_memory_type_delegate.h"
#include "Kmplete/Graphics/Vulkan/Texture/vulkan_image.h"
#include "Kmplete/Graphics/Vulkan/Buffer/vulkan_buffer.h"
#include "Kmplete/Profile/profiler_fwd.h"

#include <vulkan/vulkan.h>


namespace Kmplete
{
    namespace Graphics
    {
        class Image;


        //! Helper delegate class for creating Vulkan images (raw VkImage or VulkanImage wrappers), 
        //! image views and staging Vulkan buffers for texture creation, uses VulkanMemoryTypeDelegate object
        //! for delegating memory properties requirements during staging buffer creation.
        //! @see VulkanImage
        //! @see VulkanMemoryTypeDelegate
        class KMP_API VulkanImageCreatorDelegate
        {
            KMP_DISABLE_COPY_MOVE(VulkanImageCreatorDelegate)
            KMP_PROFILE_CONSTRUCTOR_DECLARE()

        public:
            VulkanImageCreatorDelegate(VkDevice device, const VulkanMemoryTypeDelegate& memoryTypeDelegate);
            ~VulkanImageCreatorDelegate() = default;

            KMP_NODISCARD VkImage CreateVkImage(const VkImageCreateInfo& creationParameters) const;

            KMP_NODISCARD VulkanImage CreateVulkanImage(const VkExtent2D& extent, UInt32 mipLevels, VkSampleCountFlagBits numSamples, VkFormat format, 
                                                        VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags memoryProperties) const;
            KMP_NODISCARD VulkanImage CreateVulkanImage(UInt32 width, UInt32 mipLevels, VkSampleCountFlagBits numSamples, VkFormat format,
                                                        VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags memoryProperties) const;
            KMP_NODISCARD VulkanImage CreateVulkanImage(UInt32 width, UInt32 height, UInt32 mipLevels, VkSampleCountFlagBits numSamples, VkFormat format,
                                                        VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags memoryProperties) const;
            KMP_NODISCARD VulkanImage CreateVulkanImage(const VkImageCreateInfo& creationParameters, VkMemoryPropertyFlags memoryProperties) const;

            KMP_NODISCARD Nullable<VulkanImage*> CreateVulkanImagePtr(const VkExtent2D& extent, UInt32 mipLevels, VkSampleCountFlagBits numSamples, VkFormat format,
                                                                      VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags memoryProperties) const;
            KMP_NODISCARD Nullable<VulkanImage*> CreateVulkanImagePtr(UInt32 width, UInt32 mipLevels, VkSampleCountFlagBits numSamples, VkFormat format,
                                                                      VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags memoryProperties) const;
            KMP_NODISCARD Nullable<VulkanImage*> CreateVulkanImagePtr(UInt32 width, UInt32 height, UInt32 mipLevels, VkSampleCountFlagBits numSamples, VkFormat format,
                                                                      VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags memoryProperties) const;
            KMP_NODISCARD Nullable<VulkanImage*> CreateVulkanImagePtr(const VkImageCreateInfo& creationParameters, VkMemoryPropertyFlags memoryProperties) const;

            KMP_NODISCARD VkImageView CreateVkImageView(const VulkanImage& image, VkImageViewType viewType, VkFormat format, const VkImageSubresourceRange& subresourceRange) const;
            KMP_NODISCARD VkImageView CreateVkImageView(VkImage image, VkImageViewType viewType, VkFormat format, const VkImageSubresourceRange& subresourceRange) const;
            KMP_NODISCARD VkImageView CreateVkImageView(const VkImageViewCreateInfo& creationParameters) const;

            KMP_NODISCARD VulkanBuffer CreateStagingImageBuffer(const Image& image) const;

        private:
            const VulkanMemoryTypeDelegate& _memoryTypeDelegate;

            VkDevice _device;
        };
        //--------------------------------------------------------------------------
    }
}