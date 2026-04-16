#include "Kmplete/Graphics/Vulkan/vulkan_uniform_buffer.h"
#include "Kmplete/Graphics/Vulkan/Delegates/vulkan_memory_type_delegate.h"
#include "Kmplete/Graphics/Vulkan/Utils/initializers.h"
#include "Kmplete/Graphics/Vulkan/Utils/result_description.h"
#include "Kmplete/Profile/profiler.h"
#include "Kmplete/Log/log.h"


namespace Kmplete
{
    namespace Graphics
    {
        VulkanUniformBuffer::VulkanUniformBuffer(const VulkanMemoryTypeDelegate& memoryTypeDelegate, VkDevice device, const VulkanBufferParameters& parameters, 
                                                 VkDescriptorPool descriptorPool, const Vector<VkDescriptorSetLayout>& descriptorSetLayouts, UInt32 binding)
            : VulkanBuffer(memoryTypeDelegate, device, parameters)
            , _descriptorSets()
        {
            KMP_PROFILE_FUNCTION(ProfileLevelAlways);

            _AllocateDescriptorSets(descriptorPool, descriptorSetLayouts);

            VkDescriptorBufferInfo bufferInfo{};
            bufferInfo.buffer = _buffer;
            bufferInfo.range = parameters.size;

            _UpdateDescriptorSet(bufferInfo, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 0, binding);
        }
        //--------------------------------------------------------------------------

        VulkanUniformBuffer::VulkanUniformBuffer(VulkanUniformBuffer&& other) noexcept
            : VulkanBuffer(std::move(other))
            , _descriptorSets(other._descriptorSets)
        {
            other._descriptorSets.clear();
        }
        //--------------------------------------------------------------------------

        VulkanUniformBuffer& VulkanUniformBuffer::operator=(VulkanUniformBuffer&& other) noexcept
        {
            if (this == &other)
            {
                return *this;
            }

            VulkanBuffer::operator=(std::move(other));

            _descriptorSets = other._descriptorSets;

            other._descriptorSets.clear();

            return *this;
        }
        //--------------------------------------------------------------------------

        VkDescriptorSet VulkanUniformBuffer::GetVkDescriptorSet(UInt32 index) const noexcept
        {
            if (index >= _descriptorSets.size())
            {
                KMP_LOG_ERROR("cannot get VkDescriptorSet at index '{}' - out of range", index);
                return VK_NULL_HANDLE;
            }

            return _descriptorSets[index];
        }
        //--------------------------------------------------------------------------

        void VulkanUniformBuffer::SetCombinedImageSamplerDescriptor(VkImageView imageView, VkSampler sampler, UInt32 setIndex, UInt32 binding, UInt32 count /*= 1*/) KMP_PROFILING(ProfileLevelImportant)
        {
            VkDescriptorImageInfo descriptorInfo{};
            descriptorInfo.imageView = imageView;
            descriptorInfo.sampler = sampler;
            descriptorInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

            _UpdateDescriptorSet(descriptorInfo, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, setIndex, binding, count);
        }}
        //--------------------------------------------------------------------------

        void VulkanUniformBuffer::SetSampledImageDescriptor(VkImageView imageView, UInt32 setIndex, UInt32 binding, UInt32 count /*= 1*/) KMP_PROFILING(ProfileLevelImportant)
        {
            VkDescriptorImageInfo descriptorInfo{};
            descriptorInfo.imageView = imageView;
            descriptorInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

            _UpdateDescriptorSet(descriptorInfo, VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, setIndex, binding, count);
        }}
        //--------------------------------------------------------------------------

        void VulkanUniformBuffer::SetSamplerDescriptor(VkSampler sampler, UInt32 setIndex, UInt32 binding, UInt32 count /*= 1*/) KMP_PROFILING(ProfileLevelImportant)
        {
            VkDescriptorImageInfo descriptorInfo{};
            descriptorInfo.sampler = sampler;

            _UpdateDescriptorSet(descriptorInfo, VK_DESCRIPTOR_TYPE_SAMPLER, setIndex, binding, count);
        }}
        //--------------------------------------------------------------------------

        void VulkanUniformBuffer::_AllocateDescriptorSets(VkDescriptorPool descriptorPool, const Vector<VkDescriptorSetLayout>& descriptorSetLayouts) KMP_PROFILING(ProfileLevelImportant)
        {
            auto descriptorSetAllocateInfo = VulkanUtils::InitVkDescriptorSetAllocateInfo();
            descriptorSetAllocateInfo.descriptorPool = descriptorPool;
            descriptorSetAllocateInfo.descriptorSetCount = UInt32(descriptorSetLayouts.size());
            descriptorSetAllocateInfo.pSetLayouts = descriptorSetLayouts.data();

            _descriptorSets.resize(descriptorSetLayouts.size());
            const auto result = vkAllocateDescriptorSets(_device, &descriptorSetAllocateInfo, _descriptorSets.data());
            VulkanUtils::CheckResult(result, "VulkanUniformBuffer: failed to allocate '{}' descriptor sets", UInt32(descriptorSetLayouts.size()));
        }}
        //--------------------------------------------------------------------------

        void VulkanUniformBuffer::_UpdateDescriptorSet(const VkDescriptorBufferInfo& bufferInfo, VkDescriptorType type, UInt32 setIndex, UInt32 binding, UInt32 count /*= 1*/) KMP_PROFILING(ProfileLevelImportant)
        {
            auto writeDescriptorSet = VulkanUtils::InitVkWriteDescriptorSet();
            writeDescriptorSet.dstSet = _descriptorSets[setIndex];
            writeDescriptorSet.dstBinding = binding;
            writeDescriptorSet.descriptorCount = count;
            writeDescriptorSet.descriptorType = type;
            writeDescriptorSet.pBufferInfo = &bufferInfo;
            vkUpdateDescriptorSets(_device, 1, &writeDescriptorSet, 0, nullptr);
        }}
        //--------------------------------------------------------------------------

        void VulkanUniformBuffer::_UpdateDescriptorSet(const VkDescriptorImageInfo& imageInfo, VkDescriptorType type, UInt32 setIndex, UInt32 binding, UInt32 count /*= 1*/) KMP_PROFILING(ProfileLevelImportant)
        {
            auto writeDescriptorSet = VulkanUtils::InitVkWriteDescriptorSet();
            writeDescriptorSet.dstSet = _descriptorSets[setIndex];
            writeDescriptorSet.dstBinding = binding;
            writeDescriptorSet.descriptorCount = count;
            writeDescriptorSet.descriptorType = type;
            writeDescriptorSet.pImageInfo = &imageInfo;
            vkUpdateDescriptorSets(_device, 1, &writeDescriptorSet, 0, nullptr);
        }}
        //--------------------------------------------------------------------------
    }
}