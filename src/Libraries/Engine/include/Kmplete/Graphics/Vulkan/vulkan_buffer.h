#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Base/type_traits.h"

#include <vulkan/vulkan.h>


namespace Kmplete
{
    namespace Graphics
    {
        class VulkanMemoryTypeDelegate;


        //TODO: comments
        class VulkanBuffer
        {
            KMP_DISABLE_COPY(VulkanBuffer)

        public:
            KMP_API VulkanBuffer(const VulkanMemoryTypeDelegate& memoryTypeDelegate, VkDevice device, VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags memoryPropertyFlags, VkDeviceSize size);
            KMP_API VulkanBuffer(VulkanBuffer&& other) noexcept;
            KMP_API VulkanBuffer& operator=(VulkanBuffer&& other) noexcept;
            KMP_API virtual ~VulkanBuffer();

            KMP_API VkResult Map(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
            KMP_API void Unmap();
            KMP_API VkResult Bind(VkDeviceSize offset = 0);
            KMP_API void CopyToMappedMemory(UInt32 mappedOffset, void* data, VkDeviceSize size);
            KMP_API VkResult Flush(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
            KMP_API VkResult Invalidate(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);

            KMP_NODISCARD KMP_API VkBuffer GetVkBuffer() const noexcept;
            KMP_NODISCARD KMP_API void* GetMappedPtr() const noexcept;

        protected:
            VkDevice _device;
            VkBuffer _buffer;

        private:
            VkDeviceMemory _memory;
            VkDeviceSize _size;
            void* _mapped;
            VkBufferUsageFlags _usageFlags;
        };
        //--------------------------------------------------------------------------

        static_assert(IsMoveConstructible<VulkanBuffer>::value);
        static_assert(IsMoveAssignable<VulkanBuffer>::value);
    }
}