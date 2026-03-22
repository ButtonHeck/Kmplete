#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Base/nullability.h"
#include "Kmplete/Graphics/Vulkan/vulkan_buffer.h"
#include "Kmplete/Graphics/Vulkan/vulkan_vertex_buffer.h"
#include "Kmplete/Graphics/Vulkan/vulkan_uniform_buffer.h"

#include <vulkan/vulkan.h>


namespace Kmplete
{
    namespace Graphics
    {
        class VulkanMemoryTypeDelegate;


        //TODO: comments
        class VulkanBufferCreatorDelegate
        {
            KMP_DISABLE_COPY_MOVE(VulkanBufferCreatorDelegate)

        public:
            KMP_API VulkanBufferCreatorDelegate(VkDevice device, VkDescriptorPool descriptorPool, const VulkanMemoryTypeDelegate& memoryTypeDelegate);

            KMP_NODISCARD KMP_API VulkanBuffer CreateBuffer(VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags memoryPropertyFlags, VkDeviceSize size) const;
            KMP_NODISCARD KMP_API Nullable<VulkanBuffer*> CreateBufferPtr(VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags memoryPropertyFlags, VkDeviceSize size) const;

            KMP_NODISCARD KMP_API VulkanVertexBuffer CreateVertexBuffer(VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags memoryPropertyFlags, VkDeviceSize size) const;
            KMP_NODISCARD KMP_API Nullable<VulkanVertexBuffer*> CreateVertexBufferPtr(VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags memoryPropertyFlags, VkDeviceSize size) const;

            KMP_NODISCARD KMP_API VulkanUniformBuffer CreateUniformBuffer(VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags memoryPropertyFlags, VkDeviceSize size, const Vector<VkDescriptorSetLayout>& descriptorSetLayouts, UInt32 binding) const;
            KMP_NODISCARD KMP_API Nullable<VulkanUniformBuffer*> CreateUniformBufferPtr(VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags memoryPropertyFlags, VkDeviceSize size, const Vector<VkDescriptorSetLayout>& descriptorSetLayouts, UInt32 binding) const;

        private:
            const VulkanMemoryTypeDelegate& _memoryTypeDelegate;

            VkDevice _device;
            VkDescriptorPool _descriptorPool;
        };
        //--------------------------------------------------------------------------
    }
}