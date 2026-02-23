#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Graphics/Vulkan/vulkan_memory_type_delegate.h"
#include "Kmplete/Log/log_class_macro.h"

#include <vulkan/vulkan.h>


namespace Kmplete
{
    namespace Graphics
    {
        //TODO: comments
        class VulkanImage
        {
            KMP_DISABLE_COPY(VulkanImage)
            KMP_LOG_CLASSNAME(VulkanImage)

        public:
            //TODO: comments
            struct Parameters
            {
                UInt32 width;
                UInt32 height;
                UInt32 mipLevels;
                VkSampleCountFlagBits numSamples;
                VkFormat format;
                VkImageTiling tiling;
                VkImageUsageFlags usage;
                VkMemoryPropertyFlags memoryProperties;
            };

        public:
            KMP_API VulkanImage(VkDevice device, const VulkanMemoryTypeDelegate& memoryTypeDelegate, const Parameters& creationParameters);
            KMP_API ~VulkanImage();

            KMP_NODISCARD KMP_API VkImage GetVkImage() const noexcept;

        private:
            void _CreateImageObject(const Parameters& creationParameters);
            void _AllocateImageMemory(const VulkanMemoryTypeDelegate& memoryTypeDelegate, const Parameters& creationParameters);

        private:
            VkDevice _device;
            VkImage _image;
            VkDeviceMemory _imageMemory;
        };
        //--------------------------------------------------------------------------
    }
}