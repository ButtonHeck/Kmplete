#include "Kmplete/Graphics/Vulkan/vulkan_pipeline_manager.h"
#include "Kmplete/Core/assertion.h"
#include "Kmplete/Log/log.h"
#include "Kmplete/Profile/profiler.h"


namespace Kmplete
{
    namespace Graphics
    {
        VulkanPipelineManager::VulkanPipelineManager(VkDevice device)
            : _device(device)
            , _pipelines()
        {
            KMP_ASSERT(_device);
        }
        //--------------------------------------------------------------------------

        VulkanPipelineManager::~VulkanPipelineManager() KMP_PROFILING(ProfileLevelAlways)
        {
            KMP_ASSERT(_device);

            _pipelines.clear();
        }}
        //--------------------------------------------------------------------------

        VulkanGraphicsPipeline& VulkanPipelineManager::AddGraphicsPipeline(StringID sid, const VulkanGraphicsPipelineParameters& parameters) KMP_PROFILING(ProfileLevelImportant)
        {
            KMP_ASSERT(_device);

            if (_pipelines.contains(sid))
            {
                KMP_LOG_WARN("pipeline with sid '{}' has already been created", sid);
                return *_pipelines[sid].get();
            }

            const auto [iterator, hasEmplaced] = _pipelines.emplace(sid, CreateUPtr<VulkanGraphicsPipeline>(_device, sid, parameters));
            return *iterator->second.get();
        }}
        //--------------------------------------------------------------------------

        OptionalRef<VulkanGraphicsPipeline> VulkanPipelineManager::GetGraphicsPipeline(StringID sid) const
        {
            if (_pipelines.contains(sid))
            {
                return *_pipelines.at(sid).get();
            }
            
            KMP_LOG_ERROR("graphics pipeline with sid '{}' not found", sid);
            return std::nullopt;
        }
        //--------------------------------------------------------------------------
    }
}