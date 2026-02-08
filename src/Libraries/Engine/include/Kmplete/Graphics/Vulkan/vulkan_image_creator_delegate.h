#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Graphics/Vulkan/vulkan_physical_device_info.h"
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
            KMP_API VulkanImageCreatorDelegate(const VkDevice& device, const PhysicalDeviceInfo& physicalDeviceInfo);

            KMP_NODISCARD KMP_API VulkanImage CreateImage(const VkExtent2D& extent, UInt32 mipLevels, VkSampleCountFlagBits numSamples, VkFormat format, 
                                                          VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags memoryProperties) const;
            KMP_NODISCARD KMP_API VulkanImage CreateImage(UInt32 width, UInt32 height, UInt32 mipLevels, VkSampleCountFlagBits numSamples, VkFormat format,
                                                          VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags memoryProperties) const;
            KMP_NODISCARD KMP_API VulkanImage CreateImage(const VulkanImage::Parameters& creationParameters) const;

            KMP_NODISCARD KMP_API VkImageView CreateImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, UInt32 mipLevels) const;

        private:
            const VkDevice& _device;
            const PhysicalDeviceInfo& _physicalDeviceInfo;
        };
        //--------------------------------------------------------------------------
    }
}