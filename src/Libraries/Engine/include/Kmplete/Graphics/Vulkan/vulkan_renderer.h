#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Base/pointers.h"
#include "Kmplete/Base/string_id.h"
#include "Kmplete/Base/optional.h"
#include "Kmplete/Graphics/Vulkan/vulkan_command_pool.h"
#include "Kmplete/Graphics/Vulkan/vulkan_command_buffer.h"
#include "Kmplete/Graphics/Vulkan/vulkan_graphics_pipeline.h"
#include "Kmplete/Graphics/Vulkan/vulkan_queue.h"
#include "Kmplete/Graphics/Vulkan/vulkan_buffer.h"
#include "Kmplete/Graphics/Vulkan/vulkan_swapchain.h"
#include "Kmplete/Graphics/Vulkan/vulkan_shader_object.h"
#include "Kmplete/Log/log_class_macro.h"

#include <vulkan/vulkan.h>


namespace Kmplete
{
    namespace Graphics
    {
        class VulkanRenderer
        {
            KMP_DISABLE_COPY_MOVE(VulkanRenderer)
            KMP_LOG_CLASSNAME(VulkanRenderer)

        public:
            KMP_API VulkanRenderer(VkDevice device, const UInt32& currentBufferIndex, const StringIDHashMap<UPtr<VulkanGraphicsPipeline>>& pipelines, 
                                   const StringIDHashMap<UPtr<VulkanShaderObject>>& shaderObjects, UInt32 graphicsFamilyIndex, const VulkanSwapchain& swapchain);
            KMP_API ~VulkanRenderer();

            KMP_API void StartFrame();
            KMP_API void EndFrame();

            KMP_API void SetSwapchain(const VulkanSwapchain& swapchain);

            KMP_API void BeginRendering(const VkRect2D& renderArea) const;
            KMP_API void BeginRendering(StringID pipelineSid, const VkRect2D& renderArea) const;
            KMP_API void EndRendering() const;

            KMP_API void TransitionColorAndDepthStencilImagesToWrite(VkImage colorImage, VkImage depthStencilImage) const;
            KMP_API void TransitionColorAndDepthStencilImagesToPresent(VkImage colorImage) const;

            KMP_API void SetDepthTestEnabled(bool enabled) const;
            KMP_API void SetDepthWriteEnabled(bool enabled) const;
            KMP_API void SetDepthCompareOp(VkCompareOp comparison) const;
            KMP_API void SetDepthBiasEnabled(bool enabled) const;
            KMP_API void SetDepthBias(float constantFactor, float clamp, float slopeFactor) const;
            KMP_API void SetDepthBoundsEnabled(bool enabled) const;
            KMP_API void SetDepthBounds(float min, float max) const;
            KMP_API void SetDepthClampEnabled(bool enabled) const;
            KMP_API void SetDepthClampRange(VkDepthClampModeEXT depthClampMode, float min, float max) const;
            KMP_API void SetDepthClipEnabled(bool enabled) const;
            KMP_API void SetStencilTestEnabled(bool enabled) const;
            KMP_API void SetStencilOp(VkStencilFaceFlags faceMask, VkStencilOp failOp, VkStencilOp passOp, VkStencilOp depthFailOp, VkCompareOp comparison) const;
            KMP_API void SetStencilCompareMask(VkStencilFaceFlags faceMask, UInt32 compareMask) const;
            KMP_API void SetStencilWriteMask(VkStencilFaceFlags faceMask, UInt32 writeMask) const;
            KMP_API void SetStencilReference(VkStencilFaceFlags faceMask, UInt32 reference) const;
            KMP_API void SetViewport(const VkViewport& viewport) const;
            KMP_API void SetScissor(const VkRect2D& scissorRect) const;
            KMP_API void SetViewportWithCount(const Vector<VkViewport>& viewports) const;
            KMP_API void SetScissorWithCount(const Vector<VkRect2D>& scissors) const;
            KMP_API void SetRasterizationSamples(VkSampleCountFlagBits samples) const;
            KMP_API void SetPrimitiveTopology(VkPrimitiveTopology topology) const;
            KMP_API void SetPrimitiveRestartEnabled(bool enabled) const;
            KMP_API void SetLineWidth(float lineWidth) const;
            KMP_API void SetLineStippleEnabled(bool enabled) const;
            KMP_API void SetLineStipple(UInt32 lineStippleFactor, UInt16 lineStipplePattern) const;
            KMP_API void SetLineRasterizationMode(VkLineRasterizationModeEXT lineRasterizationMode) const;
            KMP_API void SetCullMode(VkCullModeFlags cullMode) const;
            KMP_API void SetFrontFace(VkFrontFace frontFace) const;
            KMP_API void SetBlendConstants(const Array<float, 4> constants) const;
            KMP_API void SetRasterizerDiscardEnabled(bool enabled) const;
            KMP_API void SetDiscardRectangleEnabled(bool enabled) const;
            KMP_API void SetDiscardRectangle(UInt32 firstDiscardRectangle, UInt32 count, const Vector<VkRect2D>& discardRectangles) const;
            KMP_API void SetDiscardRectangleMode(VkDiscardRectangleModeEXT mode) const;
            KMP_API void SetSampleLocationsEnabled(bool enabled) const;
            KMP_API void SetSampleLocations(const Vector<VkSampleLocationsInfoEXT>& sampleLocationInfos) const;
            KMP_API void SetSampleMask(VkSampleCountFlagBits samples, const Vector<VkSampleMask>& sampleMasks) const;
            KMP_API void SetFragmentShadingRate(const VkExtent2D& fragmentSize, const Array<VkFragmentShadingRateCombinerOpKHR, 2>& combinerOps) const;
            KMP_API void SetColorWriteEnabled(UInt32 attachmentCount, const Vector<VkBool32>& colorWritesEnables) const;
            KMP_API void SetColorWriteMask(UInt32 firstAttachment, UInt32 attachmentCount, const Vector<VkColorComponentFlags>& colorWritesMasks) const;
            KMP_API void SetColorBlendEnabled(UInt32 firstAttachment, UInt32 attachmentCount, const Vector<VkBool32>& colorBlendsEnables) const;
            KMP_API void SetColorBlendEquation(UInt32 firstAttachment, UInt32 attachmentCount, const Vector<VkColorBlendEquationEXT>& colorBlendsEquations) const;
            KMP_API void SetPolygonMode(VkPolygonMode polygonMode) const;
            KMP_API void SetAlphaToCoverageEnabled(bool enabled) const;
            KMP_API void SetAlphaToOneEnabled(bool enabled) const;
            KMP_API void SetLogicOpEnabled(bool enabled) const;
            KMP_API void SetLogicOp(VkLogicOp logicOp) const;
            KMP_API void SetProvokingVertexMode(VkProvokingVertexModeEXT mode) const;
            KMP_API void SetVertexInput(const Vector<VkVertexInputBindingDescription2EXT>& vertexBindingsDescriptions, const Vector<VkVertexInputAttributeDescription2EXT>& vertexAttributeDescriptions) const;
            KMP_API void BindVertexBuffers2(UInt32 firstBinding, const Vector<VkBuffer>& buffers, const Vector<VkDeviceSize>& offsets, const Vector<VkDeviceSize>& sizes, const Vector<VkDeviceSize>& strides) const;
            KMP_API void BindShaderObjects(const Vector<VkShaderStageFlagBits>& stages, const Vector<StringID>& shadersSids) const;

