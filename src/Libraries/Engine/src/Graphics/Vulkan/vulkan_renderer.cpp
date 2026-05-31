#include "Kmplete/Graphics/Vulkan/vulkan_renderer.h"
#include "Kmplete/Graphics/Vulkan/Utils/extension_functions.h"
#include "Kmplete/Graphics/Vulkan/Utils/presets.h"
#include "Kmplete/Graphics/Vulkan/Utils/initializers.h"
#include "Kmplete/Graphics/graphics_base.h"
#include "Kmplete/Core/assertion.h"
#include "Kmplete/Profile/profiler.h"
#include "Kmplete/Log/log.h"


namespace Kmplete
{
    namespace Graphics
    {
        using namespace VKBits;


        VulkanRenderer::VulkanRenderer(VkDevice device, const UInt32& currentBufferIndex, const VulkanPipelineManager& pipelineManager,
                                       const StringIDHashMap<UPtr<VulkanShaderObject>>& shaderObjects, UInt32 graphicsFamilyIndex, const VulkanSwapchain& swapchain)
            : _currentBufferIndex(currentBufferIndex)
            , _pipelineManager(pipelineManager)
            , _shaderObjects(shaderObjects)
            , _swapchain(std::cref(swapchain))
            , _device(device)
            , _commandPool(nullptr)
            , _drawCommandBuffers()
            , _currentCommandBuffer(VK_NULL_HANDLE)
        {
            KMP_PROFILE_FUNCTION(ProfileLevelAlways);

            KMP_ASSERT(_device);

            _commandPool.reset(new VulkanCommandPool(device, graphicsFamilyIndex));
            KMP_ASSERT(_commandPool);

            _drawCommandBuffers.reserve(NumConcurrentFrames);
            for (size_t i = 0; i < NumConcurrentFrames; i++)
            {
                _drawCommandBuffers.emplace_back(device, _commandPool->GetVkCommandPool());
            }

            _currentCommandBuffer = _drawCommandBuffers[_currentBufferIndex].GetVkCommandBuffer();
            KMP_ASSERT(_currentCommandBuffer);
        }
        //--------------------------------------------------------------------------

        VulkanRenderer::~VulkanRenderer() KMP_PROFILING(ProfileLevelAlways)
        {
            KMP_ASSERT(_commandPool && !_drawCommandBuffers.empty());

            _drawCommandBuffers.clear();
            _commandPool.reset();
        }}
        //--------------------------------------------------------------------------

