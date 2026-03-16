#include "Kmplete/Graphics/Vulkan/vulkan_vertex_buffer.h"
#include "Kmplete/Graphics/Vulkan/Delegates/vulkan_memory_type_delegate.h"


namespace Kmplete
{
    namespace Graphics
    {
        VulkanVertexBuffer::VulkanVertexBuffer(const VulkanMemoryTypeDelegate& memoryTypeDelegate, VkDevice device, VkBufferUsageFlags usageFlags, 
                                               VkMemoryPropertyFlags memoryPropertyFlags, VkDeviceSize size, const BufferLayout& layout)
            : VulkanBuffer(memoryTypeDelegate, device, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | usageFlags, memoryPropertyFlags, size)
            , _layout(layout)
        {}
        //--------------------------------------------------------------------------

        VulkanVertexBuffer::VulkanVertexBuffer(VulkanVertexBuffer&& other) noexcept
            : VulkanBuffer(std::move(other))
            , _layout(other._layout)
        {
            other._layout = BufferLayout();
        }
        //--------------------------------------------------------------------------

        VulkanVertexBuffer& VulkanVertexBuffer::operator=(VulkanVertexBuffer&& other) noexcept
        {
            if (this == &other)
            {
                return *this;
            }

            VulkanBuffer::operator=(std::move(other));

            _layout = other._layout;
            other._layout = BufferLayout();

            return *this;
        }
        //--------------------------------------------------------------------------
    }
}