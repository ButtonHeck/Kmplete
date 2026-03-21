#include "Kmplete/Graphics/Vulkan/vulkan_uniform_buffer.h"
#include "Kmplete/Graphics/Vulkan/Delegates/vulkan_memory_type_delegate.h"
#include "Kmplete/Graphics/Vulkan/Utils/initializers.h"


namespace Kmplete
{
    namespace Graphics
    {
        VulkanUniformBuffer::VulkanUniformBuffer(const VulkanMemoryTypeDelegate& memoryTypeDelegate, VkDevice device, VkBufferUsageFlags usageFlags, 
                                                 VkMemoryPropertyFlags memoryPropertyFlags, VkDeviceSize size, VkDescriptorPool descriptorPool)
            : VulkanBuffer(memoryTypeDelegate, device, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | usageFlags, memoryPropertyFlags, size)
            , _descriptorSet(VK_NULL_HANDLE)
            , _descriptorPool(descriptorPool)
        {}
        //--------------------------------------------------------------------------

        VulkanUniformBuffer::VulkanUniformBuffer(VulkanUniformBuffer&& other) noexcept
            : VulkanBuffer(std::move(other))
            , _descriptorSet(other._descriptorSet)
            , _descriptorPool(other._descriptorPool)
        {
            other._descriptorSet = VK_NULL_HANDLE;
            other._descriptorPool = VK_NULL_HANDLE;
        }
        //--------------------------------------------------------------------------

        VulkanUniformBuffer& VulkanUniformBuffer::operator=(VulkanUniformBuffer&& other) noexcept
        {
            if (this == &other)
            {
                return *this;
            }

            VulkanBuffer::operator=(std::move(other));

            _descriptorSet = other._descriptorSet;
            _descriptorPool = other._descriptorPool;

            other._descriptorSet = VK_NULL_HANDLE;
            other._descriptorPool = VK_NULL_HANDLE;

            return *this;
        }
        //--------------------------------------------------------------------------

        VkDescriptorSet& VulkanUniformBuffer::GetVkDescriptorSet() noexcept
        {
            return _descriptorSet;
        }
        //--------------------------------------------------------------------------

        const VkDescriptorSet& VulkanUniformBuffer::GetVkDescriptorSet() const noexcept
        {
            return _descriptorSet;
        }
        //--------------------------------------------------------------------------
    }
}