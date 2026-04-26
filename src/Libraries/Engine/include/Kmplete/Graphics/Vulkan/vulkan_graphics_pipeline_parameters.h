#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Log/log_class_macro.h"

#include <vulkan/vulkan.h>


namespace Kmplete
{
    namespace Graphics
    {
        class VulkanVertexBuffer;


        //TODO: comments
        class VulkanGraphicsPipelineParameters
        {
            KMP_LOG_CLASSNAME(VulkanGraphicsPipelineParameters)

        public:
            enum CopyParameters
            {
                OnlyParameters          = 0,
                DescriptorSetLayouts    = 1 << 0,
                ColorAttachmentInfos    = 1 << 1,
                DynamicStates           = 1 << 2,
                VertexInputBindings     = 1 << 3,
                VertexInputDescriptions = 1 << 4,
                ShaderStages            = 1 << 5,
                VertexInputs            = VertexInputBindings | VertexInputDescriptions,
                All                     = DescriptorSetLayouts | ColorAttachmentInfos | DynamicStates | VertexInputs | ShaderStages
            };

            KMP_NODISCARD KMP_API static VulkanGraphicsPipelineParameters CopyFrom(const VulkanGraphicsPipelineParameters& source, int copyParametersMask = OnlyParameters) noexcept;

        public:
            KMP_API VulkanGraphicsPipelineParameters();
            ~VulkanGraphicsPipelineParameters() = default;

            KMP_API VulkanGraphicsPipelineParameters& SetInputAssembly(VkPrimitiveTopology topology, bool primitiveRestartEnable);
            KMP_API VulkanGraphicsPipelineParameters& SetPolygonMode(VkPolygonMode polygonMode);
            KMP_API VulkanGraphicsPipelineParameters& SetCulling(VkCullModeFlags cullMode, VkFrontFace frontFace);
            KMP_API VulkanGraphicsPipelineParameters& SetDepthClamping(bool enabled);
            KMP_API VulkanGraphicsPipelineParameters& SetDepthBiasParameters(bool biasEnabled, float constantFactor, float clamp, float slopeFactor);
            KMP_API VulkanGraphicsPipelineParameters& SetRasterizerDiscard(bool enabled);
            KMP_API VulkanGraphicsPipelineParameters& SetLineWidth(float lineWidth);
            KMP_API VulkanGraphicsPipelineParameters& SetColorBlendConstants(Array<float, 4> constants);
            KMP_API VulkanGraphicsPipelineParameters& SetDepthTest(bool enabled);
            KMP_API VulkanGraphicsPipelineParameters& SetDepthWrite(bool enabled);
            KMP_API VulkanGraphicsPipelineParameters& SetDepthComparison(VkCompareOp compareOp);
            KMP_API VulkanGraphicsPipelineParameters& SetDepthBoundsTest(bool enabled, float min = 0.0f, float max = 1.0f);
            KMP_API VulkanGraphicsPipelineParameters& SetStencilTest(bool enabled);
            KMP_API VulkanGraphicsPipelineParameters& SetStencilStates(VkStencilOpState frontFaceState, VkStencilOpState backFaceState);
            KMP_API VulkanGraphicsPipelineParameters& SetMultisamplingSamples(VkSampleCountFlagBits samples);
            KMP_API VulkanGraphicsPipelineParameters& SetMultisamplingSampleShading(bool enabled, float minSampleShading);
            KMP_API VulkanGraphicsPipelineParameters& SetRenderingDepthStencilFormats(VkFormat depthFormat, VkFormat stencilFormat);
            KMP_API VulkanGraphicsPipelineParameters& SetLineRasterizationMode(VkLineRasterizationMode mode);
            KMP_API VulkanGraphicsPipelineParameters& SetLineStipple(bool enabled, UInt32 factor, UInt16 pattern);

            KMP_API VulkanGraphicsPipelineParameters& AddDescriptorSetLayout(VkDescriptorSetLayout descriptorSetLayout);
            KMP_API VulkanGraphicsPipelineParameters& AddColorAttachmentInfo(VkFormat attachmentFormat, VkPipelineColorBlendAttachmentState colorBlendAttachment);
            KMP_API VulkanGraphicsPipelineParameters& AddDynamicState(VkDynamicState dynamicState);
            KMP_API VulkanGraphicsPipelineParameters& AddDynamicStates(InitializerList<VkDynamicState> dynamicStates);
            KMP_API VulkanGraphicsPipelineParameters& AddVertexInputBindings(const Vector<VkVertexInputBindingDescription>& inputBindingDescriptions);
            KMP_API VulkanGraphicsPipelineParameters& AddVertexAttributesDescriptions(const Vector<VkVertexInputAttributeDescription>& attributesDescriptions);
            KMP_API VulkanGraphicsPipelineParameters& AddVertexBufferAttributesBindings(const VulkanVertexBuffer& vertexBuffer, UInt32 baseBinding);
            KMP_API VulkanGraphicsPipelineParameters& AddShaderStages(const Vector<VkPipelineShaderStageCreateInfo>& shaderStages);

            KMP_NODISCARD KMP_API UInt32 GetColorAttachmentsCount() const noexcept;

        private:
            void _SetDefaults() noexcept;

        private:
            friend class VulkanGraphicsPipeline;

            Vector<VkDescriptorSetLayout> _descriptorSetLayouts;
            VkPipelineInputAssemblyStateCreateInfo _inputAssemblyCreateInfo;
            VkPipelineRasterizationStateCreateInfo _rasterizationStateCreateInfo;
            VkPipelineRasterizationLineStateCreateInfo _rasterizationLineStateCreateInfo;
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