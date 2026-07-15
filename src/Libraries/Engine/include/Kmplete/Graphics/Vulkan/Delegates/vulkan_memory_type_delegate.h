#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Log/log_class_macro.h"
#include "Kmplete/Profile/profiler_fwd.h"

#include <vulkan/vulkan.h>


namespace Kmplete
{
    namespace Graphics
    {
        //! Helper delegate class for handling memory requirements functions during
        //! image or buffer creation, finding suitable memory type for required properties
        class VulkanMemoryTypeDelegate
        {
            KMP_DISABLE_COPY_MOVE(VulkanMemoryTypeDelegate)
            KMP_LOG_CLASSNAME(VulkanMemoryTypeDelegate)
            KMP_PROFILE_CONSTRUCTOR_DECLARE()

        public:
            //! Memory allocation information wrapper struct
            struct MemoryContext
            {
                VkMemoryRequirements requirements{};
                VkMemoryAllocateInfo allocateInfo{};
            };

        public:
            KMP_API explicit VulkanMemoryTypeDelegate(VkPhysicalDeviceMemoryProperties memoryProperties) noexcept;
            ~VulkanMemoryTypeDelegate() = default;

            KMP_NODISCARD KMP_API MemoryContext GetBufferMemoryContext(VkDevice device, VkBuffer buffer, VkMemoryPropertyFlags properties) const;
            KMP_NODISCARD KMP_API MemoryContext GetImageMemoryContext(VkDevice device, VkImage image, VkMemoryPropertyFlags properties) const;
            KMP_NODISCARD KMP_API UInt32 FindMemoryType(UInt32 typeFilter, VkMemoryPropertyFlags properties) const;

        private:
            VkPhysicalDeviceMemoryProperties _memoryProperties;
        };
        //--------------------------------------------------------------------------
    }
}