        void VulkanRenderer::StartFrame() KMP_PROFILING(ProfileLevelMinor)
        {
            KMP_ASSERT(_currentBufferIndex < _drawCommandBuffers.size());

            _drawCommandBuffers[_currentBufferIndex].Reset();
            _drawCommandBuffers[_currentBufferIndex].Begin();
            _currentCommandBuffer = _drawCommandBuffers[_currentBufferIndex].GetVkCommandBuffer();
            KMP_ASSERT(_currentCommandBuffer);
        }}
        //--------------------------------------------------------------------------

        void VulkanRenderer::EndFrame() KMP_PROFILING(ProfileLevelMinor)
        {
            KMP_ASSERT(_currentBufferIndex < _drawCommandBuffers.size());

            _drawCommandBuffers[_currentBufferIndex].End();
        }}
        //--------------------------------------------------------------------------

        void VulkanRenderer::SetSwapchain(const VulkanSwapchain& swapchain)
        {
            _swapchain = std::cref(swapchain);
        }
        //--------------------------------------------------------------------------

        void VulkanRenderer::BeginRendering(const VkRect2D& renderArea, bool clearPrevious /*= true*/) const KMP_PROFILING(ProfileLevelMinor)
        {
            KMP_ASSERT(_currentCommandBuffer);

            const auto& swapchain = _swapchain.get();

            const auto colorAttachmentInfo = swapchain.GetRenderingColorAttachmentInfo(clearPrevious);
            const auto depthStencilAttachmentInfo = swapchain.GetRenderingDepthStencilAttachmentInfo(clearPrevious);

            auto renderingInfo = VKUtils::InitVkRenderingInfo();
            renderingInfo.renderArea = renderArea;
            renderingInfo.layerCount = 1;
            renderingInfo.colorAttachmentCount = 1;
            renderingInfo.pColorAttachments = &colorAttachmentInfo;
            renderingInfo.pDepthAttachment = &depthStencilAttachmentInfo;
            renderingInfo.pStencilAttachment = &depthStencilAttachmentInfo;

            vkCmdBeginRendering(_currentCommandBuffer, &renderingInfo);
        }}
        //--------------------------------------------------------------------------

        void VulkanRenderer::BeginRendering(StringID pipelineSid, const VkRect2D& renderArea, bool clearPrevious /*= true*/) const KMP_PROFILING(ProfileLevelMinor)
        {
            KMP_ASSERT(_currentCommandBuffer);

            const auto pipeline = _pipelineManager.GetGraphicsPipeline(pipelineSid);
            if (!pipeline.has_value())
            {
                KMP_LOG_ERROR("cannot begin rendering - pipeline with sid '{}' not found", pipelineSid);
                return;
            }

            const auto colorAttachmentsCount = pipeline.value().get().GetColorAttachmentsCount();
            const auto& swapchain = _swapchain.get();

            Vector<VkRenderingAttachmentInfo> colorAttachmentsInfos;
            colorAttachmentsInfos.reserve(colorAttachmentsCount);
            for (UInt32 i = 0; i < colorAttachmentsCount; i++)
            {
                colorAttachmentsInfos.push_back(swapchain.GetRenderingColorAttachmentInfo(clearPrevious));
            }

            const auto depthStencilAttachmentInfo = swapchain.GetRenderingDepthStencilAttachmentInfo(clearPrevious);

            auto renderingInfo = VKUtils::InitVkRenderingInfo();
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
            KMP_ASSERT(_currentCommandBuffer);

            vkCmdEndRendering(_currentCommandBuffer);
        }}
        //--------------------------------------------------------------------------

        void VulkanRenderer::SubmitToQueue(const VulkanQueue& queue, const Vector<VkSemaphore>& waitSemaphores, const Vector<VkSemaphore>& signalSemaphores, VkFence fence) KMP_PROFILING(ProfileLevelMinor)
        {
            KMP_ASSERT(_currentCommandBuffer);

            VkPipelineStageFlags waitStageMask = VK_PipelineStage_ColorAttachmentOutput;
            auto submitInfo = VKUtils::InitVkSubmitInfo();
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

        void VulkanRenderer::TransitionImage(VkImage image, VKUtils::MemoryBarrierParameters& memoryBarrierParameters) const KMP_PROFILING(ProfileLevelMinor)
        {
            KMP_ASSERT(_currentCommandBuffer);

            memoryBarrierParameters.cmdbuffer = _currentCommandBuffer;
            memoryBarrierParameters.image = image;
            VKUtils::InsertImageMemoryBarrier(memoryBarrierParameters);
        }}
        //--------------------------------------------------------------------------

        void VulkanRenderer::SetDepthTestEnabled(bool enabled) const KMP_PROFILING(ProfileLevelMinor)
        {
            KMP_ASSERT(_currentCommandBuffer);

            vkCmdSetDepthTestEnable(_currentCommandBuffer, enabled);
        }}
        //--------------------------------------------------------------------------

        void VulkanRenderer::SetDepthWriteEnabled(bool enabled) const KMP_PROFILING(ProfileLevelMinor)
        {
            KMP_ASSERT(_currentCommandBuffer);

            vkCmdSetDepthWriteEnable(_currentCommandBuffer, enabled);
        }}
        //--------------------------------------------------------------------------

        void VulkanRenderer::SetDepthCompareOp(VkCompareOp comparison) const KMP_PROFILING(ProfileLevelMinor)
        {
            KMP_ASSERT(_currentCommandBuffer);

            vkCmdSetDepthCompareOp(_currentCommandBuffer, comparison);
        }}
        //--------------------------------------------------------------------------

        void VulkanRenderer::SetDepthBiasEnabled(bool enabled) const KMP_PROFILING(ProfileLevelMinor)
        {
            KMP_ASSERT(_currentCommandBuffer);

            vkCmdSetDepthBiasEnable(_currentCommandBuffer, enabled);
        }}
        //--------------------------------------------------------------------------

        void VulkanRenderer::SetDepthBias(float constantFactor, float clamp, float slopeFactor) const KMP_PROFILING(ProfileLevelMinor)
        {
            KMP_ASSERT(_currentCommandBuffer);

            vkCmdSetDepthBias(_currentCommandBuffer, constantFactor, clamp, slopeFactor);
        }}
        //--------------------------------------------------------------------------

        void VulkanRenderer::SetDepthBoundsEnabled(bool enabled) const KMP_PROFILING(ProfileLevelMinor)
        {
            KMP_ASSERT(_currentCommandBuffer);

            vkCmdSetDepthBoundsTestEnable(_currentCommandBuffer, enabled);
        }}
        //--------------------------------------------------------------------------

        void VulkanRenderer::SetDepthBounds(float min, float max) const KMP_PROFILING(ProfileLevelMinor)
        {
            KMP_ASSERT(_currentCommandBuffer);

            vkCmdSetDepthBounds(_currentCommandBuffer, min, max);
        }}
        //--------------------------------------------------------------------------

        void VulkanRenderer::SetDepthClipEnabled(bool enabled) const KMP_PROFILING(ProfileLevelMinor)
        {
            KMP_ASSERT(_currentCommandBuffer);

            VKCommands::CmdSetDepthClipEnableEXT(_currentCommandBuffer, enabled);
        }}
        //--------------------------------------------------------------------------

        void VulkanRenderer::SetStencilTestEnabled(bool enabled) const KMP_PROFILING(ProfileLevelMinor)
        {
            KMP_ASSERT(_currentCommandBuffer);

            vkCmdSetStencilTestEnable(_currentCommandBuffer, enabled);
        }}
        //--------------------------------------------------------------------------

        void VulkanRenderer::SetStencilOp(VkStencilFaceFlags faceMask, VkStencilOp failOp, VkStencilOp passOp, VkStencilOp depthFailOp, VkCompareOp comparison) const KMP_PROFILING(ProfileLevelMinor)
        {
            KMP_ASSERT(_currentCommandBuffer);

            vkCmdSetStencilOp(_currentCommandBuffer, faceMask, failOp, passOp, depthFailOp, comparison);
        }}
        //--------------------------------------------------------------------------

        void VulkanRenderer::SetStencilCompareMask(VkStencilFaceFlags faceMask, UInt32 compareMask) const KMP_PROFILING(ProfileLevelMinor)
        {
            KMP_ASSERT(_currentCommandBuffer);

            vkCmdSetStencilCompareMask(_currentCommandBuffer, faceMask, compareMask);
        }}
        //--------------------------------------------------------------------------

        void VulkanRenderer::SetStencilWriteMask(VkStencilFaceFlags faceMask, UInt32 writeMask) const KMP_PROFILING(ProfileLevelMinor)
        {
            KMP_ASSERT(_currentCommandBuffer);

            vkCmdSetStencilWriteMask(_currentCommandBuffer, faceMask, writeMask);
        }}
        //--------------------------------------------------------------------------

        void VulkanRenderer::SetStencilReference(VkStencilFaceFlags faceMask, UInt32 reference) const KMP_PROFILING(ProfileLevelMinor)
        {
            KMP_ASSERT(_currentCommandBuffer);

            vkCmdSetStencilReference(_currentCommandBuffer, faceMask, reference);
        }}
        //--------------------------------------------------------------------------

        void VulkanRenderer::SetViewport(const VkViewport& viewport) const KMP_PROFILING(ProfileLevelMinor)
        {
            KMP_ASSERT(_currentCommandBuffer);

            vkCmdSetViewport(_currentCommandBuffer, 0, 1, &viewport);
        }}
        //--------------------------------------------------------------------------

        void VulkanRenderer::SetScissor(const VkRect2D& scissorRect) const KMP_PROFILING(ProfileLevelMinor)
        {
            KMP_ASSERT(_currentCommandBuffer);

            vkCmdSetScissor(_currentCommandBuffer, 0, 1, &scissorRect);
        }}
        //--------------------------------------------------------------------------

        void VulkanRenderer::SetViewportWithCount(const Vector<VkViewport>& viewports) const KMP_PROFILING(ProfileLevelMinor)
        {
            KMP_ASSERT(_currentCommandBuffer);

            vkCmdSetViewportWithCount(_currentCommandBuffer, UInt32(viewports.size()), viewports.data());
        }}
        //--------------------------------------------------------------------------

        void VulkanRenderer::SetScissorWithCount(const Vector<VkRect2D>& scissors) const KMP_PROFILING(ProfileLevelMinor)
        {
            KMP_ASSERT(_currentCommandBuffer);

            vkCmdSetScissorWithCount(_currentCommandBuffer, UInt32(scissors.size()), scissors.data());
        }}
        //--------------------------------------------------------------------------

        void VulkanRenderer::SetRasterizationSamples(VkSampleCountFlagBits samples) const KMP_PROFILING(ProfileLevelMinor)
        {
            KMP_ASSERT(_currentCommandBuffer);

            VKCommands::CmdSetRasterizationSamplesEXT(_currentCommandBuffer, samples);
        }}
        //--------------------------------------------------------------------------

        void VulkanRenderer::SetPrimitiveTopology(VkPrimitiveTopology topology) const KMP_PROFILING(ProfileLevelMinor)
        {
            KMP_ASSERT(_currentCommandBuffer);

            vkCmdSetPrimitiveTopology(_currentCommandBuffer, topology);
        }}
        //--------------------------------------------------------------------------

        void VulkanRenderer::SetPrimitiveRestartEnabled(bool enabled) const KMP_PROFILING(ProfileLevelMinor)
        {
            KMP_ASSERT(_currentCommandBuffer);

            vkCmdSetPrimitiveRestartEnable(_currentCommandBuffer, enabled);
        }}
        //--------------------------------------------------------------------------

        void VulkanRenderer::SetLineWidth(float lineWidth) const KMP_PROFILING(ProfileLevelMinor)
        {
            KMP_ASSERT(_currentCommandBuffer);

            vkCmdSetLineWidth(_currentCommandBuffer, lineWidth);
        }}
        //--------------------------------------------------------------------------

        void VulkanRenderer::SetLineStippleEnabled(bool enabled) const KMP_PROFILING(ProfileLevelMinor)
        {
            KMP_ASSERT(_currentCommandBuffer);

            VKCommands::CmdSetLineStippleEnableEXT(_currentCommandBuffer, enabled);
        }}
        //--------------------------------------------------------------------------

        void VulkanRenderer::SetLineStipple(UInt32 lineStippleFactor, UInt16 lineStipplePattern) const KMP_PROFILING(ProfileLevelMinor)
        {
            KMP_ASSERT(_currentCommandBuffer);

            vkCmdSetLineStipple(_currentCommandBuffer, lineStippleFactor, lineStipplePattern);
        }}
        //--------------------------------------------------------------------------

        void VulkanRenderer::SetLineRasterizationMode(VkLineRasterizationModeEXT lineRasterizationMode) const KMP_PROFILING(ProfileLevelMinor)
        {
            KMP_ASSERT(_currentCommandBuffer);

            VKCommands::CmdSetLineRasterizationModeEXT(_currentCommandBuffer, lineRasterizationMode);
        }}
        //--------------------------------------------------------------------------

        void VulkanRenderer::SetCullMode(VkCullModeFlags cullMode) const KMP_PROFILING(ProfileLevelMinor)
        {
            KMP_ASSERT(_currentCommandBuffer);

            vkCmdSetCullMode(_currentCommandBuffer, cullMode);
        }}
        //--------------------------------------------------------------------------

        void VulkanRenderer::SetFrontFace(VkFrontFace frontFace) const KMP_PROFILING(ProfileLevelMinor)
        {
            KMP_ASSERT(_currentCommandBuffer);

            vkCmdSetFrontFace(_currentCommandBuffer, frontFace);
        }}
        //--------------------------------------------------------------------------

        void VulkanRenderer::SetBlendConstants(const Array<float, 4> constants) const KMP_PROFILING(ProfileLevelMinor)
        {
            KMP_ASSERT(_currentCommandBuffer);

            vkCmdSetBlendConstants(_currentCommandBuffer, constants.data());
        }}
        //--------------------------------------------------------------------------

        void VulkanRenderer::SetRasterizerDiscardEnabled(bool enabled) const KMP_PROFILING(ProfileLevelMinor)
        {
            KMP_ASSERT(_currentCommandBuffer);

            vkCmdSetRasterizerDiscardEnable(_currentCommandBuffer, enabled);
        }}
        //--------------------------------------------------------------------------

        void VulkanRenderer::SetSampleLocationsEnabled(bool enabled) const KMP_PROFILING(ProfileLevelMinor)
        {
            KMP_ASSERT(_currentCommandBuffer);

            VKCommands::CmdSetSampleLocationsEnableEXT(_currentCommandBuffer, enabled);
        }}
        //--------------------------------------------------------------------------

        void VulkanRenderer::SetSampleLocations(const Vector<VkSampleLocationsInfoEXT>& sampleLocationInfos) const KMP_PROFILING(ProfileLevelMinor)
        {
            KMP_ASSERT(_currentCommandBuffer);

            VKCommands::CmdSetSampleLocationsEXT(_currentCommandBuffer, sampleLocationInfos.data());
        }}
        //--------------------------------------------------------------------------

        void VulkanRenderer::SetSampleMask(VkSampleCountFlagBits samples, const Vector<VkSampleMask>& sampleMasks) const KMP_PROFILING(ProfileLevelMinor)
        {
            KMP_ASSERT(_currentCommandBuffer);

            VKCommands::CmdSetSampleMaskEXT(_currentCommandBuffer, samples, sampleMasks.data());
        }}
        //--------------------------------------------------------------------------

        void VulkanRenderer::SetFragmentShadingRate(const VkExtent2D& fragmentSize, const Array<VkFragmentShadingRateCombinerOpKHR, 2>& combinerOps) const KMP_PROFILING(ProfileLevelMinor)
        {
            KMP_ASSERT(_currentCommandBuffer);

            VKCommands::CmdSetFragmentShadingRateKHR(_currentCommandBuffer, &fragmentSize, combinerOps.data());
        }}
        //--------------------------------------------------------------------------

        void VulkanRenderer::SetColorWriteEnabled(UInt32 attachmentCount, const Vector<VkBool32>& colorWritesEnables) const KMP_PROFILING(ProfileLevelMinor)
        {
            KMP_ASSERT(_currentCommandBuffer);

            VKCommands::CmdSetColorWriteEnableEXT(_currentCommandBuffer, attachmentCount, colorWritesEnables.data());
        }}
        //--------------------------------------------------------------------------

        void VulkanRenderer::SetColorWriteMask(UInt32 firstAttachment, UInt32 attachmentCount, const Vector<VkColorComponentFlags>& colorWritesMasks) const KMP_PROFILING(ProfileLevelMinor)
        {
            KMP_ASSERT(_currentCommandBuffer);

            VKCommands::CmdSetColorWriteMaskEXT(_currentCommandBuffer, firstAttachment, attachmentCount, colorWritesMasks.data());
        }}
        //--------------------------------------------------------------------------

        void VulkanRenderer::SetColorBlendEnabled(UInt32 firstAttachment, UInt32 attachmentCount, const Vector<VkBool32>& colorBlendsEnables) const KMP_PROFILING(ProfileLevelMinor)
        {
            KMP_ASSERT(_currentCommandBuffer);

            VKCommands::CmdSetColorBlendEnableEXT(_currentCommandBuffer, firstAttachment, attachmentCount, colorBlendsEnables.data());
        }}
        //--------------------------------------------------------------------------

        void VulkanRenderer::SetColorBlendEquation(UInt32 firstAttachment, UInt32 attachmentCount, const Vector<VkColorBlendEquationEXT>& colorBlendsEquations) const KMP_PROFILING(ProfileLevelMinor)
        {
            KMP_ASSERT(_currentCommandBuffer);

            VKCommands::CmdSetColorBlendEquationEXT(_currentCommandBuffer, firstAttachment, attachmentCount, colorBlendsEquations.data());
        }}
        //--------------------------------------------------------------------------

        void VulkanRenderer::SetPolygonMode(VkPolygonMode polygonMode) const KMP_PROFILING(ProfileLevelMinor)
        {
            KMP_ASSERT(_currentCommandBuffer);

            VKCommands::CmdSetPolygonModeEXT(_currentCommandBuffer, polygonMode);
        }}
        //--------------------------------------------------------------------------

        void VulkanRenderer::SetAlphaToCoverageEnabled(bool enabled) const KMP_PROFILING(ProfileLevelMinor)
        {
            KMP_ASSERT(_currentCommandBuffer);

            VKCommands::CmdSetAlphaToCoverageEnableEXT(_currentCommandBuffer, enabled);
        }}
        //--------------------------------------------------------------------------

        void VulkanRenderer::SetAlphaToOneEnabled(bool enabled) const KMP_PROFILING(ProfileLevelMinor)
        {
            KMP_ASSERT(_currentCommandBuffer);

            VKCommands::CmdSetAlphaToOneEnableEXT(_currentCommandBuffer, enabled);
        }}
        //--------------------------------------------------------------------------

        void VulkanRenderer::SetLogicOpEnabled(bool enabled) const KMP_PROFILING(ProfileLevelMinor)
        {
            KMP_ASSERT(_currentCommandBuffer);

            VKCommands::CmdSetLogicOpEnableEXT(_currentCommandBuffer, enabled);
        }}
        //--------------------------------------------------------------------------

        void VulkanRenderer::SetLogicOp(VkLogicOp logicOp) const KMP_PROFILING(ProfileLevelMinor)
        {
            KMP_ASSERT(_currentCommandBuffer);

            VKCommands::CmdSetLogicOpEXT(_currentCommandBuffer, logicOp);
        }}
        //--------------------------------------------------------------------------

        void VulkanRenderer::SetProvokingVertexMode(VkProvokingVertexModeEXT mode) const KMP_PROFILING(ProfileLevelMinor)
        {
            KMP_ASSERT(_currentCommandBuffer);

            VKCommands::CmdSetProvokingVertexModeEXT(_currentCommandBuffer, mode);
        }}
        //--------------------------------------------------------------------------

        void VulkanRenderer::SetVertexInput(const Vector<VkVertexInputBindingDescription2EXT>& vertexBindingsDescriptions, const Vector<VkVertexInputAttributeDescription2EXT>& vertexAttributeDescriptions) const KMP_PROFILING(ProfileLevelMinor)
        {
            KMP_ASSERT(_currentCommandBuffer);

            VKCommands::CmdSetVertexInputEXT(_currentCommandBuffer, UInt32(vertexBindingsDescriptions.size()), vertexBindingsDescriptions.data(), UInt32(vertexAttributeDescriptions.size()), vertexAttributeDescriptions.data());
        }}
        //--------------------------------------------------------------------------

        bool VulkanRenderer::BindGraphicsPipeline(StringID pipelineSid) const KMP_PROFILING(ProfileLevelImportant)
        {
            KMP_ASSERT(_currentCommandBuffer);

            const auto pipeline = _pipelineManager.GetGraphicsPipeline(pipelineSid);
            if (!pipeline.has_value())
            {
                KMP_LOG_ERROR("cannot bind pipeline with sid '{}' - pipeline not found", pipelineSid);
                return false;
            }

            vkCmdBindPipeline(_currentCommandBuffer, VK_PipelineBindPoint_Graphics, pipeline.value().get().GetVkPipeline());
            return true;
        }}
        //--------------------------------------------------------------------------

        bool VulkanRenderer::BindDescriptorSets(StringID layoutSid, UInt32 firstSetIndex, const Vector<VkDescriptorSet>& descriptorSets, const Vector<UInt32>& dynamicOffsets /*= Vector<UInt32>()*/) const KMP_PROFILING(ProfileLevelImportant)
        {
            KMP_ASSERT(_currentCommandBuffer);

            const auto pipelineLayout = _pipelineManager.GetPipelineLayout(layoutSid);
            if (pipelineLayout == VK_NULL_HANDLE)
            {
                KMP_LOG_ERROR("cannot bind descriptor sets with pipeline layout sid '{}' - pipeline layout not found", layoutSid);
                return false;
            }

            vkCmdBindDescriptorSets(
                _currentCommandBuffer, 
                VK_PipelineBindPoint_Graphics,
                pipelineLayout,
                firstSetIndex, 
                UInt32(descriptorSets.size()), descriptorSets.empty() ? nullptr : descriptorSets.data(),
                UInt32(dynamicOffsets.size()), dynamicOffsets.empty() ? nullptr : dynamicOffsets.data());

            return true;
        }}
        //--------------------------------------------------------------------------

        void VulkanRenderer::PushConstants(StringID layoutSid, VkShaderStageFlags shaderStagesFlags, UInt32 offset, UInt32 size, const void* data) const KMP_PROFILING(ProfileLevelImportantVerbose)
        {
            KMP_ASSERT(_currentCommandBuffer);

            const auto pipelineLayout = _pipelineManager.GetPipelineLayout(layoutSid);
            if (pipelineLayout == VK_NULL_HANDLE)
            {
                KMP_LOG_ERROR("cannot push constants with pipeline layout sid '{}' - pipeline layout not found", layoutSid);
                return;
            }

            vkCmdPushConstants(_currentCommandBuffer, pipelineLayout, shaderStagesFlags, offset, size, data);
        }}
        //--------------------------------------------------------------------------

        bool VulkanRenderer::BindVertexBuffers(UInt32 firstBinding, const Vector<VkBuffer>& vertexBuffers, const Vector<VkDeviceSize>& offsets) const KMP_PROFILING(ProfileLevelImportantVerbose)
        {
            KMP_ASSERT(_currentCommandBuffer);

            if (vertexBuffers.size() != offsets.size() || vertexBuffers.empty())
            {
                KMP_LOG_ERROR("failed to bind vertex buffers - vertex buffers size doesn't match size of the offsets or vertex buffers are empty");
                return false;
            }

            vkCmdBindVertexBuffers(_currentCommandBuffer, firstBinding, UInt32(vertexBuffers.size()), vertexBuffers.data(), offsets.data());
            return true;
        }}
        //--------------------------------------------------------------------------

        void VulkanRenderer::BindVertexBuffers2(UInt32 firstBinding, const Vector<VkBuffer>& buffers, const Vector<VkDeviceSize>& offsets, const Vector<VkDeviceSize>& sizes, const Vector<VkDeviceSize>& strides) const KMP_PROFILING(ProfileLevelMinor)
        {
            KMP_ASSERT(_currentCommandBuffer);

            if (buffers.size() != offsets.size() ||
                ((buffers.size() != sizes.size()) && !sizes.empty()) ||
                ((buffers.size() != strides.size()) && !strides.empty()) ||
                buffers.empty())
            {
                KMP_LOG_ERROR("failed to bind vertex buffers - vertex buffers size doesn't match size of the offsets/sizes/strides or vertex buffers are empty");
                return;
            }

            vkCmdBindVertexBuffers2(_currentCommandBuffer, firstBinding, UInt32(buffers.size()), buffers.data(), offsets.data(), sizes.data(), strides.data());
        }}
        //--------------------------------------------------------------------------

        void VulkanRenderer::BindIndexBuffer(const VulkanBuffer& indexBuffer, VkDeviceSize offset /*= 0*/, VkIndexType indexType /*= VK_Index_UInt32*/) const
        {
            BindIndexBuffer(indexBuffer.GetVkBuffer(), offset, indexType);
        }
        //--------------------------------------------------------------------------

        void VulkanRenderer::BindIndexBuffer(VkBuffer indexBuffer, VkDeviceSize offset /*= 0*/, VkIndexType indexType /*= VK_Index_UInt32*/) const KMP_PROFILING(ProfileLevelImportantVerbose)
        {
            KMP_ASSERT(_currentCommandBuffer);

            vkCmdBindIndexBuffer(_currentCommandBuffer, indexBuffer, offset, indexType);
        }}
        //--------------------------------------------------------------------------

        void VulkanRenderer::BindShaderObjects(const Vector<VkShaderStageFlagBits>& stages, const Vector<StringID>& shadersSids) const KMP_PROFILING(ProfileLevelMinor)
        {
            KMP_ASSERT(_currentCommandBuffer);

            Vector<VkShaderEXT> shaders;
            shaders.reserve(shadersSids.size());
            for (const auto& shaderSid : shadersSids)
            {
                if (_shaderObjects.contains(shaderSid))
                {
                    shaders.push_back(_shaderObjects.at(shaderSid)->GetVkShader());
                    continue;
                }

                KMP_LOG_ERROR("cannot bind shader with sid '{}' - not found", shaderSid);
            }

            VKCommands::CmdBindShadersEXT(_currentCommandBuffer, UInt32(stages.size()), stages.data(), shaders.data());
        }}
        //--------------------------------------------------------------------------

        void VulkanRenderer::Draw(UInt32 vertexCount, UInt32 instanceCount, UInt32 firstVertex, UInt32 firstInstance) const KMP_PROFILING(ProfileLevelImportantVerbose)
        {
            KMP_ASSERT(_currentCommandBuffer);

            vkCmdDraw(_currentCommandBuffer, vertexCount, instanceCount, firstVertex, firstInstance);
        }}
        //--------------------------------------------------------------------------

        void VulkanRenderer::DrawIndexed(UInt32 indexCount, UInt32 instanceCount, UInt32 firstIndex, Int32 vertexOffset, UInt32 firstInstance) const KMP_PROFILING(ProfileLevelImportantVerbose)
        {
            KMP_ASSERT(_currentCommandBuffer);

            vkCmdDrawIndexed(_currentCommandBuffer, indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
        }}
        //--------------------------------------------------------------------------

        void VulkanRenderer::DrawIndirect(const VulkanBuffer& indirectBuffer, VkDeviceSize offset, UInt32 drawCount, UInt32 stride /*= sizeof(VkDrawIndirectCommand)*/) const
        {
            DrawIndirect(indirectBuffer.GetVkBuffer(), offset, drawCount, stride);
        }
        //--------------------------------------------------------------------------

        void VulkanRenderer::DrawIndirect(VkBuffer indirectBuffer, VkDeviceSize offset, UInt32 drawCount, UInt32 stride /*= sizeof(VkDrawIndirectCommand)*/) const KMP_PROFILING(ProfileLevelImportantVerbose)
        {
            KMP_ASSERT(_currentCommandBuffer);

            vkCmdDrawIndirect(_currentCommandBuffer, indirectBuffer, offset, drawCount, stride);
        }}
        //--------------------------------------------------------------------------

        void VulkanRenderer::DrawIndexedIndirect(const VulkanBuffer& indirectBuffer, VkDeviceSize offset, UInt32 drawCount, UInt32 stride /*= sizeof(VkDrawIndexedIndirectCommand)*/) const
        {
            DrawIndexedIndirect(indirectBuffer.GetVkBuffer(), offset, drawCount, stride);
        }
        //--------------------------------------------------------------------------

        void VulkanRenderer::DrawIndexedIndirect(VkBuffer indirectBuffer, VkDeviceSize offset, UInt32 drawCount, UInt32 stride /*= sizeof(VkDrawIndexedIndirectCommand)*/) const KMP_PROFILING(ProfileLevelImportantVerbose)
        {
            KMP_ASSERT(_currentCommandBuffer);

            vkCmdDrawIndexedIndirect(_currentCommandBuffer, indirectBuffer, offset, drawCount, stride);
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

        void VulkanRenderer::CopyBuffers(const VulkanBuffer& stagingBuffer, const Vector<VKUtils::BufferCopyParameters>& copyParameters, const VulkanQueue& queue) const KMP_PROFILING(ProfileLevelImportantVerbose)
        {
            const auto copyCommandBuffer = CreateCommandBuffer();
            copyCommandBuffer.Begin();
            for (const auto& singleCopyParameters : copyParameters)
            {
                CopyBuffer(copyCommandBuffer, stagingBuffer, singleCopyParameters.destinationBuffer, singleCopyParameters.srcOfset, singleCopyParameters.dstOfset, singleCopyParameters.size);
            }
            copyCommandBuffer.End();

            queue.SyncSubmit(copyCommandBuffer);
        }}
        //--------------------------------------------------------------------------

        VulkanCommandBuffer VulkanRenderer::CreateCommandBuffer() const KMP_PROFILING(ProfileLevelImportant)
        {
            KMP_ASSERT(_device && _commandPool);

            return VulkanCommandBuffer(_device, _commandPool->GetVkCommandPool());
        }}
        //--------------------------------------------------------------------------

        VkCommandBuffer VulkanRenderer::GetCurrentCommandBuffer() const noexcept
        {
            KMP_ASSERT(_currentCommandBuffer);

            return _currentCommandBuffer;
        }
        //--------------------------------------------------------------------------
    }
}