            KMP_API void SubmitToQueue(const VulkanQueue& queue, const Vector<VkSemaphore>& waitSemaphores, const Vector<VkSemaphore>& signalSemaphores, VkFence fence);

            KMP_API bool BindGraphicsPipeline(StringID pipelineSid) const;
            KMP_API bool BindDescriptorSets(StringID pipelineSid, UInt32 firstSetIndex, const Vector<VkDescriptorSet>& descriptorSets, const Vector<UInt32>& dynamicOffsets = Vector<UInt32>()) const;
            KMP_API bool BindVertexBuffers(UInt32 firstBinding, const Vector<VkBuffer>& vertexBuffers, const Vector<VkDeviceSize>& offsets) const;
            KMP_API void BindIndexBuffer(VkBuffer indexBuffer, VkDeviceSize offset = 0, VkIndexType indexType = VK_INDEX_TYPE_UINT32) const;
            KMP_API void DrawIndexed(UInt32 indexCount, UInt32 instanceCount, UInt32 firstIndex, Int32 vertexOffset, UInt32 firstInstance) const;
            KMP_API void Draw(UInt32 vertexCount, UInt32 instanceCount, UInt32 firstVertex, UInt32 firstInstance) const;
            KMP_API void CopyBuffer(const VulkanCommandBuffer& commandBuffer, const VulkanBuffer& sourceBuffer, const VulkanBuffer& destinationBuffer, VkDeviceSize srcOffset, VkDeviceSize dstOffset, VkDeviceSize size) const;
            KMP_API void CopyBuffer(const VulkanCommandBuffer& commandBuffer, const VulkanBuffer& sourceBuffer, const VulkanBuffer& destinationBuffer, const VkBufferCopy& copyRegion) const;
            KMP_API void CopyBuffer(const VulkanCommandBuffer& commandBuffer, const VulkanBuffer& sourceBuffer, const VulkanBuffer& destinationBuffer, const Vector<VkBufferCopy>& copyRegions) const;

            KMP_NODISCARD KMP_API VulkanCommandBuffer CreateCommandBuffer() const;
            KMP_NODISCARD KMP_API VkCommandBuffer GetCurrentCommandBuffer() const noexcept;

        private:
            const UInt32& _currentBufferIndex;
            const StringIDHashMap<UPtr<VulkanGraphicsPipeline>>& _pipelines;
            const StringIDHashMap<UPtr<VulkanShaderObject>>& _shaderObjects;
            Ref<const VulkanSwapchain> _swapchain;

            VkDevice _device;
            UPtr<VulkanCommandPool> _commandPool;
            Vector<VulkanCommandBuffer> _drawCommandBuffers;
            VkCommandBuffer _currentCommandBuffer;
        };
        //--------------------------------------------------------------------------
    }
}