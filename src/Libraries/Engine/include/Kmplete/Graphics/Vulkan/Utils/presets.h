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

            static constexpr VkStencilOpState StencilOpState_Disabled{
                .failOp = VK_STENCIL_OP_KEEP,
                .passOp = VK_STENCIL_OP_KEEP,
                .compareOp = VK_COMPARE_OP_NEVER
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

            static constexpr VkImageCreateInfo ImageCI_2D_OptimalTiling_Layer1{
                .sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
                .imageType = VK_IMAGE_TYPE_2D,
                .arrayLayers = 1,
                .tiling = VK_IMAGE_TILING_OPTIMAL
            };
            //--------------------------------------------------------------------------

            KMP_NODISCARD constexpr VkImageCreateInfo GetImageCI_2D_OptimalTiling_Layer1(const VkExtent3D& extent, UInt32 mipLevels, VkSampleCountFlagBits samples)
            {
                auto imageCI = ImageCI_2D_OptimalTiling_Layer1;
                imageCI.extent = extent;
                imageCI.mipLevels = mipLevels;
                imageCI.samples = samples;
                return imageCI;
            }
            //--------------------------------------------------------------------------
        }
    }
}