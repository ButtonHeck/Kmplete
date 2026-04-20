#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Graphics/Vulkan/Utils/bits_aliases.h"

#include <vulkan/vulkan.h>


namespace Kmplete
{
    namespace Graphics
    {
        using namespace VKBits;


        namespace VKPresets
        {
            static constexpr VkPipelineColorBlendAttachmentState ColorBlendAttachmentState_NoBlend{
                .blendEnable = VK_FALSE,
                .colorWriteMask = VK_Color_RGBA
            };
            //--------------------------------------------------------------------------

            static constexpr VkPipelineColorBlendAttachmentState ColorBlendAttachmentState_AlphaBlending{
                .blendEnable = VK_TRUE,
                .srcColorBlendFactor = VK_BlendFactor_SrcAlpha,
                .dstColorBlendFactor = VK_BlendFactor_OneMinusSrcAlpha,
                .colorBlendOp = VK_Blend_Add,
                .srcAlphaBlendFactor = VK_BlendFactor_One,
                .dstAlphaBlendFactor = VK_BlendFactor_Zero,
                .alphaBlendOp = VK_Blend_Add,
                .colorWriteMask = VK_Color_RGBA
            };
            //--------------------------------------------------------------------------

            static constexpr VkColorBlendEquationEXT ColorBlendEquation_AlphaBlending{
                .srcColorBlendFactor = VK_BlendFactor_SrcAlpha,
                .dstColorBlendFactor = VK_BlendFactor_OneMinusSrcAlpha,
                .colorBlendOp = VK_Blend_Add,
                .srcAlphaBlendFactor = VK_BlendFactor_One,
                .dstAlphaBlendFactor = VK_BlendFactor_Zero,
                .alphaBlendOp = VK_Blend_Add,
            };
            //--------------------------------------------------------------------------

            static constexpr VkStencilOpState StencilOpState_Disabled{
                .failOp = VK_Stencil_Keep,
                .passOp = VK_Stencil_Keep,
                .compareOp = VK_Compare_Always
            };
            //--------------------------------------------------------------------------

            static constexpr VkImageSubresourceRange ImageSubresourceRange_Color_Layer1_Level1{
                .aspectMask = VK_ImageAspect_Color,
                .baseMipLevel = 0,
                .levelCount = 1,
                .baseArrayLayer = 0,
                .layerCount = 1
            };
            //--------------------------------------------------------------------------

            static constexpr VkImageSubresourceRange ImageSubresourceRange_DepthStencil_Layer1_Level1{ 
                .aspectMask = VK_ImageAspect_DepthStencil,
                .baseMipLevel = 0,
                .levelCount = 1,
                .baseArrayLayer = 0,
                .layerCount = 1
            };
            //--------------------------------------------------------------------------

            static constexpr VkImageCreateInfo ImageCI_2D_OptimalTiling_QueueExclusive_Layer1_NoLayout{
                .sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
                .imageType = VK_Image_2D,
                .arrayLayers = 1,
                .tiling = VK_ImageTiling_Optimal,
                .sharingMode = VK_Sharing_Exclusive,
                .initialLayout = VK_ImageLayout_Undefined
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
                .imageLayout = VK_ImageLayout_ColorAttachmentOptimal,
                .loadOp = VK_AttachmentLoad_Clear,
                .storeOp = VK_AttachmentStore_Store,
                .clearValue = VkClearValue{ .color{ 0.0f, 0.0f, 0.0f, 0.0f }}
            };
            //--------------------------------------------------------------------------

            static constexpr VkRenderingAttachmentInfo RenderingAttachmentInfo_DepthStencil_ClearStore{
                .sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO,
                .imageLayout = VK_ImageLayout_DepthStencilAttachmentOptimal,
                .loadOp = VK_AttachmentLoad_Clear,
                .storeOp = VK_AttachmentStore_Store,
                .clearValue = VkClearValue{ .depthStencil{ 1.0f, 0 }}
            };
            //--------------------------------------------------------------------------

            static constexpr VkSamplerCreateInfo SamplerCreateInfo_Nearest_MipNearest_Repeat_NoAnisotropy{
                .sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
                .magFilter = VK_Filter_Nearest,
                .minFilter = VK_Filter_Nearest,
                .mipmapMode = VK_SamplerMipmap_Nearest,
                .addressModeU = VK_SamplerAddress_Repeat,
                .addressModeV = VK_SamplerAddress_Repeat,
                .addressModeW = VK_SamplerAddress_Repeat,
                .anisotropyEnable = VK_FALSE,
                .minLod = 0.0f,
                .maxLod = VK_LOD_CLAMP_NONE
            };
            //--------------------------------------------------------------------------

            static constexpr VkSamplerCreateInfo SamplerCreateInfo_Linear_MipLinear_Repeat_NoAnisotropy{
                .sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
                .magFilter = VK_Filter_Linear,
                .minFilter = VK_Filter_Linear,
                .mipmapMode = VK_SamplerMipmap_Linear,
                .addressModeU = VK_SamplerAddress_Repeat,
                .addressModeV = VK_SamplerAddress_Repeat,
                .addressModeW = VK_SamplerAddress_Repeat,
                .anisotropyEnable = VK_FALSE,
                .minLod = 0.0f,
                .maxLod = VK_LOD_CLAMP_NONE
            };
            //--------------------------------------------------------------------------
        }
    }
}