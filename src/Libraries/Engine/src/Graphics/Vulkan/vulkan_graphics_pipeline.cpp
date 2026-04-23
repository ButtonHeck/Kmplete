#include "Kmplete/Graphics/Vulkan/vulkan_graphics_pipeline.h"
#include "Kmplete/Graphics/Vulkan/vulkan_vertex_buffer.h"
#include "Kmplete/Graphics/Vulkan/Utils/initializers.h"
#include "Kmplete/Graphics/Vulkan/Utils/result_description.h"
#include "Kmplete/Graphics/Vulkan/Utils/bits_aliases.h"
#include "Kmplete/Base/named_bool.h"
#include "Kmplete/Math/math.h"
#include "Kmplete/Core/assertion.h"
#include "Kmplete/Utils/vector_utils.h"
#include "Kmplete/Profile/profiler.h"
#include "Kmplete/Log/log.h"


namespace Kmplete
{
    namespace Graphics
    {
        using namespace VKBits;


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
            KMP_PROFILE_FUNCTION(ProfileLevelAlways);

            _Initialize();
        }
        //--------------------------------------------------------------------------

        VulkanGraphicsPipeline::~VulkanGraphicsPipeline() KMP_PROFILING(ProfileLevelAlways)
        {
            _Finalize();
        }}
        //--------------------------------------------------------------------------

        bool VulkanGraphicsPipeline::Build() KMP_PROFILING(ProfileLevelImportant)
        {
            KMP_ASSERT(_device);

            if (_pipelineLayout && _pipeline)
            {
                KMP_LOG_WARN("pipeline has already been built");
                return true;
            }

            _layoutCreateInfo.setLayoutCount = UInt32(_descriptorSetLayouts.size());
            _layoutCreateInfo.pSetLayouts = _descriptorSetLayouts.data();

            auto result = vkCreatePipelineLayout(_device, &_layoutCreateInfo, nullptr, &_pipelineLayout);
            VKUtils::CheckResult(result, "VulkanGraphicsPipeline: failed to build graphics pipeline layout", "throw exception"_false);
            if (result != VK_SUCCESS)
            {
                return false;
            }
            KMP_ASSERT(_pipelineLayout);

            _colorBlendStateCreateInfo.attachmentCount = UInt32(_colorBlendAttachments.size());
            _colorBlendStateCreateInfo.pAttachments = _colorBlendAttachments.data();
            _renderingCreateInfo.colorAttachmentCount = UInt32(_renderingColorAttachmentsFormats.size());
            _renderingCreateInfo.pColorAttachmentFormats = _renderingColorAttachmentsFormats.data();
            KMP_ASSERT(_renderingCreateInfo.colorAttachmentCount == _colorBlendStateCreateInfo.attachmentCount);

            _dynamicStateCreateInfo.dynamicStateCount = UInt32(_dynamicStates.size());
            _dynamicStateCreateInfo.pDynamicStates = _dynamicStates.data();

            _vertexInputStateCreateInfo.vertexBindingDescriptionCount = UInt32(_vertexInputBindings.size());
            _vertexInputStateCreateInfo.pVertexBindingDescriptions = _vertexInputBindings.data();
            _vertexInputStateCreateInfo.vertexAttributeDescriptionCount = UInt32(_vertexAttributesDescriptions.size());
            _vertexInputStateCreateInfo.pVertexAttributeDescriptions = _vertexAttributesDescriptions.data();

            auto pipelineCI = Graphics::VKUtils::InitVkGraphicsPipelineCreateInfo();
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
            VKUtils::CheckResult(result, "VulkanGraphicsPipeline: failed to build graphics pipeline", "throw exception"_false);
            if (result != VK_SUCCESS)
            {
                return false;
            }
            KMP_ASSERT(_pipeline);
            
            return true;
        }}
        //--------------------------------------------------------------------------

        VulkanGraphicsPipeline& VulkanGraphicsPipeline::SetInputAssembly(VkPrimitiveTopology topology, bool primitiveRestartEnable)
        {
            _inputAssemblyCreateInfo.topology = topology;
            _inputAssemblyCreateInfo.primitiveRestartEnable = primitiveRestartEnable;
            return *this;
        }
        //--------------------------------------------------------------------------

        VulkanGraphicsPipeline& VulkanGraphicsPipeline::SetPolygonMode(VkPolygonMode polygonMode)
        {
            _rasterizationStateCreateInfo.polygonMode = polygonMode;
            return *this;
        }
        //--------------------------------------------------------------------------

        VulkanGraphicsPipeline& VulkanGraphicsPipeline::SetCulling(VkCullModeFlags cullMode, VkFrontFace frontFace)
        {
            _rasterizationStateCreateInfo.cullMode = cullMode;
            _rasterizationStateCreateInfo.frontFace = frontFace;
            return *this;
        }
        //--------------------------------------------------------------------------

        VulkanGraphicsPipeline& VulkanGraphicsPipeline::SetDepthClamping(bool enabled)
        {
            _rasterizationStateCreateInfo.depthClampEnable = enabled;
            return *this;
        }
        //--------------------------------------------------------------------------

        VulkanGraphicsPipeline& VulkanGraphicsPipeline::SetDepthBiasParameters(bool biasEnabled, float constantFactor, float clamp, float slopeFactor)
        {
            _rasterizationStateCreateInfo.depthBiasEnable = biasEnabled;
            _rasterizationStateCreateInfo.depthBiasConstantFactor = constantFactor;
            _rasterizationStateCreateInfo.depthBiasClamp = clamp;
            _rasterizationStateCreateInfo.depthBiasSlopeFactor = slopeFactor;
            return *this;
        }
        //--------------------------------------------------------------------------

        VulkanGraphicsPipeline& VulkanGraphicsPipeline::SetRasterizerDiscard(bool enabled)
        {
            _rasterizationStateCreateInfo.rasterizerDiscardEnable = enabled;
            return *this;
        }
        //--------------------------------------------------------------------------

        VulkanGraphicsPipeline& VulkanGraphicsPipeline::SetLineWidth(float lineWidth)
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

        VulkanGraphicsPipeline& VulkanGraphicsPipeline::SetColorBlendConstants(Array<float, 4> constants)
        {
            _colorBlendStateCreateInfo.blendConstants[0] = constants[0];
            _colorBlendStateCreateInfo.blendConstants[1] = constants[1];
            _colorBlendStateCreateInfo.blendConstants[2] = constants[2];
            _colorBlendStateCreateInfo.blendConstants[3] = constants[3];
            return *this;
        }
        //--------------------------------------------------------------------------

        VulkanGraphicsPipeline& VulkanGraphicsPipeline::SetDepthTest(bool enabled)
        {
            _depthStencilStateCreateInfo.depthTestEnable = enabled;
            return *this;
        }
        //--------------------------------------------------------------------------

        VulkanGraphicsPipeline& VulkanGraphicsPipeline::SetDepthWrite(bool enabled)
        {
            _depthStencilStateCreateInfo.depthWriteEnable = enabled;
            return *this;
        }
        //--------------------------------------------------------------------------

        VulkanGraphicsPipeline& VulkanGraphicsPipeline::SetDepthComparison(VkCompareOp compareOp)
        {
            _depthStencilStateCreateInfo.depthCompareOp = compareOp;
            return *this;
        }
        //--------------------------------------------------------------------------

        VulkanGraphicsPipeline& VulkanGraphicsPipeline::SetDepthBoundsTest(bool enabled, float min /*= 0.0f*/, float max /*= 1.0f*/)
        {
            _depthStencilStateCreateInfo.depthBoundsTestEnable = enabled;
            _depthStencilStateCreateInfo.minDepthBounds = min;
            _depthStencilStateCreateInfo.maxDepthBounds = max;
            return *this;
        }
        //--------------------------------------------------------------------------

        VulkanGraphicsPipeline& VulkanGraphicsPipeline::SetStencilTest(bool enabled)
        {
            _depthStencilStateCreateInfo.stencilTestEnable = enabled;
            return *this;
        }
        //--------------------------------------------------------------------------

        VulkanGraphicsPipeline& VulkanGraphicsPipeline::SetStencilStates(VkStencilOpState frontFaceState, VkStencilOpState backFaceState)
        {
            _depthStencilStateCreateInfo.front = frontFaceState;
            _depthStencilStateCreateInfo.back = backFaceState;
            return *this;
        }
        //--------------------------------------------------------------------------

        VulkanGraphicsPipeline& VulkanGraphicsPipeline::SetMultisamplingSamples(VkSampleCountFlagBits samples)
        {
            _multisamplingStateCreateInfo.rasterizationSamples = samples;
            return *this;
        }
        //--------------------------------------------------------------------------

        VulkanGraphicsPipeline& VulkanGraphicsPipeline::SetMultisamplingSampleShading(bool enabled, float minSampleShading)
        {
            _multisamplingStateCreateInfo.sampleShadingEnable = enabled;
            _multisamplingStateCreateInfo.minSampleShading = minSampleShading;
            return *this;
        }
        //--------------------------------------------------------------------------

        VulkanGraphicsPipeline& VulkanGraphicsPipeline::SetRenderingDepthStencilFormats(VkFormat depthFormat, VkFormat stencilFormat)
        {
            _renderingCreateInfo.depthAttachmentFormat = depthFormat;
            _renderingCreateInfo.stencilAttachmentFormat = stencilFormat;
            return *this;
        }
        //--------------------------------------------------------------------------

        VulkanGraphicsPipeline& VulkanGraphicsPipeline::AddDescriptorSetLayout(VkDescriptorSetLayout descriptorSetLayout)
        {
            _descriptorSetLayouts.push_back(descriptorSetLayout);
            return *this;
        }
        //--------------------------------------------------------------------------

        VulkanGraphicsPipeline& VulkanGraphicsPipeline::AddColorAttachmentInfo(VkFormat attachmentFormat, VkPipelineColorBlendAttachmentState colorBlendAttachment)
        {
            _renderingColorAttachmentsFormats.push_back(attachmentFormat);
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

            if (dynamicState == VK_Dynamic_ViewportWithCount)
            {
                _viewportStateCreateInfo.viewportCount = 0;
            }
            else if (dynamicState == VK_Dynamic_ScissorWithCount)
            {
                _viewportStateCreateInfo.scissorCount = 0;
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

        VulkanGraphicsPipeline& VulkanGraphicsPipeline::AddVertexBufferAttributesBindings(const VulkanVertexBuffer& vertexBuffer, UInt32 baseBinding)
        {
            auto [inputDescriptions, attributeDescriptions] = vertexBuffer.GetBindingsDescriptions(baseBinding);
            AddVertexInputBindings(std::move(inputDescriptions));
            AddVertexAttributesDescriptions(std::move(attributeDescriptions));
            return *this;
        }
        //--------------------------------------------------------------------------

        VulkanGraphicsPipeline& VulkanGraphicsPipeline::AddShaderStages(Vector<VkPipelineShaderStageCreateInfo>&& shaderStages)
        {
            Utils::MergeVectors(shaderStages, _shadersStages);
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

        UInt32 VulkanGraphicsPipeline::GetColorAttachmentsCount() const noexcept
        {
            return _renderingCreateInfo.colorAttachmentCount;
        }
        //--------------------------------------------------------------------------

        void VulkanGraphicsPipeline::_Initialize()
        {
            _layoutCreateInfo = VKUtils::InitVkPipelineLayoutCreateInfo();
            _inputAssemblyCreateInfo = VKUtils::InitVkPipelineInputAssemblyStateCreateInfo();
            _rasterizationStateCreateInfo = VKUtils::InitVkPipelineRasterizationStateCreateInfo();
            _colorBlendStateCreateInfo = VKUtils::InitVkPipelineColorBlendStateCreateInfo();
            _viewportStateCreateInfo = VKUtils::InitVkPipelineViewportStateCreateInfo();
            _dynamicStateCreateInfo = VKUtils::InitVkPipelineDynamicStateCreateInfo();
            _depthStencilStateCreateInfo = VKUtils::InitVkPipelineDepthStencilStateCreateInfo();
            _multisamplingStateCreateInfo = VKUtils::InitVkPipelineMultisampleStateCreateInfo();
            _vertexInputStateCreateInfo = VKUtils::InitVkPipelineVertexInputStateCreateInfo();
            _renderingCreateInfo = VKUtils::InitVkPipelineRenderingCreateInfoKHR();

            _SetDefaults();
        }
        //--------------------------------------------------------------------------

        void VulkanGraphicsPipeline::_Finalize()
        {
            if (_device && _pipeline)
            {
                vkDestroyPipeline(_device, _pipeline, nullptr);
            }

            if (_device && _pipelineLayout)
            {
                vkDestroyPipelineLayout(_device, _pipelineLayout, nullptr);
            }
        }
        //--------------------------------------------------------------------------

        void VulkanGraphicsPipeline::_SetDefaults() noexcept
        {
            _inputAssemblyCreateInfo.topology = VK_Primitive_TriangleList;
            _inputAssemblyCreateInfo.primitiveRestartEnable = VK_FALSE;

            _rasterizationStateCreateInfo.polygonMode = VK_Polygon_Fill;
            _rasterizationStateCreateInfo.cullMode = VK_Cull_Back;
            _rasterizationStateCreateInfo.frontFace = VK_FrontFace_CounterClockwise;
            _rasterizationStateCreateInfo.depthClampEnable = VK_FALSE;
            _rasterizationStateCreateInfo.rasterizerDiscardEnable = VK_TRUE;
            _rasterizationStateCreateInfo.lineWidth = 1.0f;
            _rasterizationStateCreateInfo.depthBiasEnable = VK_FALSE;
            _rasterizationStateCreateInfo.depthBiasConstantFactor = 0.0f;
            _rasterizationStateCreateInfo.depthBiasClamp = 0.0f;
            _rasterizationStateCreateInfo.depthBiasSlopeFactor = 0.0f;

            _colorBlendStateCreateInfo.logicOpEnable = VK_FALSE;
            _colorBlendStateCreateInfo.logicOp = VK_LogicOp_Clear;
            _colorBlendStateCreateInfo.blendConstants[0] = 0.0f;
            _colorBlendStateCreateInfo.blendConstants[1] = 0.0f;
            _colorBlendStateCreateInfo.blendConstants[2] = 0.0f;
            _colorBlendStateCreateInfo.blendConstants[3] = 0.0f;

            _viewportStateCreateInfo.viewportCount = 1;
            _viewportStateCreateInfo.scissorCount = 1;

            _depthStencilStateCreateInfo.depthTestEnable = VK_TRUE;
            _depthStencilStateCreateInfo.depthWriteEnable = VK_TRUE;
            _depthStencilStateCreateInfo.depthCompareOp = VK_Compare_LessOrEqual;
            _depthStencilStateCreateInfo.depthBoundsTestEnable = VK_FALSE;
            _depthStencilStateCreateInfo.minDepthBounds = 0.0f;
            _depthStencilStateCreateInfo.maxDepthBounds = 1.0f;
            _depthStencilStateCreateInfo.stencilTestEnable = VK_FALSE;
            _depthStencilStateCreateInfo.back.failOp = VK_Stencil_Keep;
            _depthStencilStateCreateInfo.back.passOp = VK_Stencil_Keep;
            _depthStencilStateCreateInfo.back.compareOp = VK_Compare_Always;
            _depthStencilStateCreateInfo.front = _depthStencilStateCreateInfo.back;

            _multisamplingStateCreateInfo.rasterizationSamples = VK_SampleCount_1;
            _multisamplingStateCreateInfo.sampleShadingEnable = VK_FALSE;
            _multisamplingStateCreateInfo.minSampleShading = 0.0f;
            _multisamplingStateCreateInfo.pSampleMask = nullptr;
            _multisamplingStateCreateInfo.alphaToCoverageEnable = VK_FALSE;
            _multisamplingStateCreateInfo.alphaToOneEnable = VK_FALSE;
        }
        //--------------------------------------------------------------------------
    }
}