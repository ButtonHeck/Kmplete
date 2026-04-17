#include "Kmplete/Graphics/Vulkan/vulkan_descriptor_set_manager.h"
#include "Kmplete/Graphics/Vulkan/Utils/initializers.h"
#include "Kmplete/Graphics/Vulkan/Utils/result_description.h"
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

        bool VulkanDescriptorSetManager::AddDescriptorSetLayout(StringID sid, const Vector<VkDescriptorSetLayoutBinding>& bindings) KMP_PROFILING(ProfileLevelImportant)
        {
            if (_descriptorSetLayouts.contains(sid))
            {
                KMP_LOG_WARN("descriptor set layout with sid '{}' has already been created", sid);
                return true;
            }

            try
            {
                auto descriptorSetLayoutCreateInfo = Graphics::VulkanUtils::InitVkDescriptorSetLayoutCreateInfo();
                descriptorSetLayoutCreateInfo.bindingCount = UInt32(bindings.size());
                descriptorSetLayoutCreateInfo.pBindings = bindings.empty() ? nullptr : bindings.data();
                VkDescriptorSetLayout layout = nullptr;
                const auto result = vkCreateDescriptorSetLayout(_device, &descriptorSetLayoutCreateInfo, nullptr, &layout);
                VulkanUtils::CheckResult(result, "VulkanDescriptorSetManager: failed to create descriptor set layout");

                const auto [iterator, hasEmplaced] = _descriptorSetLayouts.emplace(sid, layout);
                return iterator->second != nullptr;
            }
            catch (KMP_MB_UNUSED const std::runtime_error& er)
            {
                KMP_LOG_ERROR("failed to create descriptor set layout '{}'", sid);
                return false;
            }
        }}
        //--------------------------------------------------------------------------

        VkDescriptorSetLayout VulkanDescriptorSetManager::GetDescriptorSetLayout(StringID sid) const noexcept
        {
            if (_descriptorSetLayouts.contains(sid))
            {
                return _descriptorSetLayouts.at(sid);
            }

            KMP_LOG_ERROR("descriptor set layout with sid '{}' not found", sid);
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
    }
}