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
            , _descriptorSetLayouts()
        {}
        //--------------------------------------------------------------------------

        VulkanDescriptorSetManager::~VulkanDescriptorSetManager() KMP_PROFILING(ProfileLevelAlways)
        {
            for (const auto& [sid, descriptorSetLayout] : _descriptorSetLayouts)
            {
                vkDestroyDescriptorSetLayout(_device, descriptorSetLayout, nullptr);
            }
            _descriptorSetLayouts.clear();
        }}
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
    }
}