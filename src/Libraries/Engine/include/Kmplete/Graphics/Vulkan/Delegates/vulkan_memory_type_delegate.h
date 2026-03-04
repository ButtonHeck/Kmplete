#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Log/log_class_macro.h"

#include <vulkan/vulkan.h>


namespace Kmplete
{
    namespace Graphics
    {
        //TODO: comments
        class VulkanMemoryTypeDelegate
        {
            KMP_DISABLE_COPY_MOVE(VulkanMemoryTypeDelegate)
            KMP_LOG_CLASSNAME(VulkanMemoryTypeDelegate)

        public:
            struct MemoryContext
            {
                VkMemoryRequirements requirements{};
                VkMemoryAllocateInfo allocateInfo{};
            };

        public:
            KMP_API explicit VulkanMemoryTypeDelegate(VkPhysicalDeviceMemoryProperties memoryProperties) noexcept;

            KMP_NODISCARD KMP_API MemoryContext GetBufferMemoryContext(VkDevice device, VkBuffer buffer, VkMemoryPropertyFlags properties) const;
            KMP_NODISCARD KMP_API MemoryContext GetImageMemoryContext(VkDevice device, VkImage image, VkMemoryPropertyFlags properties) const;
            KMP_NODISCARD KMP_API UInt32 FindMemoryType(UInt32 typeFilter, VkMemoryPropertyFlags properties) const;

        private:
            VkPhysicalDeviceMemoryProperties _memoryProperties;
        };
        //--------------------------------------------------------------------------
    }
}