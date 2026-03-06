#include "Kmplete/Graphics/Vulkan/vulkan_buffer.h"
#include "Kmplete/Graphics/Vulkan/Delegates/vulkan_memory_type_delegate.h"
#include "Kmplete/Graphics/Vulkan/Utils/initializers.h"
#include "Kmplete/Graphics/Vulkan/Utils/function_utils.h"
#include "Kmplete/Core/assertion.h"
#include "Kmplete/Profile/profiler.h"


namespace Kmplete
{
    namespace Graphics
    {
        VulkanBuffer::VulkanBuffer(const VulkanMemoryTypeDelegate& memoryTypeDelegate, VkDevice device, VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags memoryPropertyFlags, VkDeviceSize size)
            : _device(device)
            , _buffer(VK_NULL_HANDLE)
            , _memory(VK_NULL_HANDLE)
            , _size(size)
            , _mapped(nullptr)
            , _usageFlags(usageFlags)
        {
            KMP_PROFILE_FUNCTION(ProfileLevelAlways);

            auto bufferCreateInfo = VulkanUtils::InitVkBufferCreateInfo(_size, _usageFlags);

            auto result = vkCreateBuffer(_device, &bufferCreateInfo, nullptr, &_buffer);
            VulkanUtils::CheckResult(result, "VulkanBuffer: failed to create buffer object");

            auto bufferMemoryContext = memoryTypeDelegate.GetBufferMemoryContext(_device, _buffer, memoryPropertyFlags);
            VkMemoryAllocateFlagsInfoKHR allocateFlagsInfo = VulkanUtils::InitVkMemoryAllocateFlagsInfoKHR();
            if (_usageFlags & VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT)
            {
                allocateFlagsInfo.flags = VK_MEMORY_ALLOCATE_DEVICE_ADDRESS_BIT_KHR;
                bufferMemoryContext.allocateInfo.pNext = &allocateFlagsInfo;
            }

            result = vkAllocateMemory(_device, &bufferMemoryContext.allocateInfo, nullptr, &_memory);
            VulkanUtils::CheckResult(result, "VulkanBuffer: failed to allocate buffer memory");

            result = Bind();
            VulkanUtils::CheckResult(result, "VulkanBuffer: failed to bind buffer");
        }
        //--------------------------------------------------------------------------

        VulkanBuffer::VulkanBuffer(VulkanBuffer&& other) noexcept
            : _device(other._device)
            , _buffer(other._buffer)
            , _memory(other._memory)
            , _size(other._size)
            , _mapped(other._mapped)
            , _usageFlags(other._usageFlags)
        {
            other._device = VK_NULL_HANDLE;
            other._buffer = VK_NULL_HANDLE;
            other._memory = VK_NULL_HANDLE;
            other._size = 0ULL;
            other._mapped = nullptr;
        }
        //--------------------------------------------------------------------------

        VulkanBuffer& VulkanBuffer::operator=(VulkanBuffer&& other) noexcept
        {
            if (this == &other)
            {
                return *this;
            }

            Unmap();

            if (_buffer)
            {
                vkDestroyBuffer(_device, _buffer, nullptr);
            }

            if (_memory)
            {
                vkFreeMemory(_device, _memory, nullptr);
            }

            _device = other._device;
            _buffer = other._buffer;
            _memory = other._memory;
            _size = other._size;
            _mapped = other._mapped;
            _usageFlags = other._usageFlags;

            other.Unmap();
            other._device = VK_NULL_HANDLE;
            other._buffer = VK_NULL_HANDLE;
            other._memory = VK_NULL_HANDLE;
            other._size = 0ULL;
            other._mapped = nullptr;

            return *this;
        }
        //--------------------------------------------------------------------------

        VulkanBuffer::~VulkanBuffer()
        {
            KMP_PROFILE_FUNCTION(ProfileLevelAlways);

            if (_buffer)
            {
                vkDestroyBuffer(_device, _buffer, nullptr);
            }

            if (_memory)
            {
                vkFreeMemory(_device, _memory, nullptr);
            }
        }
        //--------------------------------------------------------------------------

        VkResult VulkanBuffer::Map(VkDeviceSize size /*= VK_WHOLE_SIZE*/, VkDeviceSize offset /*= 0*/)
        {
            KMP_PROFILE_FUNCTION(ProfileLevelMinorFunctions);

            return vkMapMemory(_device, _memory, offset, size, 0, &_mapped);
        }
        //--------------------------------------------------------------------------

        void VulkanBuffer::Unmap()
        {
            KMP_PROFILE_FUNCTION(ProfileLevelMinorFunctions);

            if (_mapped)
            {
                vkUnmapMemory(_device, _memory);
                _mapped = nullptr;
            }
        }
        //--------------------------------------------------------------------------

        VkResult VulkanBuffer::Bind(VkDeviceSize offset /*= 0*/)
        {
            KMP_PROFILE_FUNCTION(ProfileLevelMinorFunctions);

            return vkBindBufferMemory(_device, _buffer, _memory, offset);
        }
        //--------------------------------------------------------------------------

        void VulkanBuffer::CopyToMappedMemory(void* data, VkDeviceSize size)
        {
            KMP_PROFILE_FUNCTION(ProfileLevelMinorFunctions);

            KMP_ASSERT(_mapped);
            memcpy(_mapped, data, size);
        }
        //--------------------------------------------------------------------------

        VkResult VulkanBuffer::Flush(VkDeviceSize size /*= VK_WHOLE_SIZE*/, VkDeviceSize offset /*= 0*/)
        {
            KMP_PROFILE_FUNCTION(ProfileLevelMinorFunctions);

            auto mappedRange = VulkanUtils::InitVkMappedMemoryRange(size, offset);
            mappedRange.memory = _memory;

            return vkFlushMappedMemoryRanges(_device, 1, &mappedRange);
        }
        //--------------------------------------------------------------------------

        VkResult VulkanBuffer::Invalidate(VkDeviceSize size /*= VK_WHOLE_SIZE*/, VkDeviceSize offset /*= 0*/)
        {
            KMP_PROFILE_FUNCTION(ProfileLevelMinorFunctions);

            auto mappedRange = VulkanUtils::InitVkMappedMemoryRange(size, offset);
            mappedRange.memory = _memory;

            return vkInvalidateMappedMemoryRanges(_device, 1, &mappedRange);
        }
        //--------------------------------------------------------------------------

        VkBuffer VulkanBuffer::GetVkBuffer() const noexcept
        {
            return _buffer;
        }
        //--------------------------------------------------------------------------
    }
}