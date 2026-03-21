#include "Kmplete/Graphics/Vulkan/vulkan_graphics_pipeline.h"
#include "Kmplete/Graphics/Vulkan/Utils/initializers.h"
#include "Kmplete/Graphics/Vulkan/Utils/result_description.h"
#include "Kmplete/Math/math.h"
#include "Kmplete/Utils/vector_utils.h"
#include "Kmplete/Profile/profiler.h"
#include "Kmplete/Log/log.h"


namespace Kmplete
{
    namespace Graphics
    {
        VulkanGraphicsPipeline::VulkanGraphicsPipeline(VkDevice device, StringID sid)
            : _device(device)
            , _sid(sid)
            , _descriptorSetLayouts()
            , _pipelineLayout(VK_NULL_HANDLE)
            , _pipeline(VK_NULL_HANDLE)
            , _layoutCreateInfo()
            , _inputAssemblyCreateInfo()
            , _rasterizationStateCreateInfo()
            , _colorBlendStateCreateInfo()
            , _colorBlendAttachments()
            , _viewportStateCreateInfo()
            , _dynamicStateCreateInfo()
            , _dynamicStates()
            , _depthStencilStateCreateInfo()
            , _multisamplingStateCreateInfo()
            , _vertexInputStateCreateInfo()
            , _vertexInputBindings()
            , _vertexAttributesDescriptions()
            , _shadersStages()
            , _renderingCreateInfo()
            , _renderingColorAttachmentsFormats()
        {
            _layoutCreateInfo = VulkanUtils::InitVkPipelineLayoutCreateInfo();
            _inputAssemblyCreateInfo = VulkanUtils::InitVkPipelineInputAssemblyStateCreateInfo();
            _rasterizationStateCreateInfo = VulkanUtils::InitVkPipelineRasterizationStateCreateInfo();
            _colorBlendStateCreateInfo = VulkanUtils::InitVkPipelineColorBlendStateCreateInfo();
            _viewportStateCreateInfo = VulkanUtils::InitVkPipelineViewportStateCreateInfo();
            _dynamicStateCreateInfo = VulkanUtils::InitVkPipelineDynamicStateCreateInfo();
            _depthStencilStateCreateInfo = VulkanUtils::InitVkPipelineDepthStencilStateCreateInfo();
            _multisamplingStateCreateInfo = VulkanUtils::InitVkPipelineMultisampleStateCreateInfo();
            _vertexInputStateCreateInfo = VulkanUtils::InitVkPipelineVertexInputStateCreateInfo();
            _renderingCreateInfo = VulkanUtils::InitVkPipelineRenderingCreateInfoKHR();

            _SetupDefault();
        }
        //--------------------------------------------------------------------------

        VulkanGraphicsPipeline::~VulkanGraphicsPipeline()
        {
            if (_device != VK_NULL_HANDLE)
            {
                if (_pipeline != VK_NULL_HANDLE)
                {
                    vkDestroyPipeline(_device, _pipeline, nullptr);
                }

                if (_pipelineLayout != VK_NULL_HANDLE)
                {
                    vkDestroyPipelineLayout(_device, _pipelineLayout, nullptr);
                }
            }
        }
        //--------------------------------------------------------------------------

        bool VulkanGraphicsPipeline::Build()
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

            _layoutCreateInfo.setLayoutCount = UInt32(_descriptorSetLayouts.size());
            _layoutCreateInfo.pSetLayouts = _descriptorSetLayouts.data();

            auto result = vkCreatePipelineLayout(_device, &_layoutCreateInfo, nullptr, &_pipelineLayout);
            VulkanUtils::CheckResult(result, "VulkanGraphicsPipeline: failed to build graphics pipeline layout", false);
            if (result != VK_SUCCESS)
            {
                return false;
            }

            _colorBlendStateCreateInfo.attachmentCount = UInt32(_colorBlendAttachments.size());
            _colorBlendStateCreateInfo.pAttachments = _colorBlendAttachments.data();

            _dynamicStateCreateInfo.dynamicStateCount = UInt32(_dynamicStates.size());
            _dynamicStateCreateInfo.pDynamicStates = _dynamicStates.data();

            _vertexInputStateCreateInfo.vertexBindingDescriptionCount = UInt32(_vertexInputBindings.size());
            _vertexInputStateCreateInfo.pVertexBindingDescriptions = _vertexInputBindings.data();
            _vertexInputStateCreateInfo.vertexAttributeDescriptionCount = UInt32(_vertexAttributesDescriptions.size());
            _vertexInputStateCreateInfo.pVertexAttributeDescriptions = _vertexAttributesDescriptions.data();

