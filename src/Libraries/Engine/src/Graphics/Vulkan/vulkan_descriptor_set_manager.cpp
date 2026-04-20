#include "Kmplete/Graphics/Vulkan/vulkan_descriptor_set_manager.h"
#include "Kmplete/Graphics/Vulkan/vulkan_buffer.h"
#include "Kmplete/Graphics/Vulkan/Utils/initializers.h"
#include "Kmplete/Graphics/Vulkan/Utils/result_description.h"
#include "Kmplete/Graphics/Vulkan/Utils/bits_aliases.h"
#include "Kmplete/Base/named_bool.h"
#include "Kmplete/Utils/vector_utils.h"
#include "Kmplete/Profile/profiler.h"
#include "Kmplete/Log/log.h"


namespace Kmplete
{
    namespace Graphics
    {
        using namespace VKBits;


        VulkanDescriptorSetManager::VulkanDescriptorSetManager(VkDevice device, const UInt32& currentBufferIndex)
            : _currentBufferIndex(currentBufferIndex)
            , _device(device)
            , _descriptorPool(VK_NULL_HANDLE)
            , _descriptorSetLayouts()
        {
            KMP_PROFILE_FUNCTION(ProfileLevelAlways);

            _Initialize();
        }
        //--------------------------------------------------------------------------

        VulkanDescriptorSetManager::~VulkanDescriptorSetManager() KMP_PROFILING(ProfileLevelAlways)
        {
            _Finalize();
        }}
        //--------------------------------------------------------------------------

        VkDescriptorPool VulkanDescriptorSetManager::GetVkDescriptorPool() const noexcept
        {
            return _descriptorPool;
        }
        //--------------------------------------------------------------------------

