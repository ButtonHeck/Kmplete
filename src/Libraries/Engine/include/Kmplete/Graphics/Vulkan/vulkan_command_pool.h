#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Graphics/command_pool.h"
#include "Kmplete/Log/log_class_macro.h"

#include <vulkan/vulkan.h>


namespace Kmplete
{
    namespace Graphics
    {
        //TODO: comments
        class VulkanCommandPool : public CommandPool
        {
            KMP_DISABLE_COPY_MOVE(VulkanCommandPool)
            KMP_LOG_CLASSNAME(VulkanCommandPool)

        public:
            KMP_API VulkanCommandPool(const VkDevice& device, UInt32 graphicsQueueIndex);
            KMP_API ~VulkanCommandPool();

            KMP_NODISCARD KMP_API const VkCommandPool& GetPool() const noexcept;

        private:
            const VkDevice& _device;

            VkCommandPool _commandPool;
        };
        //--------------------------------------------------------------------------
    }
}