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


        //! Vulkan API graphics pipeline creation parameters wrapper to simplify pipeline stages
        //! construction. A premade parameters object could be used as a reference for copying
        //! to another parameters object with optional stage categories mask.
        //! @see VulkanGraphicsPipeline
        class KMP_API VulkanGraphicsPipelineParameters
        {
            KMP_LOG_CLASSNAME(VulkanGraphicsPipelineParameters)

        public:
            //! Enumeration of categories of pipeline stages to copy from source object
            enum CopyParameters
            {
                OnlyParameters          = 0,
                ColorAttachmentInfos    = 1 << 0,
                DynamicStates           = 1 << 1,
                VertexInputBindings     = 1 << 2,
                VertexInputDescriptions = 1 << 3,
                ShaderStages            = 1 << 4,
                PushConstantRanges      = 1 << 5,
                VertexInputs            = VertexInputBindings | VertexInputDescriptions,
                All                     = ColorAttachmentInfos | DynamicStates | VertexInputs | ShaderStages | PushConstantRanges
            };

            KMP_NODISCARD static VulkanGraphicsPipelineParameters CopyFrom(const VulkanGraphicsPipelineParameters& source, int copyParametersMask = OnlyParameters) noexcept;

        public:
            VulkanGraphicsPipelineParameters();
            ~VulkanGraphicsPipelineParameters() = default;

            VulkanGraphicsPipelineParameters& SetInputAssembly(VkPrimitiveTopology topology, bool primitiveRestartEnable);
            VulkanGraphicsPipelineParameters& SetPolygonMode(VkPolygonMode polygonMode);
            VulkanGraphicsPipelineParameters& SetCulling(VkCullModeFlags cullMode, VkFrontFace frontFace);
            VulkanGraphicsPipelineParameters& SetDepthBiasParameters(bool biasEnabled, float constantFactor, float clamp, float slopeFactor);
            VulkanGraphicsPipelineParameters& SetRasterizerDiscard(bool enabled);
            VulkanGraphicsPipelineParameters& SetLineWidth(float lineWidth);
            VulkanGraphicsPipelineParameters& SetColorBlendConstants(Array<float, 4> constants);
            VulkanGraphicsPipelineParameters& SetDepthTest(bool enabled);
            VulkanGraphicsPipelineParameters& SetDepthWrite(bool enabled);
            VulkanGraphicsPipelineParameters& SetDepthComparison(VkCompareOp compareOp);
            VulkanGraphicsPipelineParameters& SetDepthBoundsTest(bool enabled, float min = 0.0f, float max = 1.0f);
            VulkanGraphicsPipelineParameters& SetStencilTest(bool enabled);
            VulkanGraphicsPipelineParameters& SetStencilStates(VkStencilOpState frontFaceState, VkStencilOpState backFaceState);
            VulkanGraphicsPipelineParameters& SetMultisamplingSamples(VkSampleCountFlagBits samples);
            VulkanGraphicsPipelineParameters& SetMultisamplingSampleShading(bool enabled, float minSampleShading);
            VulkanGraphicsPipelineParameters& SetRenderingDepthStencilFormats(VkFormat depthFormat, VkFormat stencilFormat);
            VulkanGraphicsPipelineParameters& SetLineRasterizationMode(VkLineRasterizationMode mode);
            VulkanGraphicsPipelineParameters& SetLineStipple(bool enabled, UInt32 factor, UInt16 pattern);

            VulkanGraphicsPipelineParameters& AddColorAttachmentInfo(VkFormat attachmentFormat, VkPipelineColorBlendAttachmentState colorBlendAttachment);
            VulkanGraphicsPipelineParameters& AddDynamicState(VkDynamicState dynamicState);
            VulkanGraphicsPipelineParameters& AddDynamicStates(InitializerList<VkDynamicState> dynamicStates);
            VulkanGraphicsPipelineParameters& AddVertexInputBindings(const Vector<VkVertexInputBindingDescription>& inputBindingDescriptions);
            VulkanGraphicsPipelineParameters& AddVertexInputBindingsDivisors(const Vector<VkVertexInputBindingDivisorDescription>& inputBindingDivisorsDescriptions);
            VulkanGraphicsPipelineParameters& AddVertexAttributesDescriptions(const Vector<VkVertexInputAttributeDescription>& attributesDescriptions);
            VulkanGraphicsPipelineParameters& AddVertexBufferAttributesBindings(const VulkanVertexBuffer& vertexBuffer, UInt32 baseBinding);
            VulkanGraphicsPipelineParameters& AddShaderStages(const Vector<VkPipelineShaderStageCreateInfo>& shaderStages);

            KMP_NODISCARD UInt32 GetColorAttachmentsCount() const noexcept;

        private:
            VulkanGraphicsPipelineParameters(const VulkanGraphicsPipelineParameters&) = default;

            void _SetDefaults() noexcept;

        private:
            friend class VulkanGraphicsPipeline;

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
            VkPipelineVertexInputDivisorStateCreateInfo _vertexInputDivisorStateCreateInfo;
            Vector<VkVertexInputBindingDescription> _vertexInputBindings;
            Vector<VkVertexInputBindingDivisorDescription> _vertexInputBindingsDivisors;
            Vector<VkVertexInputAttributeDescription> _vertexAttributesDescriptions;
            Vector<VkPipelineShaderStageCreateInfo> _shadersStages;
            VkPipelineRenderingCreateInfoKHR _renderingCreateInfo;
            Vector<VkFormat> _renderingColorAttachmentsFormats;
        };
        //--------------------------------------------------------------------------
    }
}