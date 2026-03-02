#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Graphics/Vulkan/vulkan_memory_type_delegate.h"

#include <vulkan/vulkan.h>


namespace Kmplete
{
    namespace Graphics
    {
        //TODO: comments
        class VulkanBuffer
        {
            KMP_DISABLE_COPY_MOVE(VulkanBuffer)

        public:
            KMP_API VulkanBuffer(const VulkanMemoryTypeDelegate& memoryTypeDelegate, VkDevice device, VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags memoryPropertyFlags, VkDeviceSize size);
            KMP_API ~VulkanBuffer();

            KMP_NODISCARD KMP_API VkResult Map(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
            KMP_API void Unmap();

            KMP_NODISCARD KMP_API VkResult Bind(VkDeviceSize offset = 0);
            KMP_API void CopyToMappedMemory(void* data, VkDeviceSize size);
            KMP_API VkResult Flush(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
            KMP_NODISCARD KMP_API VkResult Invalidate(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);

            KMP_NODISCARD KMP_API VkBuffer GetVkBuffer() const noexcept;

        private:
            VkDevice _device;
            VkBuffer _buffer;
            VkDeviceMemory _memory;
            VkDeviceSize _size;
            void* _mapped;
            VkBufferUsageFlags _usageFlags;
        };
        //--------------------------------------------------------------------------
    }
}