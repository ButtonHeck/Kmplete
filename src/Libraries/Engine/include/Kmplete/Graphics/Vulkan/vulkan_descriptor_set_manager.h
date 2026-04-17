#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Base/string_id.h"
#include "Kmplete/Graphics/graphics_base.h"
#include "Kmplete/Log/log_class_macro.h"

#include <vulkan/vulkan.h>


namespace Kmplete
{
    namespace Graphics
    {
        class VulkanBuffer;


        //TODO: comments
        class VulkanDescriptorSetManager
        {
            KMP_DISABLE_COPY_MOVE(VulkanDescriptorSetManager)
            KMP_LOG_CLASSNAME(VulkanDescriptorSetManager)

        public:
            KMP_API VulkanDescriptorSetManager(VkDevice device, const UInt32& currentBufferIndex);
            KMP_API ~VulkanDescriptorSetManager();

            KMP_NODISCARD KMP_API VkDescriptorPool GetVkDescriptorPool() const noexcept;

            KMP_API bool AddDescriptorSetLayout(StringID layoutSid, const Vector<VkDescriptorSetLayoutBinding>& bindings);
            KMP_NODISCARD KMP_API VkDescriptorSetLayout GetDescriptorSetLayout(StringID layoutSid) const noexcept;
            KMP_NODISCARD KMP_API Vector<VkDescriptorSetLayout> GetDescriptorSetLayouts(const Vector<StringID>& sids) const noexcept;

            KMP_API bool AllocateDescriptorSets(StringID layoutSid, StringID setSid, UInt32 setsCount, bool perFrame);
            KMP_NODISCARD KMP_API VkDescriptorSet GetDescriptorSet(StringID setSid, UInt32 setIndex, bool perFrame, UInt32 frameIndex) const noexcept;

            KMP_API bool SetUniformBufferDescriptor(StringID setSid, UInt32 setIndex, bool perFrame, UInt32 frameIndex, const VulkanBuffer& buffer, UInt32 binding) const;
            KMP_API bool SetUniformBufferDescriptor(StringID setSid, UInt32 setIndex, bool perFrame, UInt32 frameIndex, VkBuffer buffer, VkDeviceSize size, UInt32 binding) const;
            KMP_API bool SetCombinedImageSamplerDescriptor(StringID setSid, UInt32 setIndex, bool perFrame, UInt32 frameIndex, VkImageView imageView, VkSampler sampler, UInt32 binding) const;
            KMP_API bool SetSampledImageDescriptor(StringID setSid, UInt32 setIndex, bool perFrame, UInt32 frameIndex, VkImageView imageView, UInt32 binding) const;
            KMP_API bool SetSamplerDescriptor(StringID setSid, UInt32 setIndex, bool perFrame, UInt32 frameIndex, VkSampler sampler, UInt32 binding) const;

        private:
            void _Initialize();
            void _Finalize();

            void _UpdateDescriptorSet(VkDescriptorSet descriptorSet, const VkDescriptorBufferInfo& bufferInfo, VkDescriptorType type, UInt32 binding) const;
            void _UpdateDescriptorSet(VkDescriptorSet descriptorSet, const VkDescriptorImageInfo& imageInfo, VkDescriptorType type, UInt32 binding) const;
            KMP_NODISCARD VkWriteDescriptorSet _GetWriteDescriptorSetTemplate(VkDescriptorSet descriptorSet, VkDescriptorType type, UInt32 binding) const noexcept;

        private:
            const UInt32& _currentBufferIndex;
            VkDevice _device;
            VkDescriptorPool _descriptorPool;
            StringIDHashMap<VkDescriptorSetLayout> _descriptorSetLayouts;

            Array<StringIDHashMap<Vector<VkDescriptorSet>>, NumConcurrentFrames> _descriptorsPerFrame;
            StringIDHashMap<Vector<VkDescriptorSet>> _descriptors;
        };
        //--------------------------------------------------------------------------
    }
}