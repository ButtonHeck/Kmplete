#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Base/string_id.h"
#include "Kmplete/Log/log_class_macro.h"

#include <vulkan/vulkan.h>


namespace Kmplete
{
    namespace Graphics
    {
        //TODO: comments
        class VulkanGraphicsPipeline
        {
            KMP_DISABLE_COPY_MOVE(VulkanGraphicsPipeline)
            KMP_LOG_CLASSNAME(VulkanGraphicsPipeline)

        public:
            KMP_API VulkanGraphicsPipeline(VkDevice device, StringID sid);
            KMP_API ~VulkanGraphicsPipeline();

            KMP_API bool Build();

            KMP_API VulkanGraphicsPipeline& AddDescriptorSetLayout(VkDescriptorSetLayout descriptorSetLayout);
            KMP_API VulkanGraphicsPipeline& SetupInputAssembly(VkPrimitiveTopology topology, bool primitiveRestartEnable);
            KMP_API VulkanGraphicsPipeline& SetupPolygonMode(VkPolygonMode polygonMode);
            KMP_API VulkanGraphicsPipeline& SetupCulling(VkCullModeFlags cullMode, VkFrontFace frontFace);
            KMP_API VulkanGraphicsPipeline& SetupDepthClamping(bool enabled);
            KMP_API VulkanGraphicsPipeline& SetupDepthBiasParameters(bool biasEnabled, float constantFactor, float clamp, float slopeFactor);
            KMP_API VulkanGraphicsPipeline& SetupRasterizerDiscard(bool enabled);
            KMP_API VulkanGraphicsPipeline& SetupLineWidth(float lineWidth);
            KMP_API VulkanGraphicsPipeline& SetupColorBlendConstants(Array<float, 4> constants);
            KMP_API VulkanGraphicsPipeline& AddColorBlendAttachment(VkPipelineColorBlendAttachmentState colorBlendAttachment);
            KMP_API VulkanGraphicsPipeline& AddDynamicState(VkDynamicState dynamicState);
            KMP_API VulkanGraphicsPipeline& SetupDepthTest(bool enabled);
            KMP_API VulkanGraphicsPipeline& SetupDepthWrite(bool enabled);
            KMP_API VulkanGraphicsPipeline& SetupDepthComparison(VkCompareOp compareOp);
            KMP_API VulkanGraphicsPipeline& SetupDepthBoundsTest(bool enabled, float min, float max);
            KMP_API VulkanGraphicsPipeline& SetupStencilTest(bool enabled);
            KMP_API VulkanGraphicsPipeline& SetupStencilStates(VkStencilOpState frontFaceState, VkStencilOpState backFaceState);
            KMP_API VulkanGraphicsPipeline& SetupMultisamplingSamples(VkSampleCountFlagBits samples);
            KMP_API VulkanGraphicsPipeline& SetupMultisamplingSampleShading(bool enabled, float minSampleShading);
            KMP_API VulkanGraphicsPipeline& AddVertexInputBindings(Vector<VkVertexInputBindingDescription>&& inputBindingDescriptions);
            KMP_API VulkanGraphicsPipeline& AddVertexAttributesDescriptions(Vector<VkVertexInputAttributeDescription>&& attributesDescriptions);
            KMP_API VulkanGraphicsPipeline& AddShaderStages(Vector<VkPipelineShaderStageCreateInfo>&& shaderStages);
            KMP_API VulkanGraphicsPipeline& AddRenderingColorAttachment(VkFormat attachmentFormat);
            KMP_API VulkanGraphicsPipeline& SetupRenderingDepthStencilFormats(VkFormat depthFormat, VkFormat stencilFormat);

            KMP_NODISCARD KMP_API VkPipeline GetVkPipeline() const noexcept;

        private:
            void _SetupDefault() noexcept;

        private:
            VkDevice _device;
            const StringID _sid;
            
            Vector<VkDescriptorSetLayout> _descriptorSetLayouts;
            VkPipelineLayout _pipelineLayout;
            VkPipeline _pipeline;

            VkPipelineLayoutCreateInfo _layoutCreateInfo;
            VkPipelineInputAssemblyStateCreateInfo _inputAssemblyCreateInfo;
            VkPipelineRasterizationStateCreateInfo _rasterizationStateCreateInfo;
            VkPipelineColorBlendStateCreateInfo _colorBlendStateCreateInfo;
            Vector<VkPipelineColorBlendAttachmentState> _colorBlendAttachments;
            VkPipelineViewportStateCreateInfo _viewportStateCreateInfo;
            VkPipelineDynamicStateCreateInfo _dynamicStateCreateInfo;
            Vector<VkDynamicState> _dynamicStates;
            VkPipelineDepthStencilStateCreateInfo _depthStencilStateCreateInfo;
            VkPipelineMultisampleStateCreateInfo _multisamplingStateCreateInfo;
            VkPipelineVertexInputStateCreateInfo _vertexInputStateCreateInfo;
            Vector<VkVertexInputBindingDescription> _vertexInputBindings;
            Vector<VkVertexInputAttributeDescription> _vertexAttributesDescriptions;
            Vector<VkPipelineShaderStageCreateInfo> _shadersStages;
            VkPipelineRenderingCreateInfoKHR _renderingCreateInfo;
            Vector<VkFormat> _renderingColorAttachmentsFormats;
        };
        //--------------------------------------------------------------------------
    }
}