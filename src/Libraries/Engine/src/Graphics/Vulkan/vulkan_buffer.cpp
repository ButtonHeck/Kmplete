#include "Kmplete/Graphics/Vulkan/vulkan_buffer.h"
#include "Kmplete/Graphics/Vulkan/Utils/initializers.h"
#include "Kmplete/Graphics/Vulkan/Utils/function_utils.h"
#include "Kmplete/Core/assertion.h"
#include "Kmplete/Log/log.h"


namespace Kmplete
{
    namespace Graphics
    {
        VulkanBuffer::VulkanBuffer(const PhysicalDeviceInfo& physicalDeviceInfo, VkDevice device, VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags memoryPropertyFlags, VkDeviceSize size, void* data)
            : _physicalDeviceInfo(physicalDeviceInfo)
            , _device(device)
            , _buffer(VK_NULL_HANDLE)
            , _memory(VK_NULL_HANDLE)
            , _descriptor()
            , _size(size)
            , _alignment(0)
            , _mapped(nullptr)
            , _usageFlags(usageFlags)
            , _memoryPropertyFlags(memoryPropertyFlags)
        {
            auto bufferCreateInfo = VulkanUtils::GetVkBufferCreateInfo(_size, _usageFlags);

            auto result = vkCreateBuffer(_device, &bufferCreateInfo, nullptr, &_buffer);
            VulkanUtils::CheckResult(result, "VulkanBuffer: failed to create buffer object");

            VkMemoryRequirements memoryRequirements;
            vkGetBufferMemoryRequirements(_device, _buffer, &memoryRequirements);

            auto memoryAllocateInfo = VulkanUtils::GetVkMemoryAllocateInfo();
            memoryAllocateInfo.allocationSize = memoryRequirements.size;
            memoryAllocateInfo.memoryTypeIndex = _physicalDeviceInfo.FindMemoryType(memoryRequirements.memoryTypeBits, _memoryPropertyFlags);

            VkMemoryAllocateFlagsInfoKHR allocateFlagsInfo = VulkanUtils::GetVkMemoryAllocateFlagsInfoKHR();
            if (_usageFlags & VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT)
            {
                allocateFlagsInfo.flags = VK_MEMORY_ALLOCATE_DEVICE_ADDRESS_BIT_KHR;
                memoryAllocateInfo.pNext = &allocateFlagsInfo;
            }

            result = vkAllocateMemory(_device, &memoryAllocateInfo, nullptr, &_memory);
            VulkanUtils::CheckResult(result, "VulkanBuffer: failed to allocate buffer memory");

            _alignment = memoryRequirements.alignment;

            if (data != nullptr)
            {
                result = Map();
                VulkanUtils::CheckResult(result, "VulkanBuffer: failed to map buffer memory");

                memcpy(_mapped, data, size);
                if ((_memoryPropertyFlags & VK_MEMORY_PROPERTY_HOST_COHERENT_BIT) == 0)
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
            Destroy();
        }
        //--------------------------------------------------------------------------

        VkResult VulkanBuffer::Map(VkDeviceSize size /*= VK_WHOLE_SIZE*/, VkDeviceSize offset /*= 0*/)
        {
            return vkMapMemory(_device, _memory, offset, size, 0, &_mapped);
        }
        //--------------------------------------------------------------------------

        void VulkanBuffer::Unmap()
        {
            if (_mapped)
            {
                vkUnmapMemory(_device, _memory);
                _mapped = nullptr;
            }
        }
        //--------------------------------------------------------------------------

        VkResult VulkanBuffer::Bind(VkDeviceSize offset /*= 0*/)
        {
            return vkBindBufferMemory(_device, _buffer, _memory, offset);
        }
        //--------------------------------------------------------------------------

        void VulkanBuffer::CopyTo(void* data, VkDeviceSize size)
        {
            KMP_ASSERT(_mapped);
            memcpy(_mapped, data, size);
        }
        //--------------------------------------------------------------------------

        VkResult VulkanBuffer::Flush(VkDeviceSize size /*= VK_WHOLE_SIZE*/, VkDeviceSize offset /*= 0*/)
        {
            auto mappedRange = VulkanUtils::GetVkMappedMemoryRange(size, offset);
            mappedRange.memory = _memory;

            return vkFlushMappedMemoryRanges(_device, 1, &mappedRange);
        }
        //--------------------------------------------------------------------------

        VkResult VulkanBuffer::Invalidate(VkDeviceSize size /*= VK_WHOLE_SIZE*/, VkDeviceSize offset /*= 0*/)
        {
            auto mappedRange = VulkanUtils::GetVkMappedMemoryRange(size, offset);
            mappedRange.memory = _memory;

            return vkInvalidateMappedMemoryRanges(_device, 1, &mappedRange);
        }
        //--------------------------------------------------------------------------

        void VulkanBuffer::Destroy()
        {
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