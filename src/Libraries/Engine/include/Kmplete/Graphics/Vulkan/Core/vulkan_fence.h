#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Base/type_traits.h"
#include "Kmplete/Profile/profiler_fwd.h"

#include <vulkan/vulkan.h>


namespace Kmplete
{
    namespace Graphics
    {
        //! Simple Vulkan API fence object wrapper
        class KMP_API VulkanFence
        {
            KMP_DISABLE_COPY(VulkanFence)
            KMP_PROFILE_CONSTRUCTOR_DECLARE()

        public:
            explicit VulkanFence(VkDevice device, bool signaled = true);
            VulkanFence(VulkanFence&& other) noexcept;
            VulkanFence& operator=(VulkanFence&& other) noexcept;
            ~VulkanFence();

            void Wait() const;
            void Wait(UInt64 timeout) const;
            void Reset() const;

            KMP_NODISCARD VkFence GetVkFence() const noexcept;

        private:
            void _Initialize(bool signaled);
            void _Finalize();

        private:
            VkDevice _device;
            VkFence _fence;
        };
        //--------------------------------------------------------------------------

        static_assert(IsMoveConstructible<VulkanFence>::value);
        static_assert(IsMoveAssignable<VulkanFence>::value);
    }
}