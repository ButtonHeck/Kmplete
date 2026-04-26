#include "Kmplete/Graphics/Vulkan/vulkan_graphics_pipeline_parameters.h"
#include "Kmplete/Graphics/Vulkan/vulkan_vertex_buffer.h"
#include "Kmplete/Graphics/Vulkan/Utils/initializers.h"
#include "Kmplete/Graphics/Vulkan/Utils/bits_aliases.h"
#include "Kmplete/Utils/vector_utils.h"
#include "Kmplete/Profile/profiler.h"
#include "Kmplete/Log/log.h"


namespace Kmplete
{
    namespace Graphics
    {
        using namespace VKBits;


        VulkanGraphicsPipelineParameters VulkanGraphicsPipelineParameters::CopyFrom(const VulkanGraphicsPipelineParameters& source, int copyParametersMask /*= OnlyParameters*/) noexcept
        {
            VulkanGraphicsPipelineParameters copy{};

            copy._inputAssemblyCreateInfo = source._inputAssemblyCreateInfo;
            copy._rasterizationStateCreateInfo = source._rasterizationStateCreateInfo;
            copy._rasterizationLineStateCreateInfo = source._rasterizationLineStateCreateInfo;
            copy._colorBlendStateCreateInfo = source._colorBlendStateCreateInfo;
            copy._depthStencilStateCreateInfo = source._depthStencilStateCreateInfo;
            copy._multisamplingStateCreateInfo = source._multisamplingStateCreateInfo;
            copy._renderingCreateInfo = source._renderingCreateInfo;

            if (copyParametersMask & DescriptorSetLayouts)
            {
                copy._descriptorSetLayouts = source._descriptorSetLayouts;
            }
            if (copyParametersMask & ColorAttachmentInfos)
            {
                copy._renderingColorAttachmentsFormats = source._renderingColorAttachmentsFormats;
                copy._colorBlendAttachments = source._colorBlendAttachments;
            }
            if (copyParametersMask & DynamicStates)
            {
                copy._dynamicStates = source._dynamicStates;
            }
            if (copyParametersMask & VertexInputBindings)
            {
                copy._vertexInputBindings = source._vertexInputBindings;
            }
            if (copyParametersMask & VertexInputDescriptions)
            {
                copy._vertexAttributesDescriptions = source._vertexAttributesDescriptions;
            }
            if (copyParametersMask & ShaderStages)
            {
                copy._shadersStages = source._shadersStages;
            }

            return copy;
        }
        //--------------------------------------------------------------------------

        VulkanGraphicsPipelineParameters::VulkanGraphicsPipelineParameters()
            : _descriptorSetLayouts()
            , _inputAssemblyCreateInfo()
            , _rasterizationStateCreateInfo()
            , _rasterizationLineStateCreateInfo()
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

            _inputAssemblyCreateInfo = VKUtils::InitVkPipelineInputAssemblyStateCreateInfo();
            _rasterizationStateCreateInfo = VKUtils::InitVkPipelineRasterizationStateCreateInfo();
            _rasterizationLineStateCreateInfo = VKUtils::InitVkPipelineRasterizationLineStateCreateInfo();
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

        VulkanGraphicsPipelineParameters& VulkanGraphicsPipelineParameters::SetInputAssembly(VkPrimitiveTopology topology, bool primitiveRestartEnable)
        {
            _inputAssemblyCreateInfo.topology = topology;
            _inputAssemblyCreateInfo.primitiveRestartEnable = primitiveRestartEnable;
            return *this;
        }
        //--------------------------------------------------------------------------

        VulkanGraphicsPipelineParameters& VulkanGraphicsPipelineParameters::SetPolygonMode(VkPolygonMode polygonMode)
        {
            _rasterizationStateCreateInfo.polygonMode = polygonMode;
            return *this;
        }
        //--------------------------------------------------------------------------

        VulkanGraphicsPipelineParameters& VulkanGraphicsPipelineParameters::SetCulling(VkCullModeFlags cullMode, VkFrontFace frontFace)
        {
            _rasterizationStateCreateInfo.cullMode = cullMode;
            _rasterizationStateCreateInfo.frontFace = frontFace;
            return *this;
        }
        //--------------------------------------------------------------------------

