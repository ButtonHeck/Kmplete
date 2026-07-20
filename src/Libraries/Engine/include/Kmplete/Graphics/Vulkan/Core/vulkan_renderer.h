#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Base/pointers.h"
#include "Kmplete/Base/string_id.h"
#include "Kmplete/Graphics/Vulkan/Command/vulkan_command_pool.h"
#include "Kmplete/Graphics/Vulkan/Command/vulkan_command_buffer.h"
#include "Kmplete/Graphics/Vulkan/Core/vulkan_queue.h"
#include "Kmplete/Graphics/Vulkan/Core/vulkan_swapchain.h"
#include "Kmplete/Graphics/Vulkan/Pipeline/vulkan_graphics_pipeline.h"
#include "Kmplete/Graphics/Vulkan/Pipeline/vulkan_pipeline_manager.h"
#include "Kmplete/Graphics/Vulkan/Buffer/vulkan_buffer.h"
#include "Kmplete/Graphics/Vulkan/Shader/vulkan_shader_manager.h"
#include "Kmplete/Graphics/Vulkan/Texture/vulkan_texture_attachment.h"
#include "Kmplete/Graphics/Vulkan/Utils/bits_aliases.h"
#include "Kmplete/Graphics/Vulkan/Utils/function_utils.h"
#include "Kmplete/Log/log_class_macro.h"
#include "Kmplete/Profile/profiler_fwd.h"

#include <vulkan/vulkan.h>


namespace Kmplete
{
    namespace Graphics
    {
        //! Vulkan API renderer that is responsible for all the rendering-related commands, such as:
        //! beginning/ending rendering (dynamic), drawing, queue submission, 
        //! settings rendering dynamic states values, binding objects, copying buffers
        class KMP_API VulkanRenderer
        {
            KMP_DISABLE_COPY_MOVE(VulkanRenderer)
            KMP_LOG_CLASSNAME(VulkanRenderer)
            KMP_PROFILE_CONSTRUCTOR_DECLARE()

        public:
            VulkanRenderer(VkDevice device, const UInt32& currentBufferIndex, const VulkanPipelineManager& pipelineManager,
                           const VulkanShaderManager& shaderManager, UInt32 graphicsFamilyIndex, const VulkanSwapchain& swapchain);
            ~VulkanRenderer();

            void StartFrame();
            void EndFrame();

            void BeginRendering(const VkRect2D& renderArea, const Vector<VkRenderingAttachmentInfo>& colorAttachments) const;
            void BeginRendering(const VkRect2D& renderArea, const Vector<VkRenderingAttachmentInfo>& colorAttachments, const VkRenderingAttachmentInfo& depthStencilAttachment) const;
            void BeginRendering(const VkRenderingInfo& renderingInfo) const;
            void EndRendering() const;
            void SubmitToQueue(const VulkanQueue& queue, const Vector<VkSemaphore>& waitSemaphores, const Vector<VkSemaphore>& signalSemaphores, VkFence fence);

            void InsertImageMemoryBarrier(const OptionalRef<VulkanTextureAttachment>& attachment, VKUtils::MemoryBarrierParameters& memoryBarrierParameters) const;
            void InsertImageMemoryBarrier(VkImage image, VKUtils::MemoryBarrierParameters& memoryBarrierParameters) const;

            void SetDepthTestEnabled(bool enabled) const;
            void SetDepthWriteEnabled(bool enabled) const;
            void SetDepthCompareOp(VkCompareOp comparison) const;
            void SetDepthBiasEnabled(bool enabled) const;
            void SetDepthBias(float constantFactor, float clamp, float slopeFactor) const;
            void SetDepthBoundsEnabled(bool enabled) const;
            void SetDepthBounds(float min, float max) const;
            void SetDepthClipEnabled(bool enabled) const;
            void SetStencilTestEnabled(bool enabled) const;
            void SetStencilOp(VkStencilFaceFlags faceMask, VkStencilOp failOp, VkStencilOp passOp, VkStencilOp depthFailOp, VkCompareOp comparison) const;
            void SetStencilCompareMask(VkStencilFaceFlags faceMask, UInt32 compareMask) const;
            void SetStencilWriteMask(VkStencilFaceFlags faceMask, UInt32 writeMask) const;
            void SetStencilReference(VkStencilFaceFlags faceMask, UInt32 reference) const;
            void SetViewport(const VkViewport& viewport) const;
            void SetScissor(const VkRect2D& scissorRect) const;
            void SetViewportWithCount(const Vector<VkViewport>& viewports) const;
            void SetScissorWithCount(const Vector<VkRect2D>& scissors) const;
            void SetRasterizationSamples(VkSampleCountFlagBits samples) const;
            void SetPrimitiveTopology(VkPrimitiveTopology topology) const;
            void SetPrimitiveRestartEnabled(bool enabled) const;
            void SetLineWidth(float lineWidth) const;
            void SetLineStippleEnabled(bool enabled) const;
            void SetLineStipple(UInt32 lineStippleFactor, UInt16 lineStipplePattern) const;
            void SetLineRasterizationMode(VkLineRasterizationModeEXT lineRasterizationMode) const;
            void SetCullMode(VkCullModeFlags cullMode) const;
            void SetFrontFace(VkFrontFace frontFace) const;
            void SetBlendConstants(const Array<float, 4> constants) const;
            void SetRasterizerDiscardEnabled(bool enabled) const;
            void SetSampleLocationsEnabled(bool enabled) const;
            void SetSampleLocations(const Vector<VkSampleLocationsInfoEXT>& sampleLocationInfos) const;
            void SetSampleMask(VkSampleCountFlagBits samples, const Vector<VkSampleMask>& sampleMasks) const;
            void SetFragmentShadingRate(const VkExtent2D& fragmentSize, const Array<VkFragmentShadingRateCombinerOpKHR, 2>& combinerOps) const;
            void SetColorWriteEnabled(UInt32 attachmentCount, const Vector<VkBool32>& colorWritesEnables) const;
            void SetColorWriteMask(UInt32 firstAttachment, UInt32 attachmentCount, const Vector<VkColorComponentFlags>& colorWritesMasks) const;
            void SetColorBlendEnabled(UInt32 firstAttachment, UInt32 attachmentCount, const Vector<VkBool32>& colorBlendsEnables) const;
            void SetColorBlendEquation(UInt32 firstAttachment, UInt32 attachmentCount, const Vector<VkColorBlendEquationEXT>& colorBlendsEquations) const;
            void SetPolygonMode(VkPolygonMode polygonMode) const;
            void SetAlphaToCoverageEnabled(bool enabled) const;
            void SetAlphaToOneEnabled(bool enabled) const;
            void SetLogicOpEnabled(bool enabled) const;
            void SetLogicOp(VkLogicOp logicOp) const;
            void SetProvokingVertexMode(VkProvokingVertexModeEXT mode) const;
            void SetVertexInput(const Vector<VkVertexInputBindingDescription2EXT>& vertexBindingsDescriptions, const Vector<VkVertexInputAttributeDescription2EXT>& vertexAttributeDescriptions) const;

