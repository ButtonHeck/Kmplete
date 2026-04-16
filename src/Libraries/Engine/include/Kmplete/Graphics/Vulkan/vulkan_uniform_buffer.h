#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Base/type_traits.h"
#include "Kmplete/Graphics/Vulkan/vulkan_buffer.h"
#include "Kmplete/Log/log_class_macro.h"

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
            KMP_LOG_CLASSNAME(VulkanUniformBuffer)

        public:
            KMP_API VulkanUniformBuffer(const VulkanMemoryTypeDelegate& memoryTypeDelegate, VkDevice device, const VulkanBufferParameters& parameters, 
                                        VkDescriptorPool descriptorPool, const Vector<VkDescriptorSetLayout>& descriptorSetLayouts, UInt32 binding);
            KMP_API VulkanUniformBuffer(VulkanUniformBuffer&& other) noexcept;
            KMP_API VulkanUniformBuffer& operator=(VulkanUniformBuffer&& other) noexcept;

            KMP_NODISCARD KMP_API VkDescriptorSet GetVkDescriptorSet(UInt32 index) const noexcept;

            KMP_API void SetCombinedImageSamplerDescriptor(VkImageView imageView, VkSampler sampler, UInt32 setIndex, UInt32 binding, UInt32 count = 1);
            KMP_API void SetSampledImageDescriptor(VkImageView imageView, UInt32 setIndex, UInt32 binding, UInt32 count = 1);
            KMP_API void SetSamplerDescriptor(VkSampler sampler, UInt32 setIndex, UInt32 binding, UInt32 count = 1);

        private:
            void _AllocateDescriptorSets(VkDescriptorPool descriptorPool, const Vector<VkDescriptorSetLayout>& descriptorSetLayouts);
            void _UpdateDescriptorSet(const VkDescriptorBufferInfo& bufferInfo, VkDescriptorType type, UInt32 setIndex, UInt32 binding, UInt32 count = 1);
            void _UpdateDescriptorSet(const VkDescriptorImageInfo& imageInfo, VkDescriptorType type, UInt32 setIndex, UInt32 binding, UInt32 count = 1);

        private:
            Vector<VkDescriptorSet> _descriptorSets;
        };
        //--------------------------------------------------------------------------

        static_assert(IsMoveConstructible<VulkanUniformBuffer>::value);
        static_assert(IsMoveAssignable<VulkanUniformBuffer>::value);
    }
}