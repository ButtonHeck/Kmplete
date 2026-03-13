#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Base/type_traits.h"
#include "Kmplete/Log/log_class_macro.h"

#include <vulkan/vulkan.h>


namespace Kmplete
{
    namespace Graphics
    {
        class VulkanQueue
        {
            KMP_DISABLE_COPY(VulkanQueue)
            KMP_LOG_CLASSNAME(VulkanQueue)

        public:
            KMP_API VulkanQueue(VkDevice device, UInt32 familyIndex, bool supportPresentation);
            KMP_API VulkanQueue(VulkanQueue&& other) noexcept;
            KMP_API VulkanQueue& operator=(VulkanQueue&& other) noexcept;
            ~VulkanQueue() = default;

            KMP_API void WaitIdle() const;
            KMP_API void Submit(const Vector<VkSubmitInfo>& submits, VkFence fence) const;
            KMP_API void Present(const VkPresentInfoKHR& presentationInfo) const;

            KMP_NODISCARD KMP_API bool SupportsPresentation() const noexcept;
            KMP_NODISCARD KMP_API VkQueue GetVkQueue() const noexcept;

        private:
            VkDevice _device;
            UInt32 _familyIndex;
            VkQueue _queue;
            bool _supportPresentation;
        };
        //--------------------------------------------------------------------------

        static_assert(IsMoveConstructible<VulkanQueue>::value);
        static_assert(IsMoveAssignable<VulkanQueue>::value);
    }
}