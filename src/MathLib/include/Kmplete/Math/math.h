#pragma once

#include "Kmplete/Base/kmplete_api.h"

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <type_traits>
#include <algorithm>

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

    
    template<class PositionType, class SizeType>
    requires (std::disjunction_v<
                std::conjunction<
                    std::is_same<PositionType, Point2I>, 
                    std::is_same<SizeType, Size2I>>,
                std::conjunction<
                    std::is_same<PositionType, Point2F>, 
                    std::is_same<SizeType, Size2F>>
             >)
    struct Rect2
    {
        Rect2(const PositionType& position, const SizeType& size) noexcept
            : position(position)
            , size(size)
        {}

        KMP_NODISCARD PositionType GetCenter() const noexcept
        {
            return position + size / static_cast<decltype(size.x)>(2);
        }

        KMP_NODISCARD bool ContainsPoint(const PositionType& point) const noexcept
        {
            const auto [minX, maxX] = std::minmax({position.x, position.x + size.x});
            const auto [minY, maxY] = std::minmax({position.y, position.y + size.y});

            return point.x >= minX && point.x <= maxX &&
                   point.y >= minY && point.y <= maxY;
        }

        PositionType position;
        SizeType size;
    };
    //--------------------------------------------------------------------------


    template<class PositionType, class SizeType>
        requires (std::disjunction_v<
                    std::conjunction<
                        std::is_same<PositionType, Point3I>, 
                        std::is_same<SizeType, Size3I>>,
                    std::conjunction<
                        std::is_same<PositionType, Point3F>, 
                        std::is_same<SizeType, Size3F>>
                 >)
    struct Rect3
    {
        Rect3(const PositionType& position, const SizeType& size) noexcept
            : position(position)
            , size(size)
        {}

        KMP_NODISCARD PositionType GetCenter() const noexcept
        {
            return position + size / static_cast<decltype(size.x)>(2);
        }

        KMP_NODISCARD bool ContainsPoint(const PositionType& point) const noexcept
        {
            const auto [minX, maxX] = std::minmax({position.x, position.x + size.x});
            const auto [minY, maxY] = std::minmax({position.y, position.y + size.y});
            const auto [minZ, maxZ] = std::minmax({position.z, position.z + size.z});

            return point.x >= minX && point.x <= maxX &&
                   point.y >= minY && point.y <= maxY &&
                   point.z >= minZ && point.z <= maxZ;
        }

        PositionType position;
        SizeType size;
    };
    //--------------------------------------------------------------------------


    using Rect2I = Rect2<Point2I, Size2I>;
    using Rect2F = Rect2<Point2F, Size2F>;

    using Rect3I = Rect3<Point3I, Size3I>;
    using Rect3F = Rect3<Point3F, Size3F>;
}