        VkDescriptorSetLayout VulkanDescriptorSetManager::AddDescriptorSetLayout(StringID layoutSid, const Vector<VkDescriptorSetLayoutBinding>& bindings) KMP_PROFILING(ProfileLevelImportant)
        {
            if (_descriptorSetLayouts.contains(layoutSid))
            {
                KMP_LOG_WARN("descriptor set layout with sid '{}' has already been created", layoutSid);
                return _descriptorSetLayouts[layoutSid];
            }

            try
            {
                auto descriptorSetLayoutCreateInfo = Graphics::VKUtils::InitVkDescriptorSetLayoutCreateInfo();
                descriptorSetLayoutCreateInfo.bindingCount = UInt32(bindings.size());
                descriptorSetLayoutCreateInfo.pBindings = bindings.empty() ? nullptr : bindings.data();
                VkDescriptorSetLayout layout = nullptr;
                const auto result = vkCreateDescriptorSetLayout(_device, &descriptorSetLayoutCreateInfo, nullptr, &layout);
                VKUtils::CheckResult(result, "VulkanDescriptorSetManager: failed to create descriptor set layout");

                const auto [iterator, hasEmplaced] = _descriptorSetLayouts.emplace(layoutSid, layout);
                if (!hasEmplaced)
                {
                    KMP_LOG_ERROR("failed to emplace descriptor set layout to the storage");
                    return VK_NULL_HANDLE;
                }
                
                return layout;
            }
            catch (KMP_MB_UNUSED const std::runtime_error& er)
            {
                KMP_LOG_ERROR("failed to create descriptor set layout '{}'", layoutSid);
                return VK_NULL_HANDLE;
            }
        }}
        //--------------------------------------------------------------------------

        VkDescriptorSetLayout VulkanDescriptorSetManager::GetDescriptorSetLayout(StringID layoutSid) const noexcept
        {
            if (_descriptorSetLayouts.contains(layoutSid))
            {
                return _descriptorSetLayouts.at(layoutSid);
            }

            KMP_LOG_ERROR("descriptor set layout with sid '{}' not found", layoutSid);
            return VK_NULL_HANDLE;
        }
        //--------------------------------------------------------------------------

        Vector<VkDescriptorSetLayout> VulkanDescriptorSetManager::GetDescriptorSetLayouts(const Vector<StringID>& sids) const noexcept KMP_PROFILING(ProfileLevelMinor)
        {
            Vector<VkDescriptorSetLayout> descriptorSetsLayouts;
            descriptorSetsLayouts.reserve(sids.size());
            for (const auto& sid : sids)
            {
                descriptorSetsLayouts.push_back(GetDescriptorSetLayout(sid));
            }

            return descriptorSetsLayouts;
        }}
        //--------------------------------------------------------------------------

        bool VulkanDescriptorSetManager::AllocateDescriptorSets(StringID layoutSid, StringID setSid, UInt32 setsCount, bool perFrame) 
        {
            if (!_descriptorSetLayouts.contains(layoutSid))
            {
                KMP_LOG_ERROR("cannot allocate '{}' descriptor sets with sid '{}' for layout '{}' - layout with given sid not found", setsCount, setSid, layoutSid);
                return false;
            }

            return AllocateDescriptorSets(_descriptorSetLayouts.at(layoutSid), setSid, setsCount, perFrame);
        }
        //--------------------------------------------------------------------------

        bool VulkanDescriptorSetManager::AllocateDescriptorSets(VkDescriptorSetLayout layout, StringID setSid, UInt32 setsCount, bool perFrame) KMP_PROFILING(ProfileLevelImportant)
        {
            if (setsCount == 0)
            {
                KMP_LOG_ERROR("number of descriptor set allocations should not be zero");
                return false;
            }

            const Vector<VkDescriptorSetLayout> layouts(setsCount, layout);

            if (perFrame)
            {
                for (auto& descriptorsPerFrameStorage : _descriptorsPerFrame)
                {
                    if (!_AllocateDescriptorSets(layouts, setSid, setsCount, descriptorsPerFrameStorage))
                    {
                        return false;
                    }
                }

                return true;
            }
            else
            {
                return _AllocateDescriptorSets(layouts, setSid, setsCount, _descriptors);
            }
        }}
        //--------------------------------------------------------------------------

        VkDescriptorSet VulkanDescriptorSetManager::GetDescriptorSet(StringID setSid, UInt32 setIndex, bool perFrame) const noexcept
        {
            return GetDescriptorSet(setSid, setIndex, perFrame, _currentBufferIndex);
        }
        //--------------------------------------------------------------------------

        VkDescriptorSet VulkanDescriptorSetManager::GetDescriptorSet(StringID setSid, UInt32 setIndex, bool perFrame, UInt32 frameIndex) const noexcept
        {
            if (perFrame)
            {
                if (frameIndex >= _descriptorsPerFrame.size())
                {
                    KMP_LOG_ERROR("cannot find per-frame descriptor set with sid '{}' - frame index '{}' is out of range", setSid, frameIndex);
                    return VK_NULL_HANDLE;
                }

                return _GetDescriptorSet(_descriptorsPerFrame[frameIndex], setSid, setIndex);
            }
            else
            {
                return _GetDescriptorSet(_descriptors, setSid, setIndex);
            }
        }
        //--------------------------------------------------------------------------

        bool VulkanDescriptorSetManager::SetUniformBufferDescriptor(StringID setSid, UInt32 setIndex, bool perFrame, UInt32 frameIndex, const VulkanBuffer& buffer, UInt32 binding) const
        {
            return SetUniformBufferDescriptor(setSid, setIndex, perFrame, frameIndex, buffer.GetVkBuffer(), buffer.GetSize(), binding);
        }
        //--------------------------------------------------------------------------

        bool VulkanDescriptorSetManager::SetUniformBufferDescriptor(StringID setSid, UInt32 setIndex, bool perFrame, UInt32 frameIndex, VkBuffer buffer, VkDeviceSize size, UInt32 binding) const KMP_PROFILING(ProfileLevelImportantVerbose)
        {
            auto descriptorSet = GetDescriptorSet(setSid, setIndex, perFrame, frameIndex);
            return SetUniformBufferDescriptor(descriptorSet, buffer, size, binding);
        }}
        //--------------------------------------------------------------------------

        bool VulkanDescriptorSetManager::SetUniformBufferDescriptor(VkDescriptorSet descriptorSet, VkBuffer buffer, VkDeviceSize size, UInt32 binding) const KMP_PROFILING(ProfileLevelImportant)
        {
            if (descriptorSet == VK_NULL_HANDLE)
            {
                KMP_LOG_ERROR("failed to set uniform buffer descriptor - set is null");
                return false;
            }

            VkDescriptorBufferInfo bufferInfo{};
            bufferInfo.buffer = buffer;
            bufferInfo.range = size;

            _UpdateDescriptorSet(descriptorSet, bufferInfo, VK_DescriptorType_UniformBuffer, binding);

            return true;
        }}
        //--------------------------------------------------------------------------

        bool VulkanDescriptorSetManager::SetCombinedImageSamplerDescriptor(StringID setSid, UInt32 setIndex, bool perFrame, VkImageView imageView, VkSampler sampler, UInt32 binding) const KMP_PROFILING(ProfileLevelImportant)
        {
            if (perFrame)
            {
                for (auto& descriptors : _descriptorsPerFrame)
                {
                    const auto descriptorSet = _GetDescriptorSet(descriptors, setSid, setIndex);
                    if (!SetCombinedImageSamplerDescriptor(descriptorSet, imageView, sampler, binding))
                    {
                        return false;
                    }
                }

                return true;
            }
            else
            {
                const auto descriptorSet = _GetDescriptorSet(_descriptors, setSid, setIndex);
                return SetCombinedImageSamplerDescriptor(descriptorSet, imageView, sampler, binding);
            }
        }}
        //--------------------------------------------------------------------------

        bool VulkanDescriptorSetManager::SetCombinedImageSamplerDescriptor(StringID setSid, UInt32 setIndex, bool perFrame, UInt32 frameIndex, VkImageView imageView, VkSampler sampler, UInt32 binding) const KMP_PROFILING(ProfileLevelImportantVerbose)
        {
            auto descriptorSet = GetDescriptorSet(setSid, setIndex, perFrame, frameIndex);
            return SetCombinedImageSamplerDescriptor(descriptorSet, imageView, sampler, binding);
        }}
        //--------------------------------------------------------------------------

        bool VulkanDescriptorSetManager::SetCombinedImageSamplerDescriptor(VkDescriptorSet descriptorSet, VkImageView imageView, VkSampler sampler, UInt32 binding) const KMP_PROFILING(ProfileLevelImportant)
        {
            if (descriptorSet == VK_NULL_HANDLE)
            {
                KMP_LOG_ERROR("failed to set combined image sampler descriptor - set is null");
                return false;
            }

            VkDescriptorImageInfo descriptorInfo{};
            descriptorInfo.imageView = imageView;
            descriptorInfo.sampler = sampler;
            descriptorInfo.imageLayout = VK_ImageLayout_ShaderReadOnlyOptimal;

            _UpdateDescriptorSet(descriptorSet, descriptorInfo, VK_DescriptorType_CombinedImageSampler, binding);

            return true;
        }}
        //--------------------------------------------------------------------------

        bool VulkanDescriptorSetManager::SetSampledImageDescriptor(StringID setSid, UInt32 setIndex, bool perFrame, VkImageView imageView, UInt32 binding) const KMP_PROFILING(ProfileLevelImportant)
        {
            if (perFrame)
            {
                for (auto& descriptors : _descriptorsPerFrame)
                {
                    const auto descriptorSet = _GetDescriptorSet(descriptors, setSid, setIndex);
                    if (!SetSampledImageDescriptor(descriptorSet, imageView, binding))
                    {
                        return false;
                    }
                }

                return true;
            }
            else
            {
                const auto descriptorSet = _GetDescriptorSet(_descriptors, setSid, setIndex);
                return SetSampledImageDescriptor(descriptorSet, imageView, binding);
            }
        }}
        //--------------------------------------------------------------------------

        bool VulkanDescriptorSetManager::SetSampledImageDescriptor(StringID setSid, UInt32 setIndex, bool perFrame, UInt32 frameIndex, VkImageView imageView, UInt32 binding) const KMP_PROFILING(ProfileLevelImportantVerbose)
        {
            auto descriptorSet = GetDescriptorSet(setSid, setIndex, perFrame, frameIndex);
            return SetSampledImageDescriptor(descriptorSet, imageView, binding);
        }}
        //--------------------------------------------------------------------------

        bool VulkanDescriptorSetManager::SetSampledImageDescriptor(VkDescriptorSet descriptorSet, VkImageView imageView, UInt32 binding) const KMP_PROFILING(ProfileLevelImportant)
        {
            if (descriptorSet == VK_NULL_HANDLE)
            {
                KMP_LOG_ERROR("failed to set sampled image descriptor - set is null");
                return false;
            }

            VkDescriptorImageInfo descriptorInfo{};
            descriptorInfo.imageView = imageView;
            descriptorInfo.imageLayout = VK_ImageLayout_ShaderReadOnlyOptimal;

            _UpdateDescriptorSet(descriptorSet, descriptorInfo, VK_DescriptorType_SampledImage, binding);

            return true;
        }}
        //--------------------------------------------------------------------------

        bool VulkanDescriptorSetManager::SetSamplerDescriptor(StringID setSid, UInt32 setIndex, bool perFrame, VkSampler sampler, UInt32 binding) const KMP_PROFILING(ProfileLevelImportant)
        {
            if (perFrame)
            {
                for (auto& descriptors : _descriptorsPerFrame)
                {
                    const auto descriptorSet = _GetDescriptorSet(descriptors, setSid, setIndex);
                    if (!SetSamplerDescriptor(descriptorSet, sampler, binding))
                    {
                        return false;
                    }
                }

                return true;
            }
            else
            {
                const auto descriptorSet = _GetDescriptorSet(_descriptors, setSid, setIndex);
                return SetSamplerDescriptor(descriptorSet, sampler, binding);
            }
        }}
        //--------------------------------------------------------------------------

        bool VulkanDescriptorSetManager::SetSamplerDescriptor(StringID setSid, UInt32 setIndex, bool perFrame, UInt32 frameIndex, VkSampler sampler, UInt32 binding) const KMP_PROFILING(ProfileLevelImportantVerbose)
        {
            auto descriptorSet = GetDescriptorSet(setSid, setIndex, perFrame, frameIndex);
            return SetSamplerDescriptor(descriptorSet, sampler, binding);
        }}
        //--------------------------------------------------------------------------

        bool VulkanDescriptorSetManager::SetSamplerDescriptor(VkDescriptorSet descriptorSet, VkSampler sampler, UInt32 binding) const KMP_PROFILING(ProfileLevelImportant)
        {
            if (descriptorSet == VK_NULL_HANDLE)
            {
                KMP_LOG_ERROR("failed to set sampler descriptor - set is null");
                return false;
            }

            VkDescriptorImageInfo descriptorInfo{};
            descriptorInfo.sampler = sampler;

            _UpdateDescriptorSet(descriptorSet, descriptorInfo, VK_DescriptorType_Sampler, binding);

            return true;
        }}
        //--------------------------------------------------------------------------

        void VulkanDescriptorSetManager::_Initialize() KMP_PROFILING(ProfileLevelImportant)
        {
            //TODO: fix numbers
            VkDescriptorPoolSize poolSizes[] = {
                { VK_DescriptorType_Sampler, 100 },
                { VK_DescriptorType_CombinedImageSampler, 100 },
                { VK_DescriptorType_SampledImage, 100 },
                { VK_DescriptorType_StorageImage, 100 },
                { VK_DescriptorType_UniformTexelBuffer, 100 },
                { VK_DescriptorType_StorageTexelBuffer, 100 },
                { VK_DescriptorType_UniformBuffer, 100 },
                { VK_DescriptorType_StorageBuffer, 100 },
                { VK_DescriptorType_UniformBufferDynamic, 100 },
                { VK_DescriptorType_StorageBufferDynamic, 100 },
                { VK_DescriptorType_InputAttachment, 100 } };

            auto poolInfo = VKUtils::InitVkDescriptorPoolCreateInfo();
            poolInfo.maxSets = 100;
            poolInfo.poolSizeCount = UInt32(std::size(poolSizes));
            poolInfo.pPoolSizes = poolSizes;

            const auto result = vkCreateDescriptorPool(_device, &poolInfo, nullptr, &_descriptorPool);
            VKUtils::CheckResult(result, "VulkanDescriptorSetManager: failed to create descriptor pool");
        }}
        //--------------------------------------------------------------------------

        void VulkanDescriptorSetManager::_Finalize() KMP_PROFILING(ProfileLevelImportant)
        {
            for (const auto& [sid, descriptorSetLayout] : _descriptorSetLayouts)
            {
                vkDestroyDescriptorSetLayout(_device, descriptorSetLayout, nullptr);
            }
            _descriptorSetLayouts.clear();

            vkDestroyDescriptorPool(_device, _descriptorPool, nullptr);
        }}
        //--------------------------------------------------------------------------

        bool VulkanDescriptorSetManager::_AllocateDescriptorSets(const Vector<VkDescriptorSetLayout>& layouts, StringID setSid, UInt32 setsCount, DescriptorSetStorage& storage) const KMP_PROFILING(ProfileLevelImportant)
        {
            auto descriptorSetAllocateInfo = VKUtils::InitVkDescriptorSetAllocateInfo();
            descriptorSetAllocateInfo.descriptorPool = _descriptorPool;
            descriptorSetAllocateInfo.descriptorSetCount = setsCount;
            descriptorSetAllocateInfo.pSetLayouts = layouts.data();

            Vector<VkDescriptorSet> newDesriptorSets(setsCount, VK_NULL_HANDLE);
            const auto result = vkAllocateDescriptorSets(_device, &descriptorSetAllocateInfo, newDesriptorSets.data());
            if (result != VK_SUCCESS)
            {
                VKUtils::CheckResult(result, "VulkanDescriptorSetManager: failed to allocate descriptor sets", "throw exception"_false);
                return false;
            }

            if (!storage.contains(setSid))
            {
                storage.emplace(setSid, newDesriptorSets);
            }
            else
            {
                KMP_LOG_WARN("'{}' new descriptors with sid '{}' will be appended to already existing descriptor set vector with the same sid", setsCount, setSid);
                Utils::MergeVectors(newDesriptorSets, storage[setSid]);
            }

            return true;
        }}
        //--------------------------------------------------------------------------

        VkDescriptorSet VulkanDescriptorSetManager::_GetDescriptorSet(const DescriptorSetStorage& storage, StringID setSid, UInt32 setIndex) const noexcept KMP_PROFILING(ProfileLevelImportantVerbose)
        {
            if (!storage.contains(setSid))
            {
                KMP_LOG_ERROR("cannot find descriptor set with sid '{}' - sid not found", setSid);
                return VK_NULL_HANDLE;
            }
            const auto& descriptorSets = storage.at(setSid);

            if (setIndex >= UInt32(descriptorSets.size()))
            {
                KMP_LOG_ERROR("cannot find descriptor set with sid '{}' and index '{}' - out of range", setSid, setIndex);
                return VK_NULL_HANDLE;
            }

            return descriptorSets.at(setIndex);
        }}
        //--------------------------------------------------------------------------

        VkWriteDescriptorSet VulkanDescriptorSetManager::_GetWriteDescriptorSetTemplate(VkDescriptorSet descriptorSet, VkDescriptorType type, UInt32 binding) const noexcept
        {
            auto writeDescriptorSet = VKUtils::InitVkWriteDescriptorSet();
            writeDescriptorSet.dstSet = descriptorSet;
            writeDescriptorSet.dstBinding = binding;
            writeDescriptorSet.descriptorCount = 1;
            writeDescriptorSet.descriptorType = type;

            return writeDescriptorSet;
        }
        //--------------------------------------------------------------------------

        void VulkanDescriptorSetManager::_UpdateDescriptorSet(VkDescriptorSet descriptorSet, const VkDescriptorBufferInfo& bufferInfo, VkDescriptorType type, UInt32 binding) const KMP_PROFILING(ProfileLevelImportantVerbose)
        {
            auto writeDescriptorSet = _GetWriteDescriptorSetTemplate(descriptorSet, type, binding);
            writeDescriptorSet.pBufferInfo = &bufferInfo;
            vkUpdateDescriptorSets(_device, 1, &writeDescriptorSet, 0, nullptr);
        }}
        //--------------------------------------------------------------------------

        void VulkanDescriptorSetManager::_UpdateDescriptorSet(VkDescriptorSet descriptorSet, const VkDescriptorImageInfo& imageInfo, VkDescriptorType type, UInt32 binding) const KMP_PROFILING(ProfileLevelImportantVerbose)
        {
            auto writeDescriptorSet = _GetWriteDescriptorSetTemplate(descriptorSet, type, binding);
            writeDescriptorSet.pImageInfo = &imageInfo;
            vkUpdateDescriptorSets(_device, 1, &writeDescriptorSet, 0, nullptr);
        }}
        //--------------------------------------------------------------------------
    }
}