        VulkanGraphicsPipelineParameters& VulkanGraphicsPipelineParameters::SetDepthClamping(bool enabled)
        {
            _rasterizationStateCreateInfo.depthClampEnable = enabled;
            return *this;
        }
        //--------------------------------------------------------------------------

        VulkanGraphicsPipelineParameters& VulkanGraphicsPipelineParameters::SetDepthBiasParameters(bool biasEnabled, float constantFactor, float clamp, float slopeFactor)
        {
            _rasterizationStateCreateInfo.depthBiasEnable = biasEnabled;
            _rasterizationStateCreateInfo.depthBiasConstantFactor = constantFactor;
            _rasterizationStateCreateInfo.depthBiasClamp = clamp;
            _rasterizationStateCreateInfo.depthBiasSlopeFactor = slopeFactor;
            return *this;
        }
        //--------------------------------------------------------------------------

        VulkanGraphicsPipelineParameters& VulkanGraphicsPipelineParameters::SetRasterizerDiscard(bool enabled)
        {
            _rasterizationStateCreateInfo.rasterizerDiscardEnable = enabled;
            return *this;
        }
        //--------------------------------------------------------------------------

        VulkanGraphicsPipelineParameters& VulkanGraphicsPipelineParameters::SetLineWidth(float lineWidth)
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

        VulkanGraphicsPipelineParameters& VulkanGraphicsPipelineParameters::SetColorBlendConstants(Array<float, 4> constants)
        {
            _colorBlendStateCreateInfo.blendConstants[0] = constants[0];
            _colorBlendStateCreateInfo.blendConstants[1] = constants[1];
            _colorBlendStateCreateInfo.blendConstants[2] = constants[2];
            _colorBlendStateCreateInfo.blendConstants[3] = constants[3];
            return *this;
        }
        //--------------------------------------------------------------------------

        VulkanGraphicsPipelineParameters& VulkanGraphicsPipelineParameters::SetDepthTest(bool enabled)
        {
            _depthStencilStateCreateInfo.depthTestEnable = enabled;
            return *this;
        }
        //--------------------------------------------------------------------------

        VulkanGraphicsPipelineParameters& VulkanGraphicsPipelineParameters::SetDepthWrite(bool enabled)
        {
            _depthStencilStateCreateInfo.depthWriteEnable = enabled;
            return *this;
        }
        //--------------------------------------------------------------------------

        VulkanGraphicsPipelineParameters& VulkanGraphicsPipelineParameters::SetDepthComparison(VkCompareOp compareOp)
        {
            _depthStencilStateCreateInfo.depthCompareOp = compareOp;
            return *this;
        }
        //--------------------------------------------------------------------------

        VulkanGraphicsPipelineParameters& VulkanGraphicsPipelineParameters::SetDepthBoundsTest(bool enabled, float min /*= 0.0f*/, float max /*= 1.0f*/)
        {
            _depthStencilStateCreateInfo.depthBoundsTestEnable = enabled;
            _depthStencilStateCreateInfo.minDepthBounds = min;
            _depthStencilStateCreateInfo.maxDepthBounds = max;
            return *this;
        }
        //--------------------------------------------------------------------------

        VulkanGraphicsPipelineParameters& VulkanGraphicsPipelineParameters::SetStencilTest(bool enabled)
        {
            _depthStencilStateCreateInfo.stencilTestEnable = enabled;
            return *this;
        }
        //--------------------------------------------------------------------------

        VulkanGraphicsPipelineParameters& VulkanGraphicsPipelineParameters::SetStencilStates(VkStencilOpState frontFaceState, VkStencilOpState backFaceState)
        {
            _depthStencilStateCreateInfo.front = frontFaceState;
            _depthStencilStateCreateInfo.back = backFaceState;
            return *this;
        }
        //--------------------------------------------------------------------------

        VulkanGraphicsPipelineParameters& VulkanGraphicsPipelineParameters::SetMultisamplingSamples(VkSampleCountFlagBits samples)
        {
            _multisamplingStateCreateInfo.rasterizationSamples = samples;
            return *this;
        }
        //--------------------------------------------------------------------------

