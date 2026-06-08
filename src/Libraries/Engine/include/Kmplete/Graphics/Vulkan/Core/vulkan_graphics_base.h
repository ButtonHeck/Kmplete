#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/string_id.h"
#include "Kmplete/Graphics/graphics_base.h"

#include <vulkan/vulkan.h>


namespace Kmplete
{
    namespace Graphics
    {
        KMP_NODISCARD KMP_API VkFormat ImageChannelsToVkFormat(ImageChannels channels) noexcept;
        KMP_NODISCARD KMP_API VkFormat ShaderDataTypeToVkFormat(ShaderDataType type) noexcept;

        static constexpr auto SamplerDefaultNearestSid = "DefaultNearest"_sid;
        static constexpr auto SamplerDefaultLinearSid = "DefaultLinear"_sid;
    }
}