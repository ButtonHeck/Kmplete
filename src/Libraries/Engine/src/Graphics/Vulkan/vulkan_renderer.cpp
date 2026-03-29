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
        VulkanRenderer::VulkanRenderer(VkDevice device, VkCommandPool commandPool, const UInt32& currentBufferIndex)
            : _currentBufferIndex(currentBufferIndex)
            , _currentCommandBuffer(VK_NULL_HANDLE)
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
            _currentCommandBuffer = _drawCommandBuffers[_currentBufferIndex].GetVkCommandBuffer();
        }
        //--------------------------------------------------------------------------

        void VulkanRenderer::EndFrame()
        {
            KMP_PROFILE_FUNCTION(ProfileLevelMinorFunctions);

            _drawCommandBuffers[_currentBufferIndex].End();
        }
        //--------------------------------------------------------------------------

        void VulkanRenderer::BeginRendering(const VkRenderingInfo& renderingInfo) const
        {
            KMP_PROFILE_FUNCTION(ProfileLevelMinorFunctions);

            vkCmdBeginRendering(_currentCommandBuffer, &renderingInfo);
        }
        //--------------------------------------------------------------------------

        void VulkanRenderer::EndRendering() const
        {
            KMP_PROFILE_FUNCTION(ProfileLevelMinorFunctions);

            vkCmdEndRendering(_currentCommandBuffer);
        }
        //--------------------------------------------------------------------------

        void VulkanRenderer::TransitionColorAndDepthStencilImagesToWrite(VkImage colorImage, VkImage depthStencilImage) const
        {
            KMP_PROFILE_FUNCTION(ProfileLevelMinorFunctions);

            VulkanUtils::MemoryBarrierParameters colorImageBarrierParameters = {
                .cmdbuffer = _currentCommandBuffer,
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
                .cmdbuffer = _currentCommandBuffer,
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

        void VulkanRenderer::TransitionColorAndDepthStencilImagesToPresent(VkImage colorImage) const
        {
            KMP_PROFILE_FUNCTION(ProfileLevelMinorFunctions);

            VulkanUtils::MemoryBarrierParameters barrierParameters = {
                .cmdbuffer = _currentCommandBuffer,
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

        void VulkanRenderer::SetViewport(const VkViewport& viewport) const
        {
            KMP_PROFILE_FUNCTION(ProfileLevelMinorFunctions);

            vkCmdSetViewport(_currentCommandBuffer, 0, 1, &viewport);
        }
        //--------------------------------------------------------------------------

        void VulkanRenderer::SetScissor(const VkRect2D& scissorRect) const
        {
            KMP_PROFILE_FUNCTION(ProfileLevelMinorFunctions);

            vkCmdSetScissor(_currentCommandBuffer, 0, 1, &scissorRect);
        }
        //--------------------------------------------------------------------------

        void VulkanRenderer::SetRasterizationSamples(VkSampleCountFlagBits samples) const
        {
            KMP_PROFILE_FUNCTION(ProfileLevelMinorFunctions);

            VulkanCommands::CmdSetRasterizationSamplesEXT(_currentCommandBuffer, samples);
        }
        //--------------------------------------------------------------------------

        VkCommandBuffer VulkanRenderer::GetCurrentCommandBuffer() const noexcept
        {
            return _currentCommandBuffer;
        }
        //--------------------------------------------------------------------------
    }
}