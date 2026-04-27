#include "Kmplete/Graphics/Vulkan/vulkan_renderer.h"
#include "Kmplete/Graphics/Vulkan/Utils/function_utils.h"
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


        VulkanRenderer::VulkanRenderer(VkDevice device, const UInt32& currentBufferIndex, const StringIDHashMap<UPtr<VulkanGraphicsPipeline>>& pipelines,
                                       const StringIDHashMap<UPtr<VulkanShaderObject>>& shaderObjects, UInt32 graphicsFamilyIndex, const VulkanSwapchain& swapchain)
            : _currentBufferIndex(currentBufferIndex)
            , _pipelines(pipelines)
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

        void VulkanRenderer::BeginRendering(const VkRect2D& renderArea) const KMP_PROFILING(ProfileLevelMinor)
        {
            KMP_ASSERT(_currentCommandBuffer);

            const auto& swapchain = _swapchain.get();

            const auto colorAttachmentInfo = swapchain.GetRenderingColorAttachmentInfo();
            const auto depthStencilAttachmentInfo = swapchain.GetRenderingDepthStencilAttachmentInfo();

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

        void VulkanRenderer::BeginRendering(StringID pipelineSid, const VkRect2D& renderArea) const KMP_PROFILING(ProfileLevelMinor)
        {
            KMP_ASSERT(_currentCommandBuffer);

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

        void VulkanRenderer::TransitionColorAndDepthStencilImagesToWrite(VkImage colorImage, VkImage depthStencilImage) const KMP_PROFILING(ProfileLevelMinor)
        {
            KMP_ASSERT(_currentCommandBuffer);

            VKUtils::MemoryBarrierParameters colorImageBarrierParameters = {
                .cmdbuffer = _currentCommandBuffer,
                .image = colorImage,
                .srcAccessMask = VK_Access_None,
                .dstAccessMask = VK_Access_ColorAttachmentWrite,
                .oldImageLayout = VK_ImageLayout_Undefined,
                .newImageLayout = VK_ImageLayout_AttachmentOptimal,
                .srcStageMask = VK_PipelineStage_ColorAttachmentOutput,
                .dstStageMask = VK_PipelineStage_ColorAttachmentOutput,
                .subresourceRange = VKPresets::ImageSubresourceRange_Color_Layer1_Level1
            };
            VKUtils::InsertImageMemoryBarrier(colorImageBarrierParameters);

            VKUtils::MemoryBarrierParameters depthStencilImageBarrierParameters = {
                .cmdbuffer = _currentCommandBuffer,
                .image = depthStencilImage,
                .srcAccessMask = VK_Access_None,
                .dstAccessMask = VK_Access_DepthStencilAttachmentWrite,
                .oldImageLayout = VK_ImageLayout_Undefined,
                .newImageLayout = VK_ImageLayout_AttachmentOptimal,
                .srcStageMask = VK_PipelineStage_EarlyAndLateFragmentTests,
                .dstStageMask = VK_PipelineStage_EarlyAndLateFragmentTests,
                .subresourceRange = VKPresets::ImageSubresourceRange_DepthStencil_Layer1_Level1
            };
            VKUtils::InsertImageMemoryBarrier(depthStencilImageBarrierParameters);
        }}
        //--------------------------------------------------------------------------

        void VulkanRenderer::TransitionColorAndDepthStencilImagesToPresent(VkImage colorImage) const KMP_PROFILING(ProfileLevelMinor)
        {
            KMP_ASSERT(_currentCommandBuffer);

            VKUtils::MemoryBarrierParameters barrierParameters = {
                .cmdbuffer = _currentCommandBuffer,
                .image = colorImage,
                .srcAccessMask = VK_Access_ColorAttachmentWrite,
                .dstAccessMask = VK_Access_None,
                .oldImageLayout = VK_ImageLayout_AttachmentOptimal,
                .newImageLayout = VK_ImageLayout_PresentKHR,
                .srcStageMask = VK_PipelineStage_ColorAttachmentOutput,
                .dstStageMask = VK_PipelineStage_BottomOfPipe,
                .subresourceRange = VKPresets::ImageSubresourceRange_Color_Layer1_Level1
            };
            VKUtils::InsertImageMemoryBarrier(barrierParameters);
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

        void VulkanRenderer::SetDepthClampEnabled(bool enabled) const KMP_PROFILING(ProfileLevelMinor)
        {
            KMP_ASSERT(_currentCommandBuffer);

            VKCommands::CmdSetDepthClampEnableEXT(_currentCommandBuffer, enabled);
        }}
        //--------------------------------------------------------------------------

        void VulkanRenderer::SetDepthClampRange(VkDepthClampModeEXT depthClampMode, float min, float max) const KMP_PROFILING(ProfileLevelMinor)
        {
            KMP_ASSERT(_currentCommandBuffer);

            VkDepthClampRangeEXT depthClampRange{ .minDepthClamp = min, .maxDepthClamp = max };
            VKCommands::CmdSetDepthClampRangeEXT(_currentCommandBuffer, depthClampMode, &depthClampRange);
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

        void VulkanRenderer::SetDiscardRectangleEnabled(bool enabled) const KMP_PROFILING(ProfileLevelMinor)
        {
            KMP_ASSERT(_currentCommandBuffer);

            VKCommands::CmdSetDiscardRectangleEnableEXT(_currentCommandBuffer, enabled);
        }}
        //--------------------------------------------------------------------------

        void VulkanRenderer::SetDiscardRectangle(UInt32 firstDiscardRectangle, UInt32 count, const Vector<VkRect2D>& discardRectangles) const KMP_PROFILING(ProfileLevelMinor)
        {
            KMP_ASSERT(_currentCommandBuffer);

            VKCommands::CmdSetDiscardRectangleEXT(_currentCommandBuffer, firstDiscardRectangle, count, discardRectangles.data());
        }}
        //--------------------------------------------------------------------------

        void VulkanRenderer::SetDiscardRectangleMode(VkDiscardRectangleModeEXT mode) const KMP_PROFILING(ProfileLevelMinor)
        {
            KMP_ASSERT(_currentCommandBuffer);

            VKCommands::CmdSetDiscardRectangleModeEXT(_currentCommandBuffer, mode);
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

        void VulkanRenderer::BindVertexBuffers2(UInt32 firstBinding, const Vector<VkBuffer>& buffers, const Vector<VkDeviceSize>& offsets, const Vector<VkDeviceSize>& sizes, const Vector<VkDeviceSize>& strides) const KMP_PROFILING(ProfileLevelMinor)
        {
            KMP_ASSERT(_currentCommandBuffer);

            if (buffers.size() != offsets.size() || 
                ((buffers.size() != sizes.size()) && !sizes.empty() ) || 
                ((buffers.size() != strides.size()) && !strides.empty() ) || 
                buffers.empty())
            {
                KMP_LOG_ERROR("failed to bind vertex buffers - vertex buffers size doesn't match size of the offsets/sizes/strides or vertex buffers are empty");
                return;
            }

            vkCmdBindVertexBuffers2(_currentCommandBuffer, firstBinding, UInt32(buffers.size()), buffers.data(), offsets.data(), sizes.data(), strides.data());
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

        bool VulkanRenderer::BindGraphicsPipeline(StringID pipelineSid) const KMP_PROFILING(ProfileLevelImportant)
        {
            KMP_ASSERT(_currentCommandBuffer);

            if (!_pipelines.contains(pipelineSid))
            {
                KMP_LOG_ERROR("cannot bind pipeline with sid '{}' - pipeline not found", pipelineSid);
                return false;
            }

            vkCmdBindPipeline(_currentCommandBuffer, VK_PipelineBindPoint_Graphics, _pipelines.at(pipelineSid)->GetVkPipeline());
            return true;
        }}
        //--------------------------------------------------------------------------

        bool VulkanRenderer::BindDescriptorSets(StringID pipelineSid, UInt32 firstSetIndex, const Vector<VkDescriptorSet>& descriptorSets, const Vector<UInt32>& dynamicOffsets /*= Vector<UInt32>()*/) const KMP_PROFILING(ProfileLevelImportant)
        {
            KMP_ASSERT(_currentCommandBuffer);

            if (!_pipelines.contains(pipelineSid))
            {
                KMP_LOG_ERROR("cannot bind descriptor sets with pipeline's sid '{}' - pipeline not found", pipelineSid);
                return false;
            }

            vkCmdBindDescriptorSets(
                _currentCommandBuffer, 
                VK_PipelineBindPoint_Graphics,
                _pipelines.at(pipelineSid)->GetVkPipelineLayout(),
                firstSetIndex, 
                UInt32(descriptorSets.size()), descriptorSets.empty() ? nullptr : descriptorSets.data(),
                UInt32(dynamicOffsets.size()), dynamicOffsets.empty() ? nullptr : dynamicOffsets.data());

            return true;
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

        void VulkanRenderer::DrawIndexed(UInt32 indexCount, UInt32 instanceCount, UInt32 firstIndex, Int32 vertexOffset, UInt32 firstInstance) const KMP_PROFILING(ProfileLevelImportantVerbose)
        {
            KMP_ASSERT(_currentCommandBuffer);

            vkCmdDrawIndexed(_currentCommandBuffer, indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
        }}
        //--------------------------------------------------------------------------

        void VulkanRenderer::Draw(UInt32 vertexCount, UInt32 instanceCount, UInt32 firstVertex, UInt32 firstInstance) const KMP_PROFILING(ProfileLevelImportantVerbose)
        {
            KMP_ASSERT(_currentCommandBuffer);

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