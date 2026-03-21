#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Base/type_traits.h"
#include "Kmplete/Graphics/Vulkan/vulkan_buffer.h"

#include <vulkan/vulkan.h>


namespace Kmplete
{
    namespace Graphics
    {
        class VulkanMemoryTypeDelegate;


        //TODO: comments
        class VulkanUniformBuffer : public VulkanBuffer
        {
            KMP_DISABLE_COPY(VulkanUniformBuffer)

        public:
            KMP_API VulkanUniformBuffer(const VulkanMemoryTypeDelegate& memoryTypeDelegate, VkDevice device, VkBufferUsageFlags usageFlags,
                                        VkMemoryPropertyFlags memoryPropertyFlags, VkDeviceSize size, VkDescriptorPool descriptorPool);
            KMP_API VulkanUniformBuffer(VulkanUniformBuffer&& other) noexcept;
            KMP_API VulkanUniformBuffer& operator=(VulkanUniformBuffer&& other) noexcept;

            KMP_NODISCARD KMP_API VkDescriptorSet& GetVkDescriptorSet() noexcept;
            KMP_NODISCARD KMP_API const VkDescriptorSet& GetVkDescriptorSet() const noexcept;

        private:
            VkDescriptorSet _descriptorSet;
            VkDescriptorPool _descriptorPool;
        };
        //--------------------------------------------------------------------------

        static_assert(IsMoveConstructible<VulkanUniformBuffer>::value);
        static_assert(IsMoveAssignable<VulkanUniformBuffer>::value);
    }
}