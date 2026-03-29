#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Graphics/Vulkan/vulkan_command_buffer.h"

#include <vulkan/vulkan.h>


namespace Kmplete
{
    namespace Graphics
    {
        class VulkanRenderer
        {
            KMP_DISABLE_COPY_MOVE(VulkanRenderer)

        public:
            KMP_API VulkanRenderer(VkDevice device, VkCommandPool commandPool, const UInt32& currentBufferIndex);
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

            KMP_NODISCARD KMP_API VkCommandBuffer GetCurrentCommandBuffer() const noexcept;

        private:
            const UInt32& _currentBufferIndex;

            Vector<VulkanCommandBuffer> _drawCommandBuffers;
            VkCommandBuffer _currentCommandBuffer;
        };
        //--------------------------------------------------------------------------
    }
}