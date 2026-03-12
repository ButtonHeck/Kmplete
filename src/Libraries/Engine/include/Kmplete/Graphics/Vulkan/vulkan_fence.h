#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Base/type_traits.h"

#include <vulkan/vulkan.h>


namespace Kmplete
{
    namespace Graphics
    {
        class VulkanFence
        {
            KMP_DISABLE_COPY(VulkanFence)

        public:
            KMP_API explicit VulkanFence(VkDevice device, bool signaled = true);
            KMP_API VulkanFence(VulkanFence&& other) noexcept;
            KMP_API VulkanFence& operator=(VulkanFence&& other) noexcept;
            KMP_API ~VulkanFence();

            KMP_API void Wait(UInt64 timeout) const;
            KMP_API void Reset() const;

            KMP_NODISCARD KMP_API VkFence GetVkFence() const noexcept;

        private:
            VkDevice _device;
            VkFence _fence;
        };
        //--------------------------------------------------------------------------

        static_assert(IsMoveConstructible<VulkanFence>::value);
        static_assert(IsMoveAssignable<VulkanFence>::value);
    }
}