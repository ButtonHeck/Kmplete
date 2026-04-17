#include "Kmplete/Graphics/Vulkan/vulkan_descriptor_set_manager.h"
#include "Kmplete/Graphics/Vulkan/vulkan_buffer.h"
#include "Kmplete/Graphics/Vulkan/Utils/initializers.h"
#include "Kmplete/Graphics/Vulkan/Utils/result_description.h"
#include "Kmplete/Base/named_bool.h"
#include "Kmplete/Utils/vector_utils.h"
#include "Kmplete/Profile/profiler.h"
#include "Kmplete/Log/log.h"


namespace Kmplete
{
    namespace Graphics
    {
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
                auto descriptorSetLayoutCreateInfo = Graphics::VulkanUtils::InitVkDescriptorSetLayoutCreateInfo();
                descriptorSetLayoutCreateInfo.bindingCount = UInt32(bindings.size());
                descriptorSetLayoutCreateInfo.pBindings = bindings.empty() ? nullptr : bindings.data();
                VkDescriptorSetLayout layout = nullptr;
                const auto result = vkCreateDescriptorSetLayout(_device, &descriptorSetLayoutCreateInfo, nullptr, &layout);
                VulkanUtils::CheckResult(result, "VulkanDescriptorSetManager: failed to create descriptor set layout");

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
                for (auto& descriptors : _descriptorsPerFrame)
                {
                    if (!_AllocateDescriptorSets(layouts, setSid, setsCount, descriptors))
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
                if (frameIndex >= NumConcurrentFrames)
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

        bool VulkanDescriptorSetManager::SetUniformBufferDescriptor(StringID setSid, UInt32 setIndex, bool perFrame, UInt32 frameIndex, VkBuffer buffer, VkDeviceSize size, UInt32 binding) const KMP_PROFILING(ProfileLevelImportant)
        {
            auto descriptor = GetDescriptorSet(setSid, setIndex, perFrame, frameIndex);
            if (descriptor == VK_NULL_HANDLE)
            {
                KMP_LOG_ERROR("failed to set combined image sampler descriptor for set with sid '{}' and index '{}'", setSid, setIndex);
                return false;
            }

            VkDescriptorBufferInfo bufferInfo{};
            bufferInfo.buffer = buffer;
            bufferInfo.range = size;

            _UpdateDescriptorSet(descriptor, bufferInfo, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, binding);

            return true;
        }}
        //--------------------------------------------------------------------------

        bool VulkanDescriptorSetManager::SetCombinedImageSamplerDescriptor(StringID setSid, UInt32 setIndex, bool perFrame, UInt32 frameIndex, VkImageView imageView, VkSampler sampler, UInt32 binding) const KMP_PROFILING(ProfileLevelImportant)
        {
            auto descriptor = GetDescriptorSet(setSid, setIndex, perFrame, frameIndex);
            if (descriptor == VK_NULL_HANDLE)
            {
                KMP_LOG_ERROR("failed to set combined image sampler descriptor for set with sid '{}' and index '{}'", setSid, setIndex);
                return false;
            }

            VkDescriptorImageInfo descriptorInfo{};
            descriptorInfo.imageView = imageView;
            descriptorInfo.sampler = sampler;
            descriptorInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

            _UpdateDescriptorSet(descriptor, descriptorInfo, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, binding);

            return true;
        }}
        //--------------------------------------------------------------------------

