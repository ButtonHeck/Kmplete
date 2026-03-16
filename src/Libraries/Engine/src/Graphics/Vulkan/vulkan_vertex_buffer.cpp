#include "Kmplete/Graphics/Vulkan/vulkan_vertex_buffer.h"
#include "Kmplete/Graphics/Vulkan/Delegates/vulkan_memory_type_delegate.h"
#include "Kmplete/Graphics/Vulkan/vulkan_graphics_base.h"
#include "Kmplete/Profile/profiler.h"


namespace Kmplete
{
    namespace Graphics
    {
        VulkanVertexBuffer::VulkanVertexBuffer(const VulkanMemoryTypeDelegate& memoryTypeDelegate, VkDevice device, VkBufferUsageFlags usageFlags, 
                                               VkMemoryPropertyFlags memoryPropertyFlags, VkDeviceSize size)
            : VulkanBuffer(memoryTypeDelegate, device, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | usageFlags, memoryPropertyFlags, size)
            , _layouts()
        {}
        //--------------------------------------------------------------------------

        VulkanVertexBuffer::VulkanVertexBuffer(VulkanVertexBuffer&& other) noexcept
            : VulkanBuffer(std::move(other))
            , _layouts(std::move(other._layouts))
        {}
        //--------------------------------------------------------------------------

        VulkanVertexBuffer& VulkanVertexBuffer::operator=(VulkanVertexBuffer&& other) noexcept
        {
            if (this == &other)
            {
                return *this;
            }

            VulkanBuffer::operator=(std::move(other));

            _layouts = std::move(other._layouts);

            return *this;
        }
        //--------------------------------------------------------------------------

        void VulkanVertexBuffer::AddLayout(const BufferLayout& layout)
        {
            _layouts.push_back(layout);
        }
        //--------------------------------------------------------------------------

        UInt32 VulkanVertexBuffer::LayoutCount() const noexcept
        {
            return UInt32(_layouts.size());
        }
        //--------------------------------------------------------------------------

        std::pair<Vector<VkVertexInputBindingDescription>, Vector<VkVertexInputAttributeDescription>> VulkanVertexBuffer::GetBindingsDescriptions(UInt32 baseBinding) const noexcept
        {
            KMP_PROFILE_FUNCTION(ProfileLevelMinorFunctions);

            Vector<VkVertexInputBindingDescription> inputBindingsDescriptions(_layouts.size());
            Vector<VkVertexInputAttributeDescription> attributeDescriptions;

            for (size_t i = 0; i < _layouts.size(); i++)
            {
                const auto binding = UInt32(baseBinding + i);
                const auto& layout = _layouts[i];

                inputBindingsDescriptions[i].stride = layout.GetStride();
                inputBindingsDescriptions[i].binding = binding;
                inputBindingsDescriptions[i].inputRate = layout.IsInstanced() ? VK_VERTEX_INPUT_RATE_INSTANCE : VK_VERTEX_INPUT_RATE_VERTEX;

                const auto& elements = layout.GetElements();
                for (const auto& element : elements)
                {
                    attributeDescriptions.push_back(VkVertexInputAttributeDescription{
                        .location = element.location,
                        .binding = binding,
                        .format = ShaderDataTypeToVkFormat(element.type),
                        .offset = UInt32(element.offset)
                    });
                }
            }

            return { inputBindingsDescriptions, attributeDescriptions };
        }
        //--------------------------------------------------------------------------
    }
}