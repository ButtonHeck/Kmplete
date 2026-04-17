#include "Kmplete/Graphics/Vulkan/vulkan_uniform_buffer.h"
#include "Kmplete/Graphics/Vulkan/Delegates/vulkan_memory_type_delegate.h"


namespace Kmplete
{
    namespace Graphics
    {
        VulkanUniformBuffer::VulkanUniformBuffer(const VulkanMemoryTypeDelegate& memoryTypeDelegate, VkDevice device, const VulkanBufferParameters& parameters)
            : VulkanBuffer(memoryTypeDelegate, device, parameters)
        {}
        //--------------------------------------------------------------------------

        VulkanUniformBuffer::VulkanUniformBuffer(VulkanUniformBuffer&& other) noexcept
            : VulkanBuffer(std::move(other))
        {}
        //--------------------------------------------------------------------------

        VulkanUniformBuffer& VulkanUniformBuffer::operator=(VulkanUniformBuffer&& other) noexcept
        {
            if (this == &other)
            {
                return *this;
            }

            VulkanBuffer::operator=(std::move(other));

            return *this;
        }
        //--------------------------------------------------------------------------
    }
}