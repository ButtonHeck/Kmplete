#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/type_traits.h"

#include <vulkan/vulkan.h>


namespace Kmplete
{
    namespace Graphics
    {
        class VulkanCommandBuffer
        {
            KMP_DISABLE_COPY(VulkanCommandBuffer)

        public:
            KMP_API VulkanCommandBuffer(VkDevice device, VkCommandPool commandPool);
            KMP_API VulkanCommandBuffer(VulkanCommandBuffer&& other) noexcept;
            KMP_API VulkanCommandBuffer& operator=(VulkanCommandBuffer&& other) noexcept;
            KMP_API ~VulkanCommandBuffer();

            KMP_API void Begin(VkCommandBufferUsageFlags flags = 0) const;
            KMP_API void End() const;

            KMP_API void Submit(VkQueue queue, VkFence fence = VK_NULL_HANDLE, bool waitFence = false) const;

        private:
            VkDevice _device;
            VkCommandPool _commandPool;
            VkCommandBuffer _commandBuffer;
        };
        //--------------------------------------------------------------------------

        static_assert(IsMoveConstructible<VulkanCommandBuffer>::value);
        static_assert(IsMoveAssignable<VulkanCommandBuffer>::value);
    }
}