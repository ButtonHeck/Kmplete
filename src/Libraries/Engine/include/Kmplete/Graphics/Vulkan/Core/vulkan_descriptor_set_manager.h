#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Base/string_id.h"
#include "Kmplete/Graphics/graphics_base.h"
#include "Kmplete/Log/log_class_macro.h"
#include "Kmplete/Profile/profiler_fwd.h"

#include <vulkan/vulkan.h>


namespace Kmplete
{
    namespace Graphics
    {
        class VulkanBuffer;


        //! Manager of Vulkan descriptor sets, pools and layouts objects. Responsible for storing, creating,
        //! deleting these objects, providing interface to interact with them. By default a single descriptor pool
        //! object is allocated during manager creation, but auxiliary pools can be allocated on demand (e.g. a separate pool for ImGUI).
        //! Descriptor sets, layouts and auxiliary pools stored in hashmaps (by StringID as a key). Similar to VulkanBufferManager
        //! this manager separates descriptor sets by per-frame usage and plain descriptor sets.
        //! A set may be updated using either it's StringID or just a VkDescriptorSet handle.
        //! @see VulkanBufferManager
        //! @see StringID
        class KMP_API VulkanDescriptorSetManager
        {
            KMP_DISABLE_COPY_MOVE(VulkanDescriptorSetManager)
            KMP_LOG_CLASSNAME(VulkanDescriptorSetManager)
            KMP_PROFILE_CONSTRUCTOR_DECLARE()

        public:
            //! Shortcut alias for a collection of descriptor sets objects
            using DescriptorSetStorage = StringIDHashMap<Vector<VkDescriptorSet>>;

        public:
            VulkanDescriptorSetManager(VkDevice device, const UInt32& currentBufferIndex, UInt32 maxDescriptorSets, const Vector<VkDescriptorPoolSize>& descriptorPoolSizes);
            ~VulkanDescriptorSetManager();

            KMP_NODISCARD VkDescriptorPool GetVkDescriptorPool() const noexcept;

            void AllocateAuxDescriptorPool(StringID sid, UInt32 maxDescriptorSets, const Vector<VkDescriptorPoolSize>& descriptorPoolSizes);
            KMP_NODISCARD VkDescriptorPool GetAuxDescriptorPool(StringID sid) const noexcept;

            VkDescriptorSetLayout AddDescriptorSetLayout(StringID layoutSid, const Vector<VkDescriptorSetLayoutBinding>& bindings);
            KMP_NODISCARD VkDescriptorSetLayout GetDescriptorSetLayout(StringID layoutSid) const noexcept;
            KMP_NODISCARD Vector<VkDescriptorSetLayout> GetDescriptorSetLayouts(const Vector<StringID>& sids) const noexcept;

            bool AllocateDescriptorSets(StringID layoutSid, StringID setSid, UInt32 setsCount, bool perFrame);
            bool AllocateDescriptorSets(VkDescriptorSetLayout layout, StringID setSid, UInt32 setsCount, bool perFrame);
            KMP_NODISCARD VkDescriptorSet GetDescriptorSet(StringID setSid, UInt32 setIndex, bool perFrame) const noexcept;
            KMP_NODISCARD VkDescriptorSet GetDescriptorSet(StringID setSid, UInt32 setIndex, bool perFrame, UInt32 frameIndex) const noexcept;

            bool SetUniformBufferDescriptor(StringID setSid, UInt32 setIndex, bool perFrame, UInt32 frameIndex, const VulkanBuffer& buffer, VkDeviceSize range, VkDeviceSize offset, UInt32 binding) const;
            bool SetUniformBufferDescriptor(StringID setSid, UInt32 setIndex, bool perFrame, UInt32 frameIndex, VkBuffer buffer, VkDeviceSize range, VkDeviceSize offset, UInt32 binding) const;
            bool SetUniformBufferDescriptor(VkDescriptorSet descriptorSet, VkBuffer buffer, VkDeviceSize range, VkDeviceSize offset, UInt32 binding) const;

            bool SetUniformBufferDynamicDescriptor(StringID setSid, UInt32 setIndex, bool perFrame, UInt32 frameIndex, const VulkanBuffer& buffer, VkDeviceSize range, VkDeviceSize offset, UInt32 binding) const;
            bool SetUniformBufferDynamicDescriptor(StringID setSid, UInt32 setIndex, bool perFrame, UInt32 frameIndex, VkBuffer buffer, VkDeviceSize range, VkDeviceSize offset, UInt32 binding) const;
            bool SetUniformBufferDynamicDescriptor(VkDescriptorSet descriptorSet, VkBuffer buffer, VkDeviceSize range, VkDeviceSize offset, UInt32 binding) const;