            bool BindGraphicsPipeline(StringID pipelineSid) const;
            bool BindDescriptorSets(StringID layoutSid, UInt32 firstSetIndex, const Vector<VkDescriptorSet>& descriptorSets, const Vector<UInt32>& dynamicOffsets = Vector<UInt32>()) const;
            void PushConstants(StringID layoutSid, VkShaderStageFlags shaderStagesFlags, UInt32 offset, UInt32 size, const void* data) const;
            bool BindVertexBuffers(UInt32 firstBinding, const Vector<VkBuffer>& vertexBuffers, const Vector<VkDeviceSize>& offsets) const;
            void BindVertexBuffers2(UInt32 firstBinding, const Vector<VkBuffer>& buffers, const Vector<VkDeviceSize>& offsets, const Vector<VkDeviceSize>& sizes, const Vector<VkDeviceSize>& strides) const;
            void BindIndexBuffer(const VulkanBuffer& indexBuffer, VkDeviceSize offset = 0, VkIndexType indexType = VKBits::VK_Index_UInt32) const;
            void BindIndexBuffer(VkBuffer indexBuffer, VkDeviceSize offset = 0, VkIndexType indexType = VKBits::VK_Index_UInt32) const;
            void BindShaderObjects(const Vector<VkShaderStageFlagBits>& stages, const Vector<StringID>& shadersSids) const;

            void Draw(UInt32 vertexCount, UInt32 instanceCount, UInt32 firstVertex, UInt32 firstInstance) const;
            void DrawIndexed(UInt32 indexCount, UInt32 instanceCount, UInt32 firstIndex, Int32 vertexOffset, UInt32 firstInstance) const;
            void DrawIndirect(const VulkanBuffer& indirectBuffer, VkDeviceSize offset, UInt32 drawCount, UInt32 stride = sizeof(VkDrawIndirectCommand)) const;
            void DrawIndirect(VkBuffer indirectBuffer, VkDeviceSize offset, UInt32 drawCount, UInt32 stride = sizeof(VkDrawIndirectCommand)) const;
            void DrawIndexedIndirect(const VulkanBuffer& indirectBuffer, VkDeviceSize offset, UInt32 drawCount, UInt32 stride = sizeof(VkDrawIndexedIndirectCommand)) const;
            void DrawIndexedIndirect(VkBuffer indirectBuffer, VkDeviceSize offset, UInt32 drawCount, UInt32 stride = sizeof(VkDrawIndexedIndirectCommand)) const;

            void CopyBuffer(const VulkanCommandBuffer& commandBuffer, const VulkanBuffer& sourceBuffer, const VulkanBuffer& destinationBuffer, VkDeviceSize srcOffset, VkDeviceSize dstOffset, VkDeviceSize size) const;
            void CopyBuffer(const VulkanCommandBuffer& commandBuffer, const VulkanBuffer& sourceBuffer, const VulkanBuffer& destinationBuffer, const VkBufferCopy& copyRegion) const;
            void CopyBuffer(const VulkanCommandBuffer& commandBuffer, const VulkanBuffer& sourceBuffer, const VulkanBuffer& destinationBuffer, const Vector<VkBufferCopy>& copyRegions) const;
            void CopyBuffers(const VulkanBuffer& stagingBuffer, const Vector<VKUtils::BufferCopyParameters>& copyParameters, const VulkanQueue& queue) const;

            KMP_NODISCARD VulkanCommandBuffer CreateCommandBuffer() const;
            KMP_NODISCARD VkCommandBuffer GetCurrentCommandBuffer() const noexcept;

        private:
            void _Initialize(UInt32 graphicsFamilyIndex);
            void _Finalize();

        private:
            const UInt32& _currentBufferIndex;
            const VulkanPipelineManager& _pipelineManager;
            const VulkanShaderManager& _shaderManager;
            const VulkanSwapchain& _swapchain;

            VkDevice _device;
            UPtr<VulkanCommandPool> _commandPool;
            Vector<VulkanCommandBuffer> _drawCommandBuffers;
            VkCommandBuffer _currentCommandBuffer;
        };
        //--------------------------------------------------------------------------
    }
}