        bool VulkanDescriptorSetManager::SetSampledImageDescriptor(StringID setSid, UInt32 setIndex, bool perFrame, UInt32 frameIndex, VkImageView imageView, UInt32 binding) const KMP_PROFILING(ProfileLevelImportant)
        {
            auto descriptor = GetDescriptorSet(setSid, setIndex, perFrame, frameIndex);
            if (descriptor == VK_NULL_HANDLE)
            {
                KMP_LOG_ERROR("failed to set combined image sampler descriptor for set with sid '{}' and index '{}'", setSid, setIndex);
                return false;
            }

            VkDescriptorImageInfo descriptorInfo{};
            descriptorInfo.imageView = imageView;
            descriptorInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

            _UpdateDescriptorSet(descriptor, descriptorInfo, VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, binding);

            return true;
        }}
        //--------------------------------------------------------------------------

        bool VulkanDescriptorSetManager::SetSamplerDescriptor(StringID setSid, UInt32 setIndex, bool perFrame, UInt32 frameIndex, VkSampler sampler, UInt32 binding) const KMP_PROFILING(ProfileLevelImportant)
        {
            auto descriptor = GetDescriptorSet(setSid, setIndex, perFrame, frameIndex);
            if (descriptor == VK_NULL_HANDLE)
            {
                KMP_LOG_ERROR("failed to set combined image sampler descriptor for set with sid '{}' and index '{}'", setSid, setIndex);
                return false;
            }

            VkDescriptorImageInfo descriptorInfo{};
            descriptorInfo.sampler = sampler;

            _UpdateDescriptorSet(descriptor, descriptorInfo, VK_DESCRIPTOR_TYPE_SAMPLER, binding);

            return true;
        }}
        //--------------------------------------------------------------------------

        void VulkanDescriptorSetManager::_Initialize() KMP_PROFILING(ProfileLevelImportant)
        {
            //TODO: fix numbers
            VkDescriptorPoolSize poolSizes[] = {
                { VK_DESCRIPTOR_TYPE_SAMPLER, 100 },
                { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 100 },
                { VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 100 },
                { VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 100 },
                { VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 100 },
                { VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 100 },
                { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 100 },
                { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 100 },
                { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 100 },
                { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 100 },
                { VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 100 } };

            auto poolInfo = VulkanUtils::InitVkDescriptorPoolCreateInfo();
            poolInfo.maxSets = 100;
            poolInfo.poolSizeCount = UInt32(std::size(poolSizes));
            poolInfo.pPoolSizes = poolSizes;

            const auto result = vkCreateDescriptorPool(_device, &poolInfo, nullptr, &_descriptorPool);
            VulkanUtils::CheckResult(result, "VulkanDescriptorSetManager: failed to create descriptor pool");
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

        bool VulkanDescriptorSetManager::_AllocateDescriptorSets(const Vector<VkDescriptorSetLayout>& layouts, StringID setSid, UInt32 setsCount, StringIDHashMap<Vector<VkDescriptorSet>>& storage) const KMP_PROFILING(ProfileLevelImportant)
        {
            auto descriptorSetAllocateInfo = VulkanUtils::InitVkDescriptorSetAllocateInfo();
            descriptorSetAllocateInfo.descriptorPool = _descriptorPool;
            descriptorSetAllocateInfo.descriptorSetCount = setsCount;
            descriptorSetAllocateInfo.pSetLayouts = layouts.data();

            Vector<VkDescriptorSet> newDesriptorSets(setsCount, VK_NULL_HANDLE);
            const auto result = vkAllocateDescriptorSets(_device, &descriptorSetAllocateInfo, newDesriptorSets.data());
            if (result != VK_SUCCESS)
            {
                VulkanUtils::CheckResult(result, "VulkanDescriptorSetManager: failed to allocate descriptor sets", "throw exception"_false);
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

        VkDescriptorSet VulkanDescriptorSetManager::_GetDescriptorSet(const StringIDHashMap<Vector<VkDescriptorSet>>& storage, StringID setSid, UInt32 setIndex) const KMP_PROFILING(ProfileLevelImportantVerbose)
        {
            if (!storage.contains(setSid))
            {
                KMP_LOG_ERROR("cannot find descriptor set with sid '{}' - sid not found", setSid);
                return VK_NULL_HANDLE;
            }
            const auto& descriptors = storage.at(setSid);

            if (setIndex >= UInt32(descriptors.size()))
            {
                KMP_LOG_ERROR("cannot find descriptor set with sid '{}' and index '{}' - out of range", setSid, setIndex);
                return VK_NULL_HANDLE;
            }

            return descriptors.at(setIndex);
        }}
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

        VkWriteDescriptorSet VulkanDescriptorSetManager::_GetWriteDescriptorSetTemplate(VkDescriptorSet descriptorSet, VkDescriptorType type, UInt32 binding) const noexcept
        {
            auto writeDescriptorSet = VulkanUtils::InitVkWriteDescriptorSet();
            writeDescriptorSet.dstSet = descriptorSet;
            writeDescriptorSet.dstBinding = binding;
            writeDescriptorSet.descriptorCount = 1;
            writeDescriptorSet.descriptorType = type;

            return writeDescriptorSet;
        }
        //--------------------------------------------------------------------------
    }
}