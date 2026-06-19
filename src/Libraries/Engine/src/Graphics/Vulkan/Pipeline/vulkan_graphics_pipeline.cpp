#include "Kmplete/Graphics/Vulkan/Pipeline/vulkan_graphics_pipeline.h"
#include "Kmplete/Graphics/Vulkan/Utils/initializers.h"
#include "Kmplete/Graphics/Vulkan/Utils/result_description.h"
#include "Kmplete/Core/assertion.h"
#include "Kmplete/Profile/profiler.h"
#include "Kmplete/Log/log.h"


namespace Kmplete
{
    namespace Graphics
    {
        VulkanGraphicsPipeline::VulkanGraphicsPipeline(VkDevice device, StringID sid, VkPipelineLayout layout, VkPipelineCache cache, const VulkanGraphicsPipelineParameters& parameters)
            : KMP_PROFILE_CONSTRUCTOR_START_BASE_CLASS()
              _device(device)
            , _sid(sid)
            , _pipeline(VK_NULL_HANDLE)
            , _parameters(parameters)
        {
            _Initialize(layout, cache);

            KMP_PROFILE_CONSTRUCTOR_END()
        }
        //--------------------------------------------------------------------------

        VulkanGraphicsPipeline::~VulkanGraphicsPipeline() KMP_PROFILING(ProfileLevelAlways)
        {
            _Finalize();
        }}
        //--------------------------------------------------------------------------

        VkPipeline VulkanGraphicsPipeline::GetVkPipeline() const noexcept
        {
            KMP_ASSERT(_pipeline);

            return _pipeline;
        }
        //--------------------------------------------------------------------------

        UInt32 VulkanGraphicsPipeline::GetColorAttachmentsCount() const noexcept
        {
            return _parameters.GetColorAttachmentsCount();
        }
        //--------------------------------------------------------------------------

        void VulkanGraphicsPipeline::_Initialize(VkPipelineLayout layout, VkPipelineCache cache)
        {
            KMP_ASSERT(_device);

            _parameters._colorBlendStateCreateInfo.attachmentCount = UInt32(_parameters._colorBlendAttachments.size());
            _parameters._colorBlendStateCreateInfo.pAttachments = _parameters._colorBlendAttachments.data();
            _parameters._renderingCreateInfo.colorAttachmentCount = UInt32(_parameters._renderingColorAttachmentsFormats.size());
            _parameters._renderingCreateInfo.pColorAttachmentFormats = _parameters._renderingColorAttachmentsFormats.data();
            KMP_ASSERT(_parameters._renderingCreateInfo.colorAttachmentCount == _parameters._colorBlendStateCreateInfo.attachmentCount);

            _parameters._dynamicStateCreateInfo.dynamicStateCount = UInt32(_parameters._dynamicStates.size());
            _parameters._dynamicStateCreateInfo.pDynamicStates = _parameters._dynamicStates.data();

            _parameters._vertexInputStateCreateInfo.vertexBindingDescriptionCount = UInt32(_parameters._vertexInputBindings.size());
            _parameters._vertexInputStateCreateInfo.pVertexBindingDescriptions = _parameters._vertexInputBindings.data();
            _parameters._vertexInputStateCreateInfo.vertexAttributeDescriptionCount = UInt32(_parameters._vertexAttributesDescriptions.size());
            _parameters._vertexInputStateCreateInfo.pVertexAttributeDescriptions = _parameters._vertexAttributesDescriptions.data();

            _parameters._rasterizationStateCreateInfo.pNext = &_parameters._rasterizationLineStateCreateInfo;

            if (!_parameters._vertexInputBindingsDivisors.empty())
            {
                _parameters._vertexInputDivisorStateCreateInfo.vertexBindingDivisorCount = UInt32(_parameters._vertexInputBindingsDivisors.size());
                _parameters._vertexInputDivisorStateCreateInfo.pVertexBindingDivisors = _parameters._vertexInputBindingsDivisors.data();
                _parameters._vertexInputStateCreateInfo.pNext = &_parameters._vertexInputDivisorStateCreateInfo;
            }

            auto pipelineCI = Graphics::VKUtils::InitVkGraphicsPipelineCreateInfo();
            pipelineCI.layout = layout;
            pipelineCI.pInputAssemblyState = &_parameters._inputAssemblyCreateInfo;
            pipelineCI.pRasterizationState = &_parameters._rasterizationStateCreateInfo;
            pipelineCI.pColorBlendState = &_parameters._colorBlendStateCreateInfo;
            pipelineCI.pViewportState = &_parameters._viewportStateCreateInfo;
            pipelineCI.pDynamicState = &_parameters._dynamicStateCreateInfo;
            pipelineCI.pDepthStencilState = &_parameters._depthStencilStateCreateInfo;
            pipelineCI.pMultisampleState = &_parameters._multisamplingStateCreateInfo;
            pipelineCI.pVertexInputState = &_parameters._vertexInputStateCreateInfo;
            pipelineCI.stageCount = UInt32(_parameters._shadersStages.size());
            pipelineCI.pStages = _parameters._shadersStages.data();
            pipelineCI.pNext = &_parameters._renderingCreateInfo;

            const auto result = vkCreateGraphicsPipelines(_device, cache, 1, &pipelineCI, nullptr, &_pipeline);
            VKUtils::CheckResult(result, "VulkanGraphicsPipeline: failed to build graphics pipeline");
            KMP_ASSERT(_pipeline);
        }
        //--------------------------------------------------------------------------

        void VulkanGraphicsPipeline::_Finalize()
        {
            KMP_ASSERT(_device && _pipeline);

            vkDestroyPipeline(_device, _pipeline, nullptr);
        }
        //--------------------------------------------------------------------------
    }
}