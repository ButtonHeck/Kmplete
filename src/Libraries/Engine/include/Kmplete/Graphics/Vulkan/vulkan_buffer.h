#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Graphics/Vulkan/vulkan_memory_type_delegate.h"
#include "Kmplete/Log/log_class_macro.h"

#include <vulkan/vulkan.h>


namespace Kmplete
{
    namespace Graphics
    {
        //TODO: comments
        class VulkanBuffer
        {
            KMP_DISABLE_COPY_MOVE(VulkanBuffer)
            KMP_LOG_CLASSNAME(VulkanBuffer)

        public:
            KMP_API VulkanBuffer(const VulkanMemoryTypeDelegate& memoryTypeDelegate, VkDevice device, VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags memoryPropertyFlags, VkDeviceSize size, void* data);
            KMP_API ~VulkanBuffer();

            KMP_NODISCARD KMP_API VkResult Map(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
            KMP_API void Unmap();

            KMP_NODISCARD KMP_API VkResult Bind(VkDeviceSize offset = 0);
            KMP_API void CopyTo(void* data, VkDeviceSize size);
            KMP_API VkResult Flush(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
            KMP_NODISCARD KMP_API VkResult Invalidate(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
            KMP_API void Destroy();

        private:
            void _SetupDescriptor(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);

        private:
            VkDevice _device;
            VkBuffer _buffer;
            VkDeviceMemory _memory;
            VkDescriptorBufferInfo _descriptor;
            VkDeviceSize _size;
            VkDeviceSize _alignment;
            void* _mapped;
            VkBufferUsageFlags _usageFlags;
        };
        //--------------------------------------------------------------------------
    }
}