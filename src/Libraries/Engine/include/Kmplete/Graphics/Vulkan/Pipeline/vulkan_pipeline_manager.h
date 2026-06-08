#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Base/pointers.h"
#include "Kmplete/Base/string_id.h"
#include "Kmplete/Base/optional.h"
#include "Kmplete/Graphics/Vulkan/Pipeline/vulkan_graphics_pipeline.h"
#include "Kmplete/Graphics/Vulkan/Pipeline/vulkan_graphics_pipeline_parameters.h"
#include "Kmplete/Log/log_class_macro.h"

#include <vulkan/vulkan.h>


namespace Kmplete
{
    namespace Graphics
    {
        //TODO: comments
        class VulkanPipelineManager
        {
            KMP_DISABLE_COPY_MOVE(VulkanPipelineManager)
            KMP_LOG_CLASSNAME(VulkanPipelineManager)

        public:
            KMP_API explicit VulkanPipelineManager(VkDevice device);
            KMP_API ~VulkanPipelineManager();

            KMP_API bool AddPipelineLayout(StringID layoutSid, const Vector<VkDescriptorSetLayout>& descriptorSetLayouts, const Vector<VkPushConstantRange>& pushConstantRanges);
            KMP_NODISCARD KMP_API VkPipelineLayout GetPipelineLayout(StringID layoutSid) const noexcept;

            KMP_API bool AddGraphicsPipeline(StringID pipelineSid, StringID layoutSid, const VulkanGraphicsPipelineParameters& parameters);
            KMP_API bool AddGraphicsPipeline(StringID pipelineSid, VkPipelineLayout layout, const VulkanGraphicsPipelineParameters& parameters);
            KMP_NODISCARD KMP_API OptionalRef<VulkanGraphicsPipeline> GetGraphicsPipeline(StringID pipelineSid) const;

        private:
            VkDevice _device;
            StringIDHashMap<VkPipelineLayout> _layouts;
            StringIDHashMap<UPtr<VulkanGraphicsPipeline>> _pipelines;
        };
        //--------------------------------------------------------------------------
    }
}