            _renderingCreateInfo.colorAttachmentCount = UInt32(_renderingColorAttachmentsFormats.size());
            _renderingCreateInfo.pColorAttachmentFormats = _renderingColorAttachmentsFormats.data();

            auto pipelineCI = Graphics::VulkanUtils::InitVkGraphicsPipelineCreateInfo();
            pipelineCI.layout = _pipelineLayout;
            pipelineCI.pInputAssemblyState = &_inputAssemblyCreateInfo;
            pipelineCI.pRasterizationState = &_rasterizationStateCreateInfo;
            pipelineCI.pColorBlendState = &_colorBlendStateCreateInfo;
            pipelineCI.pViewportState = &_viewportStateCreateInfo;
            pipelineCI.pDynamicState = &_dynamicStateCreateInfo;
            pipelineCI.pDepthStencilState = &_depthStencilStateCreateInfo;
            pipelineCI.pMultisampleState = &_multisamplingStateCreateInfo;
            pipelineCI.pVertexInputState = &_vertexInputStateCreateInfo;
            pipelineCI.stageCount = UInt32(_shadersStages.size());
            pipelineCI.pStages = _shadersStages.data();
            pipelineCI.pNext = &_renderingCreateInfo;

            result = vkCreateGraphicsPipelines(_device, VK_NULL_HANDLE, 1, &pipelineCI, nullptr, &_pipeline);
            VulkanUtils::CheckResult(result, "VulkanGraphicsPipeline: failed to build graphics pipeline", false);
            
            return result == VK_SUCCESS;
        }
        //--------------------------------------------------------------------------

        VulkanGraphicsPipeline& VulkanGraphicsPipeline::AddDescriptorSetLayout(VkDescriptorSetLayout descriptorSetLayout)
        {
            _descriptorSetLayouts.push_back(descriptorSetLayout);
            return *this;
        }
        //--------------------------------------------------------------------------

        VulkanGraphicsPipeline& VulkanGraphicsPipeline::SetupInputAssembly(VkPrimitiveTopology topology, bool primitiveRestartEnable)
        {
            _inputAssemblyCreateInfo.topology = topology;
            _inputAssemblyCreateInfo.primitiveRestartEnable = primitiveRestartEnable;
            return *this;
        }
        //--------------------------------------------------------------------------

        VulkanGraphicsPipeline& VulkanGraphicsPipeline::SetupPolygonMode(VkPolygonMode polygonMode)
        {
            _rasterizationStateCreateInfo.polygonMode = polygonMode;
            return *this;
        }
        //--------------------------------------------------------------------------

        VulkanGraphicsPipeline& VulkanGraphicsPipeline::SetupCulling(VkCullModeFlags cullMode, VkFrontFace frontFace)
        {
            _rasterizationStateCreateInfo.cullMode = cullMode;
            _rasterizationStateCreateInfo.frontFace = frontFace;
            return *this;
        }
        //--------------------------------------------------------------------------

        VulkanGraphicsPipeline& VulkanGraphicsPipeline::SetupDepthClamping(bool enabled)
        {
            _rasterizationStateCreateInfo.depthClampEnable = enabled;
            return *this;
        }
        //--------------------------------------------------------------------------

        VulkanGraphicsPipeline& VulkanGraphicsPipeline::SetupDepthBiasParameters(bool biasEnabled, float constantFactor, float clamp, float slopeFactor)
        {
            _rasterizationStateCreateInfo.depthBiasEnable = biasEnabled;
            _rasterizationStateCreateInfo.depthBiasConstantFactor = constantFactor;
            _rasterizationStateCreateInfo.depthBiasClamp = clamp;
            _rasterizationStateCreateInfo.depthBiasSlopeFactor = slopeFactor;
            return *this;
        }
        //--------------------------------------------------------------------------

        VulkanGraphicsPipeline& VulkanGraphicsPipeline::SetupRasterizerDiscard(bool enabled)
        {
            _rasterizationStateCreateInfo.rasterizerDiscardEnable = enabled;
            return *this;
        }
        //--------------------------------------------------------------------------

        VulkanGraphicsPipeline& VulkanGraphicsPipeline::SetupLineWidth(float lineWidth)
        {
            if (lineWidth <= 0.0f)
            {
                KMP_LOG_WARN("line width should be positive, set 1.0 by default (given {})", lineWidth);
                lineWidth = 1.0f;
            }

            _rasterizationStateCreateInfo.lineWidth = lineWidth;
            return *this;
        }
        //--------------------------------------------------------------------------

