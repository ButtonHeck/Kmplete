#include "Kmplete/Graphics/Vulkan/vulkan_uniform_buffer.h"
#include "Kmplete/Graphics/Vulkan/Delegates/vulkan_memory_type_delegate.h"
#include "Kmplete/Graphics/Vulkan/Utils/initializers.h"
#include "Kmplete/Graphics/Vulkan/Utils/result_description.h"
#include "Kmplete/Profile/profiler.h"


namespace Kmplete
{
    namespace Graphics
    {
        VulkanUniformBuffer::VulkanUniformBuffer(const VulkanMemoryTypeDelegate& memoryTypeDelegate, VkDevice device, const VulkanBufferParameters& parameters, 
                                                 VkDescriptorPool descriptorPool, const Vector<VkDescriptorSetLayout>& descriptorSetLayouts, UInt32 binding)
            : VulkanBuffer(memoryTypeDelegate, device, parameters)
            , _descriptorSet(VK_NULL_HANDLE)
        {
            KMP_PROFILE_FUNCTION(ProfileLevelAlways);

            _AllocateDescriptorSet(descriptorPool, descriptorSetLayouts);
            _UpdateDescriptorSet(binding, parameters.size);
        }
        //--------------------------------------------------------------------------

        VulkanUniformBuffer::VulkanUniformBuffer(VulkanUniformBuffer&& other) noexcept
            : VulkanBuffer(std::move(other))
            , _descriptorSet(other._descriptorSet)
        {
            other._descriptorSet = VK_NULL_HANDLE;
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

            other._descriptorSet = VK_NULL_HANDLE;

            return *this;
        }
        //--------------------------------------------------------------------------

        const VkDescriptorSet& VulkanUniformBuffer::GetVkDescriptorSet() const noexcept
        {
            return _descriptorSet;
        }
        //--------------------------------------------------------------------------

        void VulkanUniformBuffer::_AllocateDescriptorSet(VkDescriptorPool descriptorPool, const Vector<VkDescriptorSetLayout>& descriptorSetLayouts) KMP_PROFILING(ProfileLevelImportant)
        {
            auto descriptorSetAllocateInfo = VulkanUtils::InitVkDescriptorSetAllocateInfo();
            descriptorSetAllocateInfo.descriptorPool = descriptorPool;
            descriptorSetAllocateInfo.descriptorSetCount = UInt32(descriptorSetLayouts.size());
            descriptorSetAllocateInfo.pSetLayouts = descriptorSetLayouts.data();

            const auto result = vkAllocateDescriptorSets(_device, &descriptorSetAllocateInfo, &_descriptorSet);
            VulkanUtils::CheckResult(result, "VulkanUniformBuffer: failed to allocate descriptor set");
        }}
        //--------------------------------------------------------------------------

        void VulkanUniformBuffer::_UpdateDescriptorSet(UInt32 binding, VkDeviceSize size) KMP_PROFILING(ProfileLevelImportant)
        {
            VkDescriptorBufferInfo bufferInfo{};
            bufferInfo.buffer = _buffer;
            bufferInfo.range = size;

            auto writeDescriptorSet = VulkanUtils::InitVkWriteDescriptorSet();
            writeDescriptorSet.dstSet = _descriptorSet;
            writeDescriptorSet.dstBinding = binding;
            writeDescriptorSet.descriptorCount = 1;
            writeDescriptorSet.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            writeDescriptorSet.pBufferInfo = &bufferInfo;
            vkUpdateDescriptorSets(_device, 1, &writeDescriptorSet, 0, nullptr);
        }}
        //--------------------------------------------------------------------------
    }
}