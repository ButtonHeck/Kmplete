#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Base/string_id.h"
#include "Kmplete/Graphics/Vulkan/Pipeline/vulkan_graphics_pipeline_parameters.h"
#include "Kmplete/Log/log_class_macro.h"
#include "Kmplete/Profile/profiler_fwd.h"

#include <vulkan/vulkan.h>


namespace Kmplete
{
    namespace Graphics
    {
        class VulkanVertexBuffer;


        //TODO: comments
        class VulkanGraphicsPipeline
        {
            KMP_DISABLE_COPY_MOVE(VulkanGraphicsPipeline)
            KMP_LOG_CLASSNAME(VulkanGraphicsPipeline)
            KMP_PROFILE_CONSTRUCTOR_DECLARE()

        public:
            KMP_API VulkanGraphicsPipeline(VkDevice device, StringID sid, VkPipelineLayout layout, const VulkanGraphicsPipelineParameters& parameters);
            KMP_API ~VulkanGraphicsPipeline();

            KMP_NODISCARD KMP_API VkPipeline GetVkPipeline() const noexcept;
            KMP_NODISCARD KMP_API UInt32 GetColorAttachmentsCount() const noexcept;

        private:
            void _Initialize(VkPipelineLayout layout);
            void _Finalize();

        private:
            VkDevice _device;
            const StringID _sid;
            
            VkPipeline _pipeline;
            VulkanGraphicsPipelineParameters _parameters;
        };
        //--------------------------------------------------------------------------
    }
}