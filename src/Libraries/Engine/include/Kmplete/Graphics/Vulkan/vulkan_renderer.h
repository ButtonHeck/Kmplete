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
            KMP_API VulkanRenderer(VkDevice device, VkCommandPool commandPool, const UInt32& currentBufferIndex, const VkExtent2D& currentExtent);
            KMP_API ~VulkanRenderer();

            KMP_API void StartFrame();
            KMP_API void EndFrame();

            KMP_NODISCARD KMP_API VkCommandBuffer GetCurrentCommandBuffer() const noexcept;

        private:
            const UInt32& _currentBufferIndex;
            const VkExtent2D& _currentExtent;

            Vector<VulkanCommandBuffer> _drawCommandBuffers;
        };
        //--------------------------------------------------------------------------
    }
}