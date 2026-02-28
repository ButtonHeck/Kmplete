#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Graphics/Vulkan/vulkan_memory_type_delegate.h"
#include "Kmplete/Graphics/Vulkan/Utils/helper_structs.h"
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
            KMP_API VulkanImage(VkDevice device, const VulkanMemoryTypeDelegate& memoryTypeDelegate, const VulkanUtils::ImageParameters& creationParameters);
            KMP_API ~VulkanImage();

            KMP_NODISCARD KMP_API VkImage GetVkImage() const noexcept;
            KMP_NODISCARD KMP_API VkDeviceSize GetMemorySize() const noexcept;

        private:
            void _CreateImageObject(const VulkanUtils::ImageParameters& creationParameters);
            void _AllocateImageMemory(const VulkanMemoryTypeDelegate& memoryTypeDelegate, const VulkanUtils::ImageParameters& creationParameters);

        private:
            VkDevice _device;
            VkImage _image;
            VkDeviceMemory _imageMemory;
            VkDeviceSize _memorySize;
        };
        //--------------------------------------------------------------------------
    }
}