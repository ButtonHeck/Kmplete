#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Base/pointers.h"
#include "Kmplete/Base/string_id.h"
#include "Kmplete/Graphics/Vulkan/vulkan_command_pool.h"
#include "Kmplete/Graphics/Vulkan/vulkan_command_buffer.h"
#include "Kmplete/Graphics/Vulkan/vulkan_graphics_pipeline.h"
#include "Kmplete/Graphics/Vulkan/vulkan_queue.h"
#include "Kmplete/Graphics/Vulkan/vulkan_buffer.h"
#include "Kmplete/Graphics/Vulkan/vulkan_swapchain.h"
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
            KMP_API VulkanRenderer(VkDevice device, const UInt32& currentBufferIndex, const HashMap<StringID, UPtr<VulkanGraphicsPipeline>>& pipelines, UInt32 graphicsFamilyIndex, const VulkanSwapchain& swapchain);
            KMP_API ~VulkanRenderer();

            KMP_API void StartFrame();
            KMP_API void EndFrame();

            KMP_API void BeginRendering(const VkRenderingInfo& renderingInfo) const;
            KMP_API void EndRendering() const;

            KMP_API void TransitionColorAndDepthStencilImagesToWrite(VkImage colorImage, VkImage depthStencilImage) const;
            KMP_API void TransitionColorAndDepthStencilImagesToPresent(VkImage colorImage) const;

            KMP_API void SetViewport(const VkViewport& viewport) const;
            KMP_API void SetScissor(const VkRect2D& scissorRect) const;
            KMP_API void SetRasterizationSamples(VkSampleCountFlagBits samples) const;
            KMP_API void SubmitToQueue(const VulkanQueue& queue, const Vector<VkSemaphore>& waitSemaphores, const Vector<VkSemaphore>& signalSemaphores, VkFence fence);

            KMP_API bool BindGraphicsPipeline(StringID pipelineSid) const;
            KMP_API bool BindDescriptorSets(StringID pipelineSid, UInt32 firstSetIndex, const Vector<VkDescriptorSet>& descriptorSets, const Vector<UInt32>& dynamicOffsets = Vector<UInt32>()) const;
            KMP_API bool BindVertexBuffers(UInt32 firstBinding, const Vector<VkBuffer>& vertexBuffers, const Vector<VkDeviceSize>& offsets) const;
            KMP_API void BindIndexBuffer(VkBuffer indexBuffer, VkDeviceSize offset = 0, VkIndexType indexType = VK_INDEX_TYPE_UINT32) const;
            KMP_API void DrawIndexed(UInt32 indexCount, UInt32 instanceCount, UInt32 firstIndex, Int32 vertexOffset, UInt32 firstInstance) const;
            KMP_API void CopyBuffer(const VulkanCommandBuffer& commandBuffer, const VulkanBuffer& sourceBuffer, const VulkanBuffer& destinationBuffer, const Vector<VkBufferCopy>& copyRegions) const;

            KMP_NODISCARD KMP_API VulkanCommandBuffer CreateCommandBuffer() const;
            KMP_NODISCARD KMP_API VkCommandBuffer GetCurrentCommandBuffer() const noexcept;

        private:
            const UInt32& _currentBufferIndex;
            const HashMap<StringID, UPtr<VulkanGraphicsPipeline>>& _pipelines;
            const VulkanSwapchain& _swapchain;

            VkDevice _device;
            UPtr<VulkanCommandPool> _commandPool;
            Vector<VulkanCommandBuffer> _drawCommandBuffers;
            VkCommandBuffer _currentCommandBuffer;
        };
        //--------------------------------------------------------------------------
    }
}