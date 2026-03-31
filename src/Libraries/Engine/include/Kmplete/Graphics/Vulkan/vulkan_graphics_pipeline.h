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
        class VulkanVertexBuffer;


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
            KMP_API VulkanGraphicsPipeline& SetInputAssembly(VkPrimitiveTopology topology, bool primitiveRestartEnable);
            KMP_API VulkanGraphicsPipeline& SetPolygonMode(VkPolygonMode polygonMode);
            KMP_API VulkanGraphicsPipeline& SetCulling(VkCullModeFlags cullMode, VkFrontFace frontFace);
            KMP_API VulkanGraphicsPipeline& SetDepthClamping(bool enabled);
            KMP_API VulkanGraphicsPipeline& SetDepthBiasParameters(bool biasEnabled, float constantFactor, float clamp, float slopeFactor);
            KMP_API VulkanGraphicsPipeline& SetRasterizerDiscard(bool enabled);
            KMP_API VulkanGraphicsPipeline& SetLineWidth(float lineWidth);
            KMP_API VulkanGraphicsPipeline& SetColorBlendConstants(Array<float, 4> constants);
            KMP_API VulkanGraphicsPipeline& AddColorAttachmentInfo(VkFormat attachmentFormat, VkPipelineColorBlendAttachmentState colorBlendAttachment);
            KMP_API VulkanGraphicsPipeline& AddDynamicState(VkDynamicState dynamicState);
            KMP_API VulkanGraphicsPipeline& AddDynamicStates(std::initializer_list<VkDynamicState> dynamicStates);
            KMP_API VulkanGraphicsPipeline& SetDepthTest(bool enabled);
            KMP_API VulkanGraphicsPipeline& SetDepthWrite(bool enabled);
            KMP_API VulkanGraphicsPipeline& SetDepthComparison(VkCompareOp compareOp);
            KMP_API VulkanGraphicsPipeline& SetDepthBoundsTest(bool enabled, float min = 0.0f, float max = 1.0f);
            KMP_API VulkanGraphicsPipeline& SetStencilTest(bool enabled);
            KMP_API VulkanGraphicsPipeline& SetStencilStates(VkStencilOpState frontFaceState, VkStencilOpState backFaceState);
            KMP_API VulkanGraphicsPipeline& SetMultisamplingSamples(VkSampleCountFlagBits samples);
            KMP_API VulkanGraphicsPipeline& SetMultisamplingSampleShading(bool enabled, float minSampleShading);
            KMP_API VulkanGraphicsPipeline& AddVertexInputBindings(Vector<VkVertexInputBindingDescription>&& inputBindingDescriptions);
            KMP_API VulkanGraphicsPipeline& AddVertexAttributesDescriptions(Vector<VkVertexInputAttributeDescription>&& attributesDescriptions);
            KMP_API VulkanGraphicsPipeline& AddVertexBufferAttributesBindings(const VulkanVertexBuffer& vertexBuffer, UInt32 baseBinding);
            KMP_API VulkanGraphicsPipeline& AddShaderStages(Vector<VkPipelineShaderStageCreateInfo>&& shaderStages);
            KMP_API VulkanGraphicsPipeline& SetRenderingDepthStencilFormats(VkFormat depthFormat, VkFormat stencilFormat);

            KMP_NODISCARD KMP_API VkPipeline GetVkPipeline() const noexcept;
            KMP_NODISCARD KMP_API VkPipelineLayout GetVkPipelineLayout() const noexcept;
            KMP_NODISCARD KMP_API UInt32 GetColorAttachmentsCount() const noexcept;

        private:
            void _SetDefaults() noexcept;

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