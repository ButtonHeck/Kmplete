#pragma once

#include "Kmplete/Base/kmplete_api.h"

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

namespace Kmplete
{
    using Point2I = glm::ivec2;
    using Point2F = glm::vec2;

    using Point3I = glm::ivec3;
    using Point3F = glm::vec3;

    using Point4I = glm::ivec4;
    using Point4F = glm::vec4;

    using Size2I = glm::ivec2;
    using Size2F = glm::vec2;

    using Size3I = glm::ivec3;
    using Size3F = glm::vec3;

    // TODO: make templated
    struct Rect2I
    {
        KMP_API Rect2I(const Point2I& position, const Size2I& size) noexcept;

        KMP_NODISCARD KMP_API Point2I GetCenter() const noexcept;
        KMP_NODISCARD KMP_API bool ContainsPoint(const Point2I& point) const noexcept;

        Point2I position;
        Size2I size;
    };
    //--------------------------------------------------------------------------
}