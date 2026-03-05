#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Log/log_class_macro.h"

#include <vulkan/vulkan.h>


namespace Kmplete
{
    namespace Graphics
    {
        class VulkanMemoryTypeDelegate;


        //TODO: comments
        class VulkanImage
        {
            KMP_DISABLE_COPY(VulkanImage)
            KMP_LOG_CLASSNAME(VulkanImage)

        public:
            KMP_API VulkanImage(VkDevice device, const VkImageCreateInfo& creationParameters, const VulkanMemoryTypeDelegate& memoryTypeDelegate, VkMemoryPropertyFlags memoryProperties);
            KMP_API ~VulkanImage();

            KMP_NODISCARD KMP_API VkImage GetVkImage() const noexcept;
            KMP_NODISCARD KMP_API VkDeviceSize GetMemorySize() const noexcept;

        private:
            void _CreateImageObject(const VkImageCreateInfo& creationParameters);
            void _AllocateImageMemory(const VulkanMemoryTypeDelegate& memoryTypeDelegate, VkMemoryPropertyFlags memoryProperties);

        private:
            VkDevice _device;
            VkImage _image;
            VkDeviceMemory _imageMemory;
            VkDeviceSize _memorySize;
        };
        //--------------------------------------------------------------------------
    }
}