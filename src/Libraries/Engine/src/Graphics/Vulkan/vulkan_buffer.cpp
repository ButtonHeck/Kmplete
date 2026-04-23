#include "Kmplete/Graphics/Vulkan/vulkan_buffer.h"
#include "Kmplete/Graphics/Vulkan/Delegates/vulkan_memory_type_delegate.h"
#include "Kmplete/Graphics/Vulkan/Utils/initializers.h"
#include "Kmplete/Graphics/Vulkan/Utils/result_description.h"
#include "Kmplete/Graphics/Vulkan/Utils/bits_aliases.h"
#include "Kmplete/Core/assertion.h"
#include "Kmplete/Profile/profiler.h"


namespace Kmplete
{
    namespace Graphics
    {
        using namespace VKBits;


        VulkanBuffer::VulkanBuffer(const VulkanMemoryTypeDelegate& memoryTypeDelegate, VkDevice device, const VulkanBufferParameters& parameters)
            : _device(device)
            , _buffer(VK_NULL_HANDLE)
            , _memory(VK_NULL_HANDLE)
            , _size(parameters.size)
            , _mapped(nullptr)
            , _usageFlags(parameters.usageFlags)
        {
            KMP_PROFILE_FUNCTION(ProfileLevelAlways);

            _Initialize(memoryTypeDelegate, parameters);
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

            KMP_ASSERT(_device && _buffer && _memory);
        }
        //--------------------------------------------------------------------------

        VulkanBuffer& VulkanBuffer::operator=(VulkanBuffer&& other) noexcept
        {
            if (this == &other)
            {
                return *this;
            }

            Unmap();
            _Finalize();

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

            KMP_ASSERT(_device && _buffer && _memory);

            return *this;
        }
        //--------------------------------------------------------------------------

        VulkanBuffer::~VulkanBuffer() KMP_PROFILING(ProfileLevelAlways)
        {
            _Finalize();
        }}
        //--------------------------------------------------------------------------

        VkResult VulkanBuffer::Map(VkDeviceSize size /*= VK_WHOLE_SIZE*/, VkDeviceSize offset /*= 0*/) KMP_PROFILING(ProfileLevelMinor)
        {
            KMP_ASSERT(_device && _memory);

            return vkMapMemory(_device, _memory, offset, size, 0, &_mapped);
        }}
        //--------------------------------------------------------------------------

        VkResult VulkanBuffer::Unmap(bool flush /*= false*/, VkDeviceSize size/* = VK_WHOLE_SIZE*/, VkDeviceSize offset /*= 0*/) KMP_PROFILING(ProfileLevelMinor)
        {
            KMP_ASSERT(_device && _memory);

            auto result = VK_SUCCESS;
            if (flush)
            {
                result = Flush(size, offset);
            }

            if (_mapped)
            {
                vkUnmapMemory(_device, _memory);
                _mapped = nullptr;
            }

            return result;
        }}
        //--------------------------------------------------------------------------

        VkResult VulkanBuffer::Bind(VkDeviceSize offset /*= 0*/) KMP_PROFILING(ProfileLevelMinor)
        {
            KMP_ASSERT(_device && _buffer && _memory);

            return vkBindBufferMemory(_device, _buffer, _memory, offset);
        }}
        //--------------------------------------------------------------------------

        VkResult VulkanBuffer::Flush(VkDeviceSize size /*= VK_WHOLE_SIZE*/, VkDeviceSize offset /*= 0*/) KMP_PROFILING(ProfileLevelMinor)
        {
            KMP_ASSERT(_device && _memory);

            auto mappedRange = VKUtils::InitVkMappedMemoryRange(size, offset);
            mappedRange.memory = _memory;

            return vkFlushMappedMemoryRanges(_device, 1, &mappedRange);
        }}
        //--------------------------------------------------------------------------

        VkResult VulkanBuffer::Invalidate(VkDeviceSize size /*= VK_WHOLE_SIZE*/, VkDeviceSize offset /*= 0*/) KMP_PROFILING(ProfileLevelMinor)
        {
            KMP_ASSERT(_device && _memory);

            auto mappedRange = VKUtils::InitVkMappedMemoryRange(size, offset);
            mappedRange.memory = _memory;

            return vkInvalidateMappedMemoryRanges(_device, 1, &mappedRange);
        }}
        //--------------------------------------------------------------------------

        void VulkanBuffer::CopyToMappedMemory(UInt32 mappedOffset, void* data, VkDeviceSize size) KMP_PROFILING(ProfileLevelMinor)
        {
            KMP_ASSERT(_mapped);

            memcpy((char*)_mapped + mappedOffset, data, size);
        }}
        //--------------------------------------------------------------------------

        VkBuffer VulkanBuffer::GetVkBuffer() const noexcept
        {
            KMP_ASSERT(_buffer);

            return _buffer;
        }
        //--------------------------------------------------------------------------

        VkDeviceSize VulkanBuffer::GetSize() const noexcept
        {
            return _size;
        }
        //--------------------------------------------------------------------------

        void* VulkanBuffer::GetMappedPtr() const noexcept
        {
            return _mapped;
        }
        //--------------------------------------------------------------------------

        void VulkanBuffer::_Initialize(const VulkanMemoryTypeDelegate& memoryTypeDelegate, const VulkanBufferParameters& parameters) KMP_PROFILING(ProfileLevelAlways)
        {
            KMP_ASSERT(_device);

            auto bufferCreateInfo = VKUtils::InitVkBufferCreateInfo(_size, _usageFlags);

            auto result = vkCreateBuffer(_device, &bufferCreateInfo, nullptr, &_buffer);
            VKUtils::CheckResult(result, "VulkanBuffer: failed to create buffer object");
            KMP_ASSERT(_buffer);

            auto bufferMemoryContext = memoryTypeDelegate.GetBufferMemoryContext(_device, _buffer, parameters.memoryPropertyFlags);
            VkMemoryAllocateFlagsInfoKHR allocateFlagsInfo = VKUtils::InitVkMemoryAllocateFlagsInfoKHR();
            if (_usageFlags & VK_BufferUsage_ShaderDeviceAddress)
            {
                allocateFlagsInfo.flags = VK_MemoryAllocate_DeviceAddress;
                bufferMemoryContext.allocateInfo.pNext = &allocateFlagsInfo;
            }

            result = vkAllocateMemory(_device, &bufferMemoryContext.allocateInfo, nullptr, &_memory);
            VKUtils::CheckResult(result, "VulkanBuffer: failed to allocate buffer memory");
            KMP_ASSERT(_memory);

            result = Bind();
            VKUtils::CheckResult(result, "VulkanBuffer: failed to bind buffer");
        }}
        //--------------------------------------------------------------------------

        void VulkanBuffer::_Finalize() KMP_PROFILING(ProfileLevelAlways)
        {
            if (_buffer && _device)
            {
                vkDestroyBuffer(_device, _buffer, nullptr);
            }

            if (_memory && _device)
            {
                vkFreeMemory(_device, _memory, nullptr);
            }
        }}
        //--------------------------------------------------------------------------
    }
}