            bool SetStorageBufferDescriptor(StringID setSid, UInt32 setIndex, bool perFrame, UInt32 frameIndex, const VulkanBuffer& buffer, VkDeviceSize range, VkDeviceSize offset, UInt32 binding) const;
            bool SetStorageBufferDescriptor(StringID setSid, UInt32 setIndex, bool perFrame, UInt32 frameIndex, VkBuffer buffer, VkDeviceSize range, VkDeviceSize offset, UInt32 binding) const;
            bool SetStorageBufferDescriptor(VkDescriptorSet descriptorSet, VkBuffer buffer, VkDeviceSize range, VkDeviceSize offset, UInt32 binding) const;

            bool SetStorageBufferDynamicDescriptor(StringID setSid, UInt32 setIndex, bool perFrame, UInt32 frameIndex, const VulkanBuffer& buffer, VkDeviceSize range, VkDeviceSize offset, UInt32 binding) const;
            bool SetStorageBufferDynamicDescriptor(StringID setSid, UInt32 setIndex, bool perFrame, UInt32 frameIndex, VkBuffer buffer, VkDeviceSize range, VkDeviceSize offset, UInt32 binding) const;
            bool SetStorageBufferDynamicDescriptor(VkDescriptorSet descriptorSet, VkBuffer buffer, VkDeviceSize range, VkDeviceSize offset, UInt32 binding) const;

            bool SetCombinedImageSamplerDescriptor(StringID setSid, UInt32 setIndex, bool perFrame, VkImageView imageView, VkSampler sampler, UInt32 binding) const;
            bool SetCombinedImageSamplerDescriptor(StringID setSid, UInt32 setIndex, bool perFrame, UInt32 frameIndex, VkImageView imageView, VkSampler sampler, UInt32 binding) const;
            bool SetCombinedImageSamplerDescriptor(VkDescriptorSet descriptorSet, VkImageView imageView, VkSampler sampler, UInt32 binding) const;

            bool SetSampledImageDescriptor(StringID setSid, UInt32 setIndex, bool perFrame, VkImageView imageView, UInt32 binding) const;
            bool SetSampledImageDescriptor(StringID setSid, UInt32 setIndex, bool perFrame, UInt32 frameIndex, VkImageView imageView, UInt32 binding) const;
            bool SetSampledImageDescriptor(VkDescriptorSet descriptorSet, VkImageView imageView, UInt32 binding) const;

            bool SetSamplerDescriptor(StringID setSid, UInt32 setIndex, bool perFrame, VkSampler sampler, UInt32 binding) const;
            bool SetSamplerDescriptor(StringID setSid, UInt32 setIndex, bool perFrame, UInt32 frameIndex, VkSampler sampler, UInt32 binding) const;
            bool SetSamplerDescriptor(VkDescriptorSet descriptorSet, VkSampler sampler, UInt32 binding) const;

        private:
            void _Initialize(UInt32 maxDescriptorSets, const Vector<VkDescriptorPoolSize>& descriptorPoolSizes);
            void _Finalize();

            KMP_NODISCARD bool _AllocateDescriptorSets(const Vector<VkDescriptorSetLayout>& layouts, StringID setSid, UInt32 setsCount, DescriptorSetStorage& storage) const;
            KMP_NODISCARD VkDescriptorSet _GetDescriptorSet(const DescriptorSetStorage& storage, StringID setSid, UInt32 setIndex) const noexcept;
            KMP_NODISCARD VkWriteDescriptorSet _GetWriteDescriptorSetTemplate(VkDescriptorSet descriptorSet, VkDescriptorType type, UInt32 binding) const noexcept;

            void _UpdateDescriptorSet(VkDescriptorSet descriptorSet, const VkDescriptorBufferInfo& bufferInfo, VkDescriptorType type, UInt32 binding) const;
            void _UpdateDescriptorSet(VkDescriptorSet descriptorSet, const VkDescriptorImageInfo& imageInfo, VkDescriptorType type, UInt32 binding) const;

        private:
            const UInt32& _currentBufferIndex;
            VkDevice _device;
            VkDescriptorPool _descriptorPool;
            StringIDHashMap<VkDescriptorPool> _auxDescriptorPools;
            StringIDHashMap<VkDescriptorSetLayout> _descriptorSetLayouts;

            Array<DescriptorSetStorage, NumConcurrentFrames> _descriptorsPerFrame;
            DescriptorSetStorage _descriptors;
        };
        //--------------------------------------------------------------------------
    }
}