#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Base/type_traits.h"
#include "Kmplete/Graphics/Vulkan/vulkan_buffer.h"

#include <vulkan/vulkan.h>


namespace Kmplete
{
    namespace Graphics
    {
        class VulkanMemoryTypeDelegate;


        //TODO: comments
        class VulkanUniformBuffer : public VulkanBuffer
        {
            KMP_DISABLE_COPY(VulkanUniformBuffer)

        public:
            KMP_API VulkanUniformBuffer(const VulkanMemoryTypeDelegate& memoryTypeDelegate, VkDevice device, const VulkanBufferParameters& parameters);
            KMP_API VulkanUniformBuffer(VulkanUniformBuffer&& other) noexcept;
            KMP_API VulkanUniformBuffer& operator=(VulkanUniformBuffer&& other) noexcept;
        };
        //--------------------------------------------------------------------------

        static_assert(IsMoveConstructible<VulkanUniformBuffer>::value);
        static_assert(IsMoveAssignable<VulkanUniformBuffer>::value);
    }
}