        VulkanGraphicsPipeline& VulkanGraphicsPipeline::SetupColorBlendConstants(Array<float, 4> constants)
        {
            _colorBlendStateCreateInfo.blendConstants[0] = constants[0];
            _colorBlendStateCreateInfo.blendConstants[1] = constants[1];
            _colorBlendStateCreateInfo.blendConstants[2] = constants[2];
            _colorBlendStateCreateInfo.blendConstants[3] = constants[3];
            return *this;
        }
        //--------------------------------------------------------------------------

        VulkanGraphicsPipeline& VulkanGraphicsPipeline::AddColorBlendAttachment(VkPipelineColorBlendAttachmentState colorBlendAttachment)
        {
            _colorBlendAttachments.push_back(colorBlendAttachment);
            return *this;
        }
        //--------------------------------------------------------------------------

        VulkanGraphicsPipeline& VulkanGraphicsPipeline::AddDynamicState(VkDynamicState dynamicState)
        {
            if (!Utils::VectorContains(_dynamicStates, dynamicState))
            {
                _dynamicStates.push_back(dynamicState);
            }

            return *this;
        }
        //--------------------------------------------------------------------------

        VulkanGraphicsPipeline& VulkanGraphicsPipeline::AddDynamicStates(std::initializer_list<VkDynamicState> dynamicStates)
        {
            for (const auto& state : dynamicStates)
            {
                if (!Utils::VectorContains(_dynamicStates, state))
                {
                    _dynamicStates.push_back(state);
                }
            }

            return *this;
        }
        //--------------------------------------------------------------------------

        VulkanGraphicsPipeline& VulkanGraphicsPipeline::SetupDepthTest(bool enabled)
        {
            _depthStencilStateCreateInfo.depthTestEnable = enabled;
            return *this;
        }
        //--------------------------------------------------------------------------

        VulkanGraphicsPipeline& VulkanGraphicsPipeline::SetupDepthWrite(bool enabled)
        {
            _depthStencilStateCreateInfo.depthWriteEnable = enabled;
            return *this;
        }
        //--------------------------------------------------------------------------

        VulkanGraphicsPipeline& VulkanGraphicsPipeline::SetupDepthComparison(VkCompareOp compareOp)
        {
            _depthStencilStateCreateInfo.depthCompareOp = compareOp;
            return *this;
        }
        //--------------------------------------------------------------------------

        VulkanGraphicsPipeline& VulkanGraphicsPipeline::SetupDepthBoundsTest(bool enabled, float min /*= 0.0f*/, float max /*= 1.0f*/)
        {
            _depthStencilStateCreateInfo.depthBoundsTestEnable = enabled;
            _depthStencilStateCreateInfo.minDepthBounds = min;
            _depthStencilStateCreateInfo.maxDepthBounds = max;
            return *this;
        }
        //--------------------------------------------------------------------------

        VulkanGraphicsPipeline& VulkanGraphicsPipeline::SetupStencilTest(bool enabled)
        {
            _depthStencilStateCreateInfo.stencilTestEnable = enabled;
            return *this;
        }
        //--------------------------------------------------------------------------

        VulkanGraphicsPipeline& VulkanGraphicsPipeline::SetupStencilStates(VkStencilOpState frontFaceState, VkStencilOpState backFaceState)
        {
            _depthStencilStateCreateInfo.front = frontFaceState;
            _depthStencilStateCreateInfo.back = backFaceState;
            return *this;
        }
        //--------------------------------------------------------------------------

        VulkanGraphicsPipeline& VulkanGraphicsPipeline::SetupMultisamplingSamples(VkSampleCountFlagBits samples)
        {
            _multisamplingStateCreateInfo.rasterizationSamples = samples;
            return *this;
        }
        //--------------------------------------------------------------------------

        VulkanGraphicsPipeline& VulkanGraphicsPipeline::SetupMultisamplingSampleShading(bool enabled, float minSampleShading)
        {
            _multisamplingStateCreateInfo.sampleShadingEnable = enabled;
            _multisamplingStateCreateInfo.minSampleShading = minSampleShading;
            return *this;
        }
        //--------------------------------------------------------------------------

        VulkanGraphicsPipeline& VulkanGraphicsPipeline::AddVertexInputBindings(Vector<VkVertexInputBindingDescription>&& inputBindingDescriptions)
        {
            Utils::MergeVectors(inputBindingDescriptions, _vertexInputBindings);
            return *this;
        }
        //--------------------------------------------------------------------------

        VulkanGraphicsPipeline& VulkanGraphicsPipeline::AddVertexAttributesDescriptions(Vector<VkVertexInputAttributeDescription>&& attributesDescriptions)
        {
            Utils::MergeVectors(attributesDescriptions, _vertexAttributesDescriptions);
            return *this;
        }
        //--------------------------------------------------------------------------

