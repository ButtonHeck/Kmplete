#include "Kmplete/Graphics/Vulkan/vulkan_buffer.h"
#include "Kmplete/Graphics/Vulkan/Utils/initializers.h"
#include "Kmplete/Graphics/Vulkan/Utils/function_utils.h"
#include "Kmplete/Core/assertion.h"
#include "Kmplete/Profile/profiler.h"


namespace Kmplete
{
    namespace Graphics
    {
        VulkanBuffer::VulkanBuffer(const VulkanMemoryTypeDelegate& memoryTypeDelegate, VkDevice device, VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags memoryPropertyFlags, VkDeviceSize size, void* data)
            : _device(device)
            , _buffer(VK_NULL_HANDLE)
            , _memory(VK_NULL_HANDLE)
            , _descriptor()
            , _size(size)
            , _alignment(0)
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

            _alignment = bufferMemoryContext.requirements.alignment;

            if (data != nullptr)
            {
                result = Map();
                VulkanUtils::CheckResult(result, "VulkanBuffer: failed to map buffer memory");

                memcpy(_mapped, data, size);
                if ((memoryPropertyFlags & VK_MEMORY_PROPERTY_HOST_COHERENT_BIT) == 0)
                {
                    Flush();
                }

                Unmap();
            }

            _SetupDescriptor();

            result = Bind();
            VulkanUtils::CheckResult(result, "VulkanBuffer: failed to bind buffer");
        }
        //--------------------------------------------------------------------------

        VulkanBuffer::~VulkanBuffer()
        {
            KMP_PROFILE_FUNCTION(ProfileLevelAlways);

            Destroy();
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

        void VulkanBuffer::CopyTo(void* data, VkDeviceSize size)
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

        void VulkanBuffer::Destroy()
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

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

        void VulkanBuffer::_SetupDescriptor(VkDeviceSize size /*= VK_WHOLE_SIZE*/, VkDeviceSize offset /*= 0*/)
        {
            _descriptor.buffer = _buffer;
            _descriptor.offset = offset;
            _descriptor.range = size;
        }
        //--------------------------------------------------------------------------
    }
}