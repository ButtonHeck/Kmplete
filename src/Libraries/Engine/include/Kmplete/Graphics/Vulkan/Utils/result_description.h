#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"

#include <vulkan/vulkan.h>


namespace Kmplete
{
    namespace Graphics
    {
        namespace VulkanUtils
        {
            KMP_API VkResult CheckResult(VkResult result, const char* message, bool throwException = true);
            KMP_NODISCARD KMP_API String VkResultToString(VkResult result) noexcept;
        }
    }
}