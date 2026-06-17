#include "Kmplete/Graphics/Vulkan/Pipeline/vulkan_pipeline_manager.h"
#include "Kmplete/Graphics/Vulkan/Utils/initializers.h"
#include "Kmplete/Graphics/Vulkan/Utils/result_description.h"
#include "Kmplete/Core/assertion.h"
#include "Kmplete/Log/log.h"
#include "Kmplete/Profile/profiler.h"


namespace Kmplete
{
    namespace Graphics
    {
        VulkanPipelineManager::VulkanPipelineManager(VkDevice device, const VulkanContext& context)
            : KMP_PROFILE_CONSTRUCTOR_START_BASE_CLASS()
              _device(device)
            , _context(context)
            , _pipelines()
        {
            KMP_ASSERT(_device);
            KMP_PROFILE_CONSTRUCTOR_END()
        }
        //--------------------------------------------------------------------------

        VulkanPipelineManager::~VulkanPipelineManager() KMP_PROFILING(ProfileLevelAlways)
        {
            KMP_ASSERT(_device);

            _pipelines.clear();

            for (const auto& [sid, layout] : _layouts)
            {
                vkDestroyPipelineLayout(_device, layout, nullptr);
            }
            _layouts.clear();
        }}
        //--------------------------------------------------------------------------

        bool VulkanPipelineManager::AddPipelineLayout(StringID layoutSid, const Vector<VkDescriptorSetLayout>& descriptorSetLayouts, const Vector<VkPushConstantRange>& pushConstantRanges) KMP_PROFILING(ProfileLevelImportant)
        {
            KMP_ASSERT(_device);

            if (_layouts.contains(layoutSid))
            {
                KMP_LOG_WARN("pipeline layout with sid '{}' has already been added", layoutSid);
                return true;
            }

            VkPipelineLayoutCreateInfo layoutCreateInfo = VKUtils::InitVkPipelineLayoutCreateInfo();
            if (!descriptorSetLayouts.empty())
            {
                layoutCreateInfo.setLayoutCount = UInt32(descriptorSetLayouts.size());
                layoutCreateInfo.pSetLayouts = descriptorSetLayouts.data();
            }

            if (!pushConstantRanges.empty())
            {
                layoutCreateInfo.pushConstantRangeCount = UInt32(pushConstantRanges.size());
                layoutCreateInfo.pPushConstantRanges = pushConstantRanges.data();
            }

            VkPipelineLayout layout;
            auto result = vkCreatePipelineLayout(_device, &layoutCreateInfo, nullptr, &layout);
            VKUtils::CheckResult(result, "VulkanPipelineManager: failed to build graphics pipeline layout");

            const auto [iterator, hasEmplaced] = _layouts.emplace(layoutSid, layout);
            return hasEmplaced;
        }}
        //--------------------------------------------------------------------------

        VkPipelineLayout VulkanPipelineManager::GetPipelineLayout(StringID layoutSid) const noexcept
        {
            if (_layouts.contains(layoutSid))
            {
                return _layouts.at(layoutSid);
            }

            KMP_LOG_ERROR("pipeline layout with sid '{}' not found", layoutSid);
            return VK_NULL_HANDLE;
        }
        //--------------------------------------------------------------------------

        bool VulkanPipelineManager::AddGraphicsPipeline(StringID pipelineSid, StringID layoutSid, const VulkanGraphicsPipelineParameters& parameters)
        {
            return AddGraphicsPipeline(pipelineSid, GetPipelineLayout(layoutSid), parameters);
        }
        //--------------------------------------------------------------------------

        bool VulkanPipelineManager::AddGraphicsPipeline(StringID pipelineSid, VkPipelineLayout layout, const VulkanGraphicsPipelineParameters& parameters) KMP_PROFILING(ProfileLevelImportant)
        {
            KMP_ASSERT(_device);

            if (_pipelines.contains(pipelineSid))
            {
                KMP_LOG_WARN("pipeline with sid '{}' has already been created", pipelineSid);
                return true;
            }

            if (layout == VK_NULL_HANDLE)
            {
                KMP_LOG_ERROR("cannot create pipeline with sid '{}' - pipeline layout is null", pipelineSid);
                return false;
            }

            const auto [iterator, hasEmplaced] = _pipelines.emplace(pipelineSid, CreateUPtr<VulkanGraphicsPipeline>(_device, pipelineSid, layout, parameters));
            return hasEmplaced;
        }}
        //--------------------------------------------------------------------------

        OptionalRef<VulkanGraphicsPipeline> VulkanPipelineManager::GetGraphicsPipeline(StringID pipelineSid) const
        {
            if (_pipelines.contains(pipelineSid))
            {
                return *_pipelines.at(pipelineSid).get();
            }
            
            KMP_LOG_ERROR("graphics pipeline with sid '{}' not found", pipelineSid);
            return std::nullopt;
        }
        //--------------------------------------------------------------------------
    }
}