#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"

#include <vulkan/vulkan.h>


namespace Kmplete
{
    namespace Graphics
    {
        namespace VulkanPresets
        {
            static constexpr VkPipelineColorBlendAttachmentState ColorBlendAttachmentState_NoBlend{
                .blendEnable = VK_FALSE,
                .colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT
            };
            //--------------------------------------------------------------------------

            static constexpr VkPipelineColorBlendAttachmentState ColorBlendAttachmentState_AlphaBlending{
                .blendEnable = VK_TRUE,
                .srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA,
                .dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
                .colorBlendOp = VK_BLEND_OP_ADD,
                .srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE,
                .dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,
                .alphaBlendOp = VK_BLEND_OP_ADD,
                .colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT
            };
            //--------------------------------------------------------------------------

            static constexpr VkColorBlendEquationEXT ColorBlendEquation_AlphaBlending{
                .srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA,
                .dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
                .colorBlendOp = VK_BLEND_OP_ADD,
                .srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE,
                .dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,
                .alphaBlendOp = VK_BLEND_OP_ADD,
            };
            //--------------------------------------------------------------------------

            static constexpr VkStencilOpState StencilOpState_Disabled{
                .failOp = VK_STENCIL_OP_KEEP,
                .passOp = VK_STENCIL_OP_KEEP,
                .compareOp = VK_COMPARE_OP_ALWAYS
            };
            //--------------------------------------------------------------------------

            static constexpr VkImageSubresourceRange ImageSubresourceRange_Color_Layer1_Level1{
                .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                .baseMipLevel = 0,
                .levelCount = 1,
                .baseArrayLayer = 0,
                .layerCount = 1
            };
            //--------------------------------------------------------------------------

            static constexpr VkImageSubresourceRange ImageSubresourceRange_DepthStencil_Layer1_Level1{ 
                .aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT,
                .baseMipLevel = 0,
                .levelCount = 1,
                .baseArrayLayer = 0,
                .layerCount = 1
            };
            //--------------------------------------------------------------------------

            static constexpr VkImageCreateInfo ImageCI_2D_OptimalTiling_QueueExclusive_Layer1_NoLayout{
                .sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
                .imageType = VK_IMAGE_TYPE_2D,
                .arrayLayers = 1,
                .tiling = VK_IMAGE_TILING_OPTIMAL,
                .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
                .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED
            };
            //--------------------------------------------------------------------------

            KMP_NODISCARD constexpr VkImageCreateInfo GetImageCI_2D_OptimalTiling_QueueExclusive_Layer1_NoLayout(VkFormat format, const VkExtent3D& extent, UInt32 mipLevels, VkSampleCountFlagBits samples, VkImageUsageFlags usageFlags)
            {
                auto imageCI = ImageCI_2D_OptimalTiling_QueueExclusive_Layer1_NoLayout;
                imageCI.format = format;
                imageCI.extent = extent;
                imageCI.mipLevels = mipLevels;
                imageCI.samples = samples;
                imageCI.usage = usageFlags;
                return imageCI;
            }
            //--------------------------------------------------------------------------

            static constexpr VkRenderingAttachmentInfo RenderingAttachmentInfo_Color_ClearStore{
                .sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO,
                .imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
                .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
                .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
                .clearValue = VkClearValue{ .color{ 0.0f, 0.0f, 0.0f, 0.0f }}
            };
            //--------------------------------------------------------------------------

            static constexpr VkRenderingAttachmentInfo RenderingAttachmentInfo_DepthStencil_ClearStore{
                .sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO,
                .imageLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
                .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
                .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
                .clearValue = VkClearValue{ .depthStencil{ 1.0f, 0 }}
            };
            //--------------------------------------------------------------------------

            static constexpr std::initializer_list<VkDynamicState> DynamicStates_Default{
                VK_DYNAMIC_STATE_VIEWPORT,
                VK_DYNAMIC_STATE_SCISSOR,
                VK_DYNAMIC_STATE_RASTERIZATION_SAMPLES_EXT
            };
            //--------------------------------------------------------------------------
        }
    }
}