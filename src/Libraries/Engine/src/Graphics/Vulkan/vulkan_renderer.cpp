#include "Kmplete/Graphics/Vulkan/vulkan_renderer.h"
#include "Kmplete/Graphics/Vulkan/Utils/function_utils.h"
#include "Kmplete/Graphics/Vulkan/Utils/extension_functions.h"
#include "Kmplete/Graphics/Vulkan/Utils/presets.h"
#include "Kmplete/Graphics/Vulkan/Utils/initializers.h"
#include "Kmplete/Graphics/graphics_base.h"
#include "Kmplete/Profile/profiler.h"
#include "Kmplete/Log/log.h"


namespace Kmplete
{
    namespace Graphics
    {
        VulkanRenderer::VulkanRenderer(VkDevice device, const UInt32& currentBufferIndex, const HashMap<StringID, UPtr<VulkanGraphicsPipeline>>& pipelines, UInt32 graphicsFamilyIndex, const VulkanSwapchain& swapchain)
            : _currentBufferIndex(currentBufferIndex)
            , _pipelines(pipelines)
            , _swapchain(std::cref(swapchain))
            , _device(device)
            , _commandPool(nullptr)
            , _drawCommandBuffers()
            , _currentCommandBuffer(VK_NULL_HANDLE)
        {
            KMP_PROFILE_FUNCTION(ProfileLevelAlways);

            _commandPool.reset(new VulkanCommandPool(device, graphicsFamilyIndex));

            _drawCommandBuffers.reserve(NumConcurrentFrames);
            for (size_t i = 0; i < NumConcurrentFrames; i++)
            {
                _drawCommandBuffers.emplace_back(device, _commandPool->GetVkCommandPool());
            }
        }
        //--------------------------------------------------------------------------

        VulkanRenderer::~VulkanRenderer() KMP_PROFILING(ProfileLevelAlways)
        {
            _drawCommandBuffers.clear();
            _commandPool.reset();
        }}
        //--------------------------------------------------------------------------

