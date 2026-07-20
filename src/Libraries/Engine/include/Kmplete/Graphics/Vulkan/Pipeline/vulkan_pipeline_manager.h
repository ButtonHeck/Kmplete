#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Base/pointers.h"
#include "Kmplete/Base/string_id.h"
#include "Kmplete/Base/optional.h"
#include "Kmplete/Graphics/Vulkan/Pipeline/vulkan_graphics_pipeline.h"
#include "Kmplete/Graphics/Vulkan/Pipeline/vulkan_graphics_pipeline_parameters.h"
#include "Kmplete/Graphics/Vulkan/Pipeline/vulkan_pipeline_cache.h"
#include "Kmplete/Graphics/Vulkan/Core/vulkan_context.h"
#include "Kmplete/Log/log_class_macro.h"
#include "Kmplete/Profile/profiler_fwd.h"

#include <vulkan/vulkan.h>


namespace Kmplete
{
    namespace Graphics
    {
        class VulkanDescriptorSetManager;


        //! Manager of Vulkan pipeline objects, pipeline caches and pipeline layouts.
        //! @see VulkanGraphicsPipeline
        //! @see VulkanPipelineCache
        class KMP_API VulkanPipelineManager
        {
            KMP_DISABLE_COPY_MOVE(VulkanPipelineManager)
            KMP_LOG_CLASSNAME(VulkanPipelineManager)
            KMP_PROFILE_CONSTRUCTOR_DECLARE()

        public:
            VulkanPipelineManager(VkDevice device, const VulkanContext& context, const VulkanDescriptorSetManager& descriptorSetManager);
            ~VulkanPipelineManager();

            bool AddPipelineLayoutWithSetsSids(StringID layoutSid, const Vector<StringID>& descriptorSetLayoutsSids, const Vector<VkPushConstantRange>& pushConstantRanges = {});
            bool AddPipelineLayout(StringID layoutSid, const Vector<VkDescriptorSetLayout>& descriptorSetLayouts, const Vector<VkPushConstantRange>& pushConstantRanges = {});
            KMP_NODISCARD VkPipelineLayout GetPipelineLayout(StringID layoutSid) const noexcept;

            bool AddPipelineCache(StringID pipelineSid, const Filepath& binaryPath);

            bool AddGraphicsPipeline(StringID pipelineSid, StringID layoutSid, const VulkanGraphicsPipelineParameters& parameters, const Filepath& cacheBinaryPath);
            bool AddGraphicsPipeline(StringID pipelineSid, VkPipelineLayout layout, const VulkanGraphicsPipelineParameters& parameters, const Filepath& cacheBinaryPath);
            bool AddGraphicsPipeline(StringID pipelineSid, StringID layoutSid, const VulkanGraphicsPipelineParameters& parameters);
            bool AddGraphicsPipeline(StringID pipelineSid, VkPipelineLayout layout, const VulkanGraphicsPipelineParameters& parameters);
            KMP_NODISCARD OptionalRef<VulkanGraphicsPipeline> GetGraphicsPipeline(StringID pipelineSid) const;

        private:
            VkDevice _device;
            const VulkanContext& _context;
            const VulkanDescriptorSetManager& _descriptorSetManager;
            StringIDHashMap<VkPipelineLayout> _layouts;
            StringIDHashMap<UPtr<VulkanGraphicsPipeline>> _pipelines;
            StringIDHashMap<UPtr<VulkanPipelineCache>> _pipelineCaches;
        };
        //--------------------------------------------------------------------------
    }
}