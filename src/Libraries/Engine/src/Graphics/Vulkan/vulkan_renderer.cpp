#include "Kmplete/Graphics/Vulkan/vulkan_renderer.h"
#include "Kmplete/Graphics/Vulkan/Utils/function_utils.h"
#include "Kmplete/Graphics/Vulkan/Utils/extension_functions.h"
#include "Kmplete/Graphics/Vulkan/Utils/presets.h"
#include "Kmplete/Graphics/graphics_base.h"
#include "Kmplete/Profile/profiler.h"


namespace Kmplete
{
    namespace Graphics
    {
        VulkanRenderer::VulkanRenderer(VkDevice device, VkCommandPool commandPool, const UInt32& currentBufferIndex, const VkExtent2D& currentExtent)
            : _currentBufferIndex(currentBufferIndex)
            , _currentExtent(currentExtent)
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

            _drawCommandBuffers.reserve(NumConcurrentFrames);
            for (size_t i = 0; i < NumConcurrentFrames; i++)
            {
                _drawCommandBuffers.emplace_back(device, commandPool);
            }
        }
        //--------------------------------------------------------------------------

        VulkanRenderer::~VulkanRenderer()
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

            _drawCommandBuffers.clear();
        }
        //--------------------------------------------------------------------------

        void VulkanRenderer::StartFrame()
        {
            KMP_PROFILE_FUNCTION(ProfileLevelMinorFunctions);

            _drawCommandBuffers[_currentBufferIndex].Reset();
            _drawCommandBuffers[_currentBufferIndex].Begin();
        }
        //--------------------------------------------------------------------------

        void VulkanRenderer::BeginRendering(const VkRenderingInfo& renderingInfo)
        {
            KMP_PROFILE_FUNCTION(ProfileLevelMinorFunctions);

            vkCmdBeginRendering(GetCurrentCommandBuffer(), &renderingInfo);
        }
        //--------------------------------------------------------------------------

        void VulkanRenderer::EndRendering()
        {
            KMP_PROFILE_FUNCTION(ProfileLevelMinorFunctions);

            vkCmdEndRendering(GetCurrentCommandBuffer());
        }
        //--------------------------------------------------------------------------

        void VulkanRenderer::TransitionColorAndDepthStencilImagesToWrite(VkImage colorImage, VkImage depthStencilImage)
        {
            KMP_PROFILE_FUNCTION(ProfileLevelMinorFunctions);

            VulkanUtils::MemoryBarrierParameters colorImageBarrierParameters = {
                .cmdbuffer = GetCurrentCommandBuffer(),
                .image = colorImage,
                .srcAccessMask = VK_ACCESS_NONE,
                .dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
                .oldImageLayout = VK_IMAGE_LAYOUT_UNDEFINED,
                .newImageLayout = VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL,
                .srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
                .dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
                .subresourceRange = VulkanPresets::ImageSubresourceRange_Color_Layer1_Level1
            };
            VulkanUtils::InsertImageMemoryBarrier(colorImageBarrierParameters);

            VulkanUtils::MemoryBarrierParameters depthStencilImageBarrierParameters = {
                .cmdbuffer = GetCurrentCommandBuffer(),
                .image = depthStencilImage,
                .srcAccessMask = VK_ACCESS_NONE,
                .dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,
                .oldImageLayout = VK_IMAGE_LAYOUT_UNDEFINED,
                .newImageLayout = VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL,
                .srcStageMask = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT,
                .dstStageMask = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT,
                .subresourceRange = VulkanPresets::ImageSubresourceRange_DepthStencil_Layer1_Level1
            };
            VulkanUtils::InsertImageMemoryBarrier(depthStencilImageBarrierParameters);
        }
        //--------------------------------------------------------------------------

        void VulkanRenderer::SetViewportAndScissorFullExtent()
        {
            KMP_PROFILE_FUNCTION(ProfileLevelMinorFunctions);

            VkViewport viewport{ 0.0f, 0.0f, float(_currentExtent.width), float(_currentExtent.height) };
            vkCmdSetViewport(GetCurrentCommandBuffer(), 0, 1, &viewport);

            VkRect2D scissor{ 0, 0, _currentExtent.width, _currentExtent.height };
            vkCmdSetScissor(GetCurrentCommandBuffer(), 0, 1, &scissor);
        }
        //--------------------------------------------------------------------------

        void VulkanRenderer::SetRasterizationSamples(VkSampleCountFlagBits samples)
        {
            KMP_PROFILE_FUNCTION(ProfileLevelMinorFunctions);

            VulkanCommands::CmdSetRasterizationSamplesEXT(GetCurrentCommandBuffer(), samples);
        }
        //--------------------------------------------------------------------------

        void VulkanRenderer::TransitionColorAndDepthStencilImagesToPresent(VkImage colorImage)
        {
            KMP_PROFILE_FUNCTION(ProfileLevelMinorFunctions);

            VulkanUtils::MemoryBarrierParameters barrierParameters = {
                .cmdbuffer = GetCurrentCommandBuffer(),
                .image = colorImage,
                .srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
                .dstAccessMask = VK_ACCESS_NONE,
                .oldImageLayout = VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL,
                .newImageLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
                .srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
                .dstStageMask = VK_PIPELINE_STAGE_2_NONE,
                .subresourceRange = VulkanPresets::ImageSubresourceRange_Color_Layer1_Level1
            };
            VulkanUtils::InsertImageMemoryBarrier(barrierParameters);
        }
        //--------------------------------------------------------------------------

        void VulkanRenderer::EndFrame()
        {
            KMP_PROFILE_FUNCTION(ProfileLevelMinorFunctions);

            _drawCommandBuffers[_currentBufferIndex].End();
        }
        //--------------------------------------------------------------------------

        VkCommandBuffer VulkanRenderer::GetCurrentCommandBuffer() const noexcept
        {
            return _drawCommandBuffers.at(_currentBufferIndex).GetVkCommandBuffer();
        }
        //--------------------------------------------------------------------------
    }
}