        VulkanGraphicsPipeline& VulkanGraphicsPipeline::AddShaderStages(Vector<VkPipelineShaderStageCreateInfo>&& shaderStages)
        {
            Utils::MergeVectors(shaderStages, _shadersStages);
            return *this;
        }
        //--------------------------------------------------------------------------

        VulkanGraphicsPipeline& VulkanGraphicsPipeline::AddRenderingColorAttachment(VkFormat attachmentFormat)
        {
            _renderingColorAttachmentsFormats.push_back(attachmentFormat);
            return *this;
        }
        //--------------------------------------------------------------------------
        
        VulkanGraphicsPipeline& VulkanGraphicsPipeline::SetupRenderingDepthStencilFormats(VkFormat depthFormat, VkFormat stencilFormat)
        {
            _renderingCreateInfo.depthAttachmentFormat = depthFormat;
            _renderingCreateInfo.stencilAttachmentFormat = stencilFormat;
            return *this;
        }
        //--------------------------------------------------------------------------

        VkPipeline VulkanGraphicsPipeline::GetVkPipeline() const noexcept
        {
            return _pipeline;
        }
        //--------------------------------------------------------------------------

        VkPipelineLayout VulkanGraphicsPipeline::GetVkPipelineLayout() const noexcept
        {
            return _pipelineLayout;
        }
        //--------------------------------------------------------------------------

        void VulkanGraphicsPipeline::_SetupDefault() noexcept
        {
            _inputAssemblyCreateInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
            _inputAssemblyCreateInfo.primitiveRestartEnable = VK_FALSE;

            _rasterizationStateCreateInfo.polygonMode = VK_POLYGON_MODE_FILL;
            _rasterizationStateCreateInfo.cullMode = VK_CULL_MODE_BACK_BIT;
            _rasterizationStateCreateInfo.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
            _rasterizationStateCreateInfo.depthClampEnable = VK_FALSE;
            _rasterizationStateCreateInfo.rasterizerDiscardEnable = VK_TRUE;
            _rasterizationStateCreateInfo.lineWidth = 1.0f;
            _rasterizationStateCreateInfo.depthBiasEnable = VK_FALSE;
            _rasterizationStateCreateInfo.depthBiasConstantFactor = 0.0f;
            _rasterizationStateCreateInfo.depthBiasClamp = 0.0f;
            _rasterizationStateCreateInfo.depthBiasSlopeFactor = 0.0f;

            _colorBlendStateCreateInfo.logicOpEnable = VK_FALSE;
            _colorBlendStateCreateInfo.logicOp = VK_LOGIC_OP_CLEAR;
            _colorBlendStateCreateInfo.blendConstants[0] = 0.0f;
            _colorBlendStateCreateInfo.blendConstants[1] = 0.0f;
            _colorBlendStateCreateInfo.blendConstants[2] = 0.0f;
            _colorBlendStateCreateInfo.blendConstants[3] = 0.0f;

            _viewportStateCreateInfo.viewportCount = 1;
            _viewportStateCreateInfo.scissorCount = 1;

            _depthStencilStateCreateInfo.depthTestEnable = VK_TRUE;
            _depthStencilStateCreateInfo.depthWriteEnable = VK_TRUE;
            _depthStencilStateCreateInfo.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
            _depthStencilStateCreateInfo.depthBoundsTestEnable = VK_FALSE;
            _depthStencilStateCreateInfo.minDepthBounds = 0.0f;
            _depthStencilStateCreateInfo.maxDepthBounds = 1.0f;
            _depthStencilStateCreateInfo.stencilTestEnable = VK_FALSE;
            _depthStencilStateCreateInfo.back.failOp = VK_STENCIL_OP_KEEP;
            _depthStencilStateCreateInfo.back.passOp = VK_STENCIL_OP_KEEP;
            _depthStencilStateCreateInfo.back.compareOp = VK_COMPARE_OP_ALWAYS;
            _depthStencilStateCreateInfo.front = _depthStencilStateCreateInfo.back;

            _multisamplingStateCreateInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
            _multisamplingStateCreateInfo.sampleShadingEnable = VK_FALSE;
            _multisamplingStateCreateInfo.minSampleShading = 0.0f;
            _multisamplingStateCreateInfo.pSampleMask = nullptr;
            _multisamplingStateCreateInfo.alphaToCoverageEnable = VK_FALSE;
            _multisamplingStateCreateInfo.alphaToOneEnable = VK_FALSE;

            AddDynamicStates({ VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR, VK_DYNAMIC_STATE_RASTERIZATION_SAMPLES_EXT });
        }
        //--------------------------------------------------------------------------
    }
}