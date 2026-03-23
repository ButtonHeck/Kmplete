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
            KMP_API VulkanUniformBuffer(const VulkanMemoryTypeDelegate& memoryTypeDelegate, VkDevice device, const VulkanBufferParameters& parameters, 
                                        VkDescriptorPool descriptorPool, const Vector<VkDescriptorSetLayout>& descriptorSetLayouts, UInt32 binding);
            KMP_API VulkanUniformBuffer(VulkanUniformBuffer&& other) noexcept;
            KMP_API VulkanUniformBuffer& operator=(VulkanUniformBuffer&& other) noexcept;

            KMP_NODISCARD KMP_API const VkDescriptorSet& GetVkDescriptorSet() const noexcept;

        private:
            void _AllocateDescriptorSet(VkDescriptorPool descriptorPool, const Vector<VkDescriptorSetLayout>& descriptorSetLayouts);
            void _UpdateDescriptorSet(UInt32 binding, VkDeviceSize size);

        private:
            VkDescriptorSet _descriptorSet;
        };
        //--------------------------------------------------------------------------

        static_assert(IsMoveConstructible<VulkanUniformBuffer>::value);
        static_assert(IsMoveAssignable<VulkanUniformBuffer>::value);
    }
}