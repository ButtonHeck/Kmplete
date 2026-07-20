#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Base/type_traits.h"
#include "Kmplete/Profile/profiler_fwd.h"

#include <vulkan/vulkan.h>


namespace Kmplete
{
    namespace Graphics
    {
        class VulkanMemoryTypeDelegate;


        //! Helper struct for storing Vulkan buffer creation parameters
        //! @see VulkanBuffer
        struct VulkanBufferParameters
        {
            VkBufferUsageFlags usageFlags;
            VkMemoryPropertyFlags memoryPropertyFlags;
            VkDeviceSize size;
        };
        //--------------------------------------------------------------------------


        //! Base class of a Vulkan buffer object, supports memory management functionality such as: (un)mapping,
        //! flushing, invalidation, copying. Buffer's intentional usage can be acquired either by using VkBufferUsageFlags getter or 
        //! by an exact type getter.
        class KMP_API VulkanBuffer
        {
            KMP_DISABLE_COPY(VulkanBuffer)
            KMP_PROFILE_CONSTRUCTOR_DECLARE()

        public:
            VulkanBuffer(const VulkanMemoryTypeDelegate& memoryTypeDelegate, VkDevice device, const VulkanBufferParameters& parameters);
            VulkanBuffer(VulkanBuffer&& other) noexcept;
            VulkanBuffer& operator=(VulkanBuffer&& other) noexcept;
            virtual ~VulkanBuffer();

            VkResult Map(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
            VkResult Unmap(bool flush = false, VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
            VkResult Flush(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
            VkResult Invalidate(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
            void CopyToMappedMemory(UInt32 mappedOffset, void* data, VkDeviceSize size);

            KMP_NODISCARD VkBuffer GetVkBuffer() const noexcept;
            KMP_NODISCARD VkDeviceSize GetSize() const noexcept;
            KMP_NODISCARD void* GetMappedPtr() const noexcept;
            KMP_NODISCARD VkBufferUsageFlags GetUsageFlags() const noexcept;

            KMP_NODISCARD bool IsTransferSourceBuffer() const noexcept;
            KMP_NODISCARD bool IsTransferDestinationBuffer() const noexcept;
            KMP_NODISCARD bool IsUniformTexelBuffer() const noexcept;
            KMP_NODISCARD bool IsStorageTexelBuffer() const noexcept;
            KMP_NODISCARD bool IsUniformBuffer() const noexcept;
            KMP_NODISCARD bool IsStorageBuffer() const noexcept;
            KMP_NODISCARD bool IsIndexBuffer() const noexcept;
            KMP_NODISCARD bool IsVertexBuffer() const noexcept;
            KMP_NODISCARD bool IsIndirectBuffer() const noexcept;
            KMP_NODISCARD bool IsShaderDeviceAddressBuffer() const noexcept;

        private:
            void _Initialize(const VulkanMemoryTypeDelegate& memoryTypeDelegate, const VulkanBufferParameters& parameters);
            void _Finalize();

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