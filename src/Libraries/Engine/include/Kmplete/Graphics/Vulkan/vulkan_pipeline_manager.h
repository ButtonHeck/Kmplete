#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Base/pointers.h"
#include "Kmplete/Base/string_id.h"
#include "Kmplete/Base/optional.h"
#include "Kmplete/Graphics/Vulkan/vulkan_graphics_pipeline.h"
#include "Kmplete/Graphics/Vulkan/vulkan_graphics_pipeline_parameters.h"
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

            KMP_API VulkanGraphicsPipeline& AddGraphicsPipeline(StringID sid, const VulkanGraphicsPipelineParameters& parameters);
            KMP_NODISCARD KMP_API OptionalRef<VulkanGraphicsPipeline> GetGraphicsPipeline(StringID sid) const;

        private:
            VkDevice _device;
            StringIDHashMap<UPtr<VulkanGraphicsPipeline>> _pipelines;
        };
        //--------------------------------------------------------------------------
    }
}