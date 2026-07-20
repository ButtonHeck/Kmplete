#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Graphics/command_pool.h"
#include "Kmplete/Log/log_class_macro.h"
#include "Kmplete/Profile/profiler_fwd.h"

#include <vulkan/vulkan.h>


namespace Kmplete
{
    namespace Graphics
    {
        //! Simple Vulkan command pool object wrapper
        class KMP_API VulkanCommandPool : public CommandPool
        {
            KMP_DISABLE_COPY_MOVE(VulkanCommandPool)
            KMP_LOG_CLASSNAME(VulkanCommandPool)
            KMP_PROFILE_CONSTRUCTOR_DECLARE()

        public:
            VulkanCommandPool(VkDevice device, UInt32 graphicsQueueIndex);
            ~VulkanCommandPool();

            KMP_NODISCARD VkCommandPool GetVkCommandPool() const noexcept;

        private:
            VkDevice _device;
            VkCommandPool _commandPool;
        };
        //--------------------------------------------------------------------------
    }
}