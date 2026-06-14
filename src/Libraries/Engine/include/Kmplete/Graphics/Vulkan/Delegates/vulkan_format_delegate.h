#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Log/log_class_macro.h"
#include "Kmplete/Profile/profiler_fwd.h"

#include <vulkan/vulkan.h>


namespace Kmplete
{
    namespace Graphics
    {
        //TODO: comments
        class VulkanFormatDelegate
        {
            KMP_DISABLE_COPY_MOVE(VulkanFormatDelegate)
            KMP_LOG_CLASSNAME(VulkanFormatDelegate)
            KMP_PROFILE_CONSTRUCTOR_DECLARE()

        public:
            KMP_API explicit VulkanFormatDelegate(VkPhysicalDevice physicalDevice) noexcept;
            ~VulkanFormatDelegate() = default;

            KMP_NODISCARD KMP_API VkFormatProperties GetFormatProperties(VkFormat format) const;
            KMP_NODISCARD KMP_API VkFormat FindImageFormat(const Vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features) const;
            KMP_NODISCARD KMP_API bool IsMipmapCompatible(VkFormat format) const;

        private:
            VkPhysicalDevice _physicalDevice;
        };
        //--------------------------------------------------------------------------
    }
}