        void VulkanRenderer::StartFrame() KMP_PROFILING(ProfileLevelMinor)
        {
            _drawCommandBuffers[_currentBufferIndex].Reset();
            _drawCommandBuffers[_currentBufferIndex].Begin();
            _currentCommandBuffer = _drawCommandBuffers[_currentBufferIndex].GetVkCommandBuffer();
        }}
        //--------------------------------------------------------------------------

        void VulkanRenderer::EndFrame() KMP_PROFILING(ProfileLevelMinor)
        {
            _drawCommandBuffers[_currentBufferIndex].End();
        }}
        //--------------------------------------------------------------------------

        void VulkanRenderer::SetSwapchain(const VulkanSwapchain& swapchain)
        {
            _swapchain = std::cref(swapchain);
        }
        //--------------------------------------------------------------------------

        void VulkanRenderer::BeginRendering(const VkRect2D& renderArea) const KMP_PROFILING(ProfileLevelMinor)
        {
            const auto& swapchain = _swapchain.get();

            const auto colorAttachmentInfo = swapchain.GetRenderingColorAttachmentInfo();
            const auto depthStencilAttachmentInfo = swapchain.GetRenderingDepthStencilAttachmentInfo();

            auto renderingInfo = VulkanUtils::InitVkRenderingInfo();
            renderingInfo.renderArea = renderArea;
            renderingInfo.layerCount = 1;
            renderingInfo.colorAttachmentCount = 1;
            renderingInfo.pColorAttachments = &colorAttachmentInfo;
            renderingInfo.pDepthAttachment = &depthStencilAttachmentInfo;
            renderingInfo.pStencilAttachment = &depthStencilAttachmentInfo;

            vkCmdBeginRendering(_currentCommandBuffer, &renderingInfo);
        }}
        //--------------------------------------------------------------------------

        void VulkanRenderer::BeginRendering(StringID pipelineSid, const VkRect2D& renderArea) const KMP_PROFILING(ProfileLevelMinor)
        {
            if (!_pipelines.contains(pipelineSid))
            {
                KMP_LOG_ERROR("cannot begin rendering - pipeline with sid '{}' not found", pipelineSid);
                return;
            }

            const auto colorAttachmentsCount = _pipelines.at(pipelineSid)->GetColorAttachmentsCount();
            const auto& swapchain = _swapchain.get();

            Vector<VkRenderingAttachmentInfo> colorAttachmentsInfos;
            colorAttachmentsInfos.reserve(colorAttachmentsCount);
            for (UInt32 i = 0; i < colorAttachmentsCount; i++)
            {
                colorAttachmentsInfos.push_back(swapchain.GetRenderingColorAttachmentInfo());
            }

            const auto depthStencilAttachmentInfo = swapchain.GetRenderingDepthStencilAttachmentInfo();

            auto renderingInfo = VulkanUtils::InitVkRenderingInfo();
            renderingInfo.renderArea = renderArea;
            renderingInfo.layerCount = 1;
            renderingInfo.colorAttachmentCount = colorAttachmentsCount;
            renderingInfo.pColorAttachments = colorAttachmentsInfos.data();
            renderingInfo.pDepthAttachment = &depthStencilAttachmentInfo;
            renderingInfo.pStencilAttachment = &depthStencilAttachmentInfo;

            vkCmdBeginRendering(_currentCommandBuffer, &renderingInfo);
        }}
        //--------------------------------------------------------------------------

        void VulkanRenderer::EndRendering() const KMP_PROFILING(ProfileLevelMinor)
        {
            vkCmdEndRendering(_currentCommandBuffer);
        }}
        //--------------------------------------------------------------------------

        void VulkanRenderer::TransitionColorAndDepthStencilImagesToWrite(VkImage colorImage, VkImage depthStencilImage) const KMP_PROFILING(ProfileLevelMinor)
        {
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
        }}
        //--------------------------------------------------------------------------

        void VulkanRenderer::TransitionColorAndDepthStencilImagesToPresent(VkImage colorImage) const KMP_PROFILING(ProfileLevelMinor)
        {
            VulkanUtils::MemoryBarrierParameters barrierParameters = {
                .cmdbuffer = _currentCommandBuffer,
                .image = colorImage,
                .srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
                .dstAccessMask = VK_ACCESS_NONE,
                .oldImageLayout = VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL,
                .newImageLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
                .srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
                .dstStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
                .subresourceRange = VulkanPresets::ImageSubresourceRange_Color_Layer1_Level1
            };
            VulkanUtils::InsertImageMemoryBarrier(barrierParameters);
        }}
        //--------------------------------------------------------------------------

        void VulkanRenderer::SetViewport(const VkViewport& viewport) const KMP_PROFILING(ProfileLevelMinor)
        {
            vkCmdSetViewport(_currentCommandBuffer, 0, 1, &viewport);
        }}
        //--------------------------------------------------------------------------

        void VulkanRenderer::SetScissor(const VkRect2D& scissorRect) const KMP_PROFILING(ProfileLevelMinor)
        {
            vkCmdSetScissor(_currentCommandBuffer, 0, 1, &scissorRect);
        }}
        //--------------------------------------------------------------------------

        void VulkanRenderer::SetRasterizationSamples(VkSampleCountFlagBits samples) const KMP_PROFILING(ProfileLevelMinor)
        {
            VulkanCommands::CmdSetRasterizationSamplesEXT(_currentCommandBuffer, samples);
        }}
        //--------------------------------------------------------------------------

        void VulkanRenderer::SetDepthBiasEnabled(bool enabled) const KMP_PROFILING(ProfileLevelMinor)
        {
            vkCmdSetDepthBiasEnable(_currentCommandBuffer, enabled);
        }}
        //--------------------------------------------------------------------------

        void VulkanRenderer::SetDepthBias(float constantFactor, float clamp, float slopeFactor) const KMP_PROFILING(ProfileLevelMinor)
        {
            vkCmdSetDepthBias(_currentCommandBuffer, constantFactor, clamp, slopeFactor);
        }}
        //--------------------------------------------------------------------------

        void VulkanRenderer::SetDepthBoundsEnabled(bool enabled) const KMP_PROFILING(ProfileLevelMinor)
        {
            vkCmdSetDepthBoundsTestEnable(_currentCommandBuffer, enabled);
        }}
        //--------------------------------------------------------------------------

        void VulkanRenderer::SetDepthBounds(float min, float max) const KMP_PROFILING(ProfileLevelMinor)
        {
            vkCmdSetDepthBounds(_currentCommandBuffer, min, max);
        }}
        //--------------------------------------------------------------------------

        void VulkanRenderer::SetPrimitiveTopology(VkPrimitiveTopology topology) const KMP_PROFILING(ProfileLevelMinor)
        {
            vkCmdSetPrimitiveTopology(_currentCommandBuffer, topology);
        }}
        //--------------------------------------------------------------------------

        void VulkanRenderer::SetLineWidth(float lineWidth) const KMP_PROFILING(ProfileLevelMinor)
        {
            vkCmdSetLineWidth(_currentCommandBuffer, lineWidth);
        }}
        //--------------------------------------------------------------------------

        void VulkanRenderer::SetCullMode(VkCullModeFlags cullMode) const KMP_PROFILING(ProfileLevelMinor)
        {
            vkCmdSetCullMode(_currentCommandBuffer, cullMode);
        }}
        //--------------------------------------------------------------------------

        void VulkanRenderer::SetFrontFace(VkFrontFace frontFace) const KMP_PROFILING(ProfileLevelMinor)
        {
            vkCmdSetFrontFace(_currentCommandBuffer, frontFace);
        }}
        //--------------------------------------------------------------------------

        void VulkanRenderer::SetStencilCompareMask(VkStencilFaceFlags faceMask, UInt32 compareMask) const KMP_PROFILING(ProfileLevelMinor)
        {
            vkCmdSetStencilCompareMask(_currentCommandBuffer, faceMask, compareMask);
        }}
        //--------------------------------------------------------------------------

        void VulkanRenderer::SetStencilWriteMask(VkStencilFaceFlags faceMask, UInt32 writeMask) const KMP_PROFILING(ProfileLevelMinor)
        {
            vkCmdSetStencilWriteMask(_currentCommandBuffer, faceMask, writeMask);
        }}
        //--------------------------------------------------------------------------

        void VulkanRenderer::SetStencilReference(VkStencilFaceFlags faceMask, UInt32 reference) const KMP_PROFILING(ProfileLevelMinor)
        {
            vkCmdSetStencilReference(_currentCommandBuffer, faceMask, reference);
        }}
        //--------------------------------------------------------------------------

        void VulkanRenderer::SetBlendConstants(const Array<float, 4> constants) const KMP_PROFILING(ProfileLevelMinor)
        {
            vkCmdSetBlendConstants(_currentCommandBuffer, constants.data());
        }}
        //--------------------------------------------------------------------------

        void VulkanRenderer::SubmitToQueue(const VulkanQueue& queue, const Vector<VkSemaphore>& waitSemaphores, const Vector<VkSemaphore>& signalSemaphores, VkFence fence) KMP_PROFILING(ProfileLevelMinor)
        {
            VkPipelineStageFlags waitStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
            auto submitInfo = VulkanUtils::InitVkSubmitInfo();
            submitInfo.pWaitDstStageMask = &waitStageMask;
            submitInfo.commandBufferCount = 1;
            submitInfo.pCommandBuffers = &_currentCommandBuffer;
            submitInfo.waitSemaphoreCount = UInt32(waitSemaphores.size());
            submitInfo.pWaitSemaphores = waitSemaphores.data();
            submitInfo.signalSemaphoreCount = UInt32(signalSemaphores.size());
            submitInfo.pSignalSemaphores = signalSemaphores.data();

            queue.Submit({ submitInfo }, fence);
        }}
        //--------------------------------------------------------------------------

        bool VulkanRenderer::BindGraphicsPipeline(StringID pipelineSid) const KMP_PROFILING(ProfileLevelImportant)
        {
            if (!_pipelines.contains(pipelineSid))
            {
                KMP_LOG_ERROR("cannot bind pipeline with sid '{}' - pipeline not found", pipelineSid);
                return false;
            }

            vkCmdBindPipeline(_currentCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, _pipelines.at(pipelineSid)->GetVkPipeline());
            return true;
        }}
        //--------------------------------------------------------------------------

        bool VulkanRenderer::BindDescriptorSets(StringID pipelineSid, UInt32 firstSetIndex, const Vector<VkDescriptorSet>& descriptorSets, const Vector<UInt32>& dynamicOffsets /*= Vector<UInt32>()*/) const KMP_PROFILING(ProfileLevelImportant)
        {
            if (!_pipelines.contains(pipelineSid))
            {
                KMP_LOG_ERROR("cannot bind descriptor sets with pipeline's sid '{}' - pipeline not found", pipelineSid);
                return false;
            }

            vkCmdBindDescriptorSets(
                _currentCommandBuffer, 
                VK_PIPELINE_BIND_POINT_GRAPHICS, 
                _pipelines.at(pipelineSid)->GetVkPipelineLayout(),
                firstSetIndex, 
                UInt32(descriptorSets.size()), descriptorSets.empty() ? nullptr : descriptorSets.data(),
                UInt32(dynamicOffsets.size()), dynamicOffsets.empty() ? nullptr : dynamicOffsets.data());

            return true;
        }}
        //--------------------------------------------------------------------------

        bool VulkanRenderer::BindVertexBuffers(UInt32 firstBinding, const Vector<VkBuffer>& vertexBuffers, const Vector<VkDeviceSize>& offsets) const KMP_PROFILING(ProfileLevelImportantVerbose)
        {
            if (vertexBuffers.size() != offsets.size() || vertexBuffers.empty())
            {
                KMP_LOG_ERROR("failed to bind vertex buffers - vertex buffers size doesn't match size of the offsets or vertex buffers are empty");
                return false;
            }

            vkCmdBindVertexBuffers(_currentCommandBuffer, firstBinding, UInt32(vertexBuffers.size()), vertexBuffers.data(), offsets.data());
            return true;
        }}
        //--------------------------------------------------------------------------

        void VulkanRenderer::BindIndexBuffer(VkBuffer indexBuffer, VkDeviceSize offset /*= 0*/, VkIndexType indexType /*= VK_INDEX_TYPE_UINT32*/) const KMP_PROFILING(ProfileLevelImportantVerbose)
        {
            vkCmdBindIndexBuffer(_currentCommandBuffer, indexBuffer, offset, indexType);
        }}
        //--------------------------------------------------------------------------

        void VulkanRenderer::DrawIndexed(UInt32 indexCount, UInt32 instanceCount, UInt32 firstIndex, Int32 vertexOffset, UInt32 firstInstance) const KMP_PROFILING(ProfileLevelImportantVerbose)
        {
            vkCmdDrawIndexed(_currentCommandBuffer, indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
        }}
        //--------------------------------------------------------------------------

        void VulkanRenderer::Draw(UInt32 vertexCount, UInt32 instanceCount, UInt32 firstVertex, UInt32 firstInstance) const KMP_PROFILING(ProfileLevelImportantVerbose)
        {
            vkCmdDraw(_currentCommandBuffer, vertexCount, instanceCount, firstVertex, firstInstance);
        }}
        //--------------------------------------------------------------------------

        void VulkanRenderer::CopyBuffer(const VulkanCommandBuffer& commandBuffer, const VulkanBuffer& sourceBuffer, const VulkanBuffer& destinationBuffer, VkDeviceSize srcOffset, VkDeviceSize dstOffset, VkDeviceSize size) const KMP_PROFILING(ProfileLevelImportantVerbose)
        {
            VkBufferCopy copyRegion{
                .srcOffset = srcOffset,
                .dstOffset = dstOffset,
                .size = size
            };
            vkCmdCopyBuffer(commandBuffer.GetVkCommandBuffer(), sourceBuffer.GetVkBuffer(), destinationBuffer.GetVkBuffer(), 1, &copyRegion);
        }}
        //--------------------------------------------------------------------------

        void VulkanRenderer::CopyBuffer(const VulkanCommandBuffer& commandBuffer, const VulkanBuffer& sourceBuffer, const VulkanBuffer& destinationBuffer, const VkBufferCopy& copyRegion) const KMP_PROFILING(ProfileLevelImportantVerbose)
        {
            vkCmdCopyBuffer(commandBuffer.GetVkCommandBuffer(), sourceBuffer.GetVkBuffer(), destinationBuffer.GetVkBuffer(), 1, &copyRegion);
        }}
        //--------------------------------------------------------------------------

        void VulkanRenderer::CopyBuffer(const VulkanCommandBuffer& commandBuffer, const VulkanBuffer& sourceBuffer, const VulkanBuffer& destinationBuffer, const Vector<VkBufferCopy>& copyRegions) const KMP_PROFILING(ProfileLevelImportantVerbose)
        {
            vkCmdCopyBuffer(commandBuffer.GetVkCommandBuffer(), sourceBuffer.GetVkBuffer(), destinationBuffer.GetVkBuffer(), UInt32(copyRegions.size()), copyRegions.data());
        }}
        //--------------------------------------------------------------------------

        VulkanCommandBuffer VulkanRenderer::CreateCommandBuffer() const KMP_PROFILING(ProfileLevelImportant)
        {
            return VulkanCommandBuffer(_device, _commandPool->GetVkCommandPool());
        }}
        //--------------------------------------------------------------------------

        VkCommandBuffer VulkanRenderer::GetCurrentCommandBuffer() const noexcept
        {
            return _currentCommandBuffer;
        }
        //--------------------------------------------------------------------------
    }
}