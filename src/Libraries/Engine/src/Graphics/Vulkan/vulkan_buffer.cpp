#include "Kmplete/Graphics/Vulkan/vulkan_buffer.h"
#include "Kmplete/Graphics/Vulkan/vulkan_result_description.h"
#include "Kmplete/Core/assertion.h"
#include "Kmplete/Log/log.h"


namespace Kmplete
{
    namespace Graphics
    {
        VulkanBuffer::VulkanBuffer(VkDevice device, const PhysicalDeviceInfo& physicalDeviceInfo, VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags memoryPropertyFlags, VkDeviceSize size, void* data)
            : _device(device)
            , _physicalDeviceInfo(physicalDeviceInfo)
            , _buffer(VK_NULL_HANDLE)
            , _memory(VK_NULL_HANDLE)
            , _descriptor()
            , _size(size)
            , _alignment(0)
            , _mapped(nullptr)
            , _usageFlags(usageFlags)
            , _memoryPropertyFlags(memoryPropertyFlags)
        {
            VkBufferCreateInfo createInfo{};
            createInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
            createInfo.usage = _usageFlags;
            createInfo.size = _size;

            auto result = vkCreateBuffer(_device, &createInfo, nullptr, &_buffer);
            if (result != VK_SUCCESS)
            {
                const auto resultDescription = VkResultToString(result);
                KMP_LOG_CRITICAL("failed to create buffer object: {}", resultDescription);
                throw std::runtime_error(String("VulkanBuffer: failed to create buffer object: ").append(resultDescription));
            }

            VkMemoryRequirements memoryRequirements;
            VkMemoryAllocateInfo memoryAllocateInfo{};
            memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
            vkGetBufferMemoryRequirements(_device, _buffer, &memoryRequirements);
            memoryAllocateInfo.allocationSize = memoryRequirements.size;
            memoryAllocateInfo.memoryTypeIndex = _physicalDeviceInfo.FindMemoryType(memoryRequirements.memoryTypeBits, _memoryPropertyFlags);

            VkMemoryAllocateFlagsInfoKHR allocateFlagsInfo{};
            if (_usageFlags & VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT)
            {
                allocateFlagsInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_FLAGS_INFO_KHR;
                allocateFlagsInfo.flags = VK_MEMORY_ALLOCATE_DEVICE_ADDRESS_BIT_KHR;
                memoryAllocateInfo.pNext = &allocateFlagsInfo;
            }

            result = vkAllocateMemory(_device, &memoryAllocateInfo, nullptr, &_memory);
            if (result != VK_SUCCESS)
            {
                const auto resultDescription = VkResultToString(result);
                KMP_LOG_CRITICAL("failed to allocate buffer memory: {}", resultDescription);
                throw std::runtime_error(String("VulkanBuffer: failed to allocate buffer memory: ").append(resultDescription));
            }

            _alignment = memoryRequirements.alignment;

            if (data != nullptr)
            {
                result = Map();
                if (result != VK_SUCCESS)
                {
                    const auto resultDescription = VkResultToString(result);
                    KMP_LOG_CRITICAL("failed to map buffer memory: {}", resultDescription);
                    throw std::runtime_error(String("VulkanBuffer: failed to map buffer memory: ").append(resultDescription));
                }

                memcpy(_mapped, data, size);
                if ((_memoryPropertyFlags & VK_MEMORY_PROPERTY_HOST_COHERENT_BIT) == 0)
                {
                    Flush();
                }

                Unmap();
            }

            _SetupDescriptor();

            result = Bind();
            if (result != VK_SUCCESS)
            {
                const auto resultDescription = VkResultToString(result);
                KMP_LOG_CRITICAL("failed to bind buffer: {}", resultDescription);
                throw std::runtime_error(String("VulkanBuffer: failed to bind buffer: ").append(resultDescription));
            }
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
            VkMappedMemoryRange mappedRange{};
            mappedRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
            mappedRange.memory = _memory;
            mappedRange.offset = offset;
            mappedRange.size = size;

            return vkFlushMappedMemoryRanges(_device, 1, &mappedRange);
        }
        //--------------------------------------------------------------------------

        VkResult VulkanBuffer::Invalidate(VkDeviceSize size /*= VK_WHOLE_SIZE*/, VkDeviceSize offset /*= 0*/)
        {
            VkMappedMemoryRange mappedRange{};
            mappedRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
            mappedRange.memory = _memory;
            mappedRange.offset = offset;
            mappedRange.size = size;

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