        VulkanGraphicsPipelineParameters& VulkanGraphicsPipelineParameters::SetMultisamplingSampleShading(bool enabled, float minSampleShading)
        {
            _multisamplingStateCreateInfo.sampleShadingEnable = enabled;
            _multisamplingStateCreateInfo.minSampleShading = minSampleShading;
            return *this;
        }
        //--------------------------------------------------------------------------

        VulkanGraphicsPipelineParameters& VulkanGraphicsPipelineParameters::SetRenderingDepthStencilFormats(VkFormat depthFormat, VkFormat stencilFormat)
        {
            _renderingCreateInfo.depthAttachmentFormat = depthFormat;
            _renderingCreateInfo.stencilAttachmentFormat = stencilFormat;
            return *this;
        }
        //--------------------------------------------------------------------------

        VulkanGraphicsPipelineParameters& VulkanGraphicsPipelineParameters::SetLineRasterizationMode(VkLineRasterizationMode mode)
        {
            _rasterizationLineStateCreateInfo.lineRasterizationMode = mode;
            return *this;
        }
        //--------------------------------------------------------------------------

        VulkanGraphicsPipelineParameters& VulkanGraphicsPipelineParameters::AddDescriptorSetLayout(VkDescriptorSetLayout descriptorSetLayout)
        {
            _descriptorSetLayouts.push_back(descriptorSetLayout);
            return *this;
        }
        //--------------------------------------------------------------------------

        VulkanGraphicsPipelineParameters& VulkanGraphicsPipelineParameters::AddColorAttachmentInfo(VkFormat attachmentFormat, VkPipelineColorBlendAttachmentState colorBlendAttachment)
        {
            _renderingColorAttachmentsFormats.push_back(attachmentFormat);
            _colorBlendAttachments.push_back(colorBlendAttachment);
            return *this;
        }
        //--------------------------------------------------------------------------

        VulkanGraphicsPipelineParameters& VulkanGraphicsPipelineParameters::AddDynamicState(VkDynamicState dynamicState)
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

        VulkanGraphicsPipelineParameters& VulkanGraphicsPipelineParameters::AddDynamicStates(InitializerList<VkDynamicState> dynamicStates)
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

        VulkanGraphicsPipelineParameters& VulkanGraphicsPipelineParameters::AddVertexInputBindings(const Vector<VkVertexInputBindingDescription>& inputBindingDescriptions)
        {
            Utils::AppendVectors(inputBindingDescriptions, _vertexInputBindings);
            return *this;
        }
        //--------------------------------------------------------------------------

        VulkanGraphicsPipelineParameters& VulkanGraphicsPipelineParameters::AddVertexAttributesDescriptions(const Vector<VkVertexInputAttributeDescription>& attributesDescriptions)
        {
            Utils::AppendVectors(attributesDescriptions, _vertexAttributesDescriptions);
            return *this;
        }
        //--------------------------------------------------------------------------

        VulkanGraphicsPipelineParameters& VulkanGraphicsPipelineParameters::AddVertexBufferAttributesBindings(const VulkanVertexBuffer& vertexBuffer, UInt32 baseBinding)
        {
            auto [inputDescriptions, attributeDescriptions] = vertexBuffer.GetBindingsDescriptions(baseBinding);
            AddVertexInputBindings(inputDescriptions);
            AddVertexAttributesDescriptions(attributeDescriptions);
            return *this;
        }
        //--------------------------------------------------------------------------

        VulkanGraphicsPipelineParameters& VulkanGraphicsPipelineParameters::AddShaderStages(const Vector<VkPipelineShaderStageCreateInfo>& shaderStages)
        {
            Utils::AppendVectors(shaderStages, _shadersStages);
            return *this;
        }
        //--------------------------------------------------------------------------

        UInt32 VulkanGraphicsPipelineParameters::GetColorAttachmentsCount() const noexcept
        {
            return _renderingCreateInfo.colorAttachmentCount;
        }
        //--------------------------------------------------------------------------

        void VulkanGraphicsPipelineParameters::_SetDefaults() noexcept
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

            _rasterizationLineStateCreateInfo.lineRasterizationMode = VK_LineRasterization_Default;
            _rasterizationLineStateCreateInfo.stippledLineEnable = VK_FALSE;

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