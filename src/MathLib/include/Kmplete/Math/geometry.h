#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/type_traits.h"

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/geometric.hpp>

#include <algorithm>


namespace Kmplete
{
    //! Definitions of commonly used geometric primitives backed by the GLM library structs/functions
    namespace Math
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
        //--------------------------------------------------------------------------

    
        //! Two-dimensional rectangle defined by a position vector and a size vector,
        //! underlying primitive type (integral or float) should match for those vectors.
        template<class PositionType, class SizeType>
        requires (Either<
                    Both<
                        Same<PositionType, Point2I>, 
                        Same<SizeType, Size2I>>,
                    Both<
                        Same<PositionType, Point2F>, 
                        Same<SizeType, Size2F>>
                 >::value)
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

            KMP_NODISCARD bool ContainsPoint(const PositionType& point, const typename PositionType::value_type& offset = PositionType::value_type()) const noexcept
            {
                const auto [minX, maxX] = std::minmax({position.x, position.x + size.x});
                const auto [minY, maxY] = std::minmax({position.y, position.y + size.y});

                return (point.x >= minX - offset) && (point.x <= maxX + offset) &&
                       (point.y >= minY - offset) && (point.y <= maxY + offset);
            }

            PositionType position;
            SizeType size;
        };
        //--------------------------------------------------------------------------


        //! Three-dimensional rectangle defined by a position vector and a size vector,
        //! underlying primitive type (integral or float) should match for those vectors.
        template<class PositionType, class SizeType>
        requires (Either<
                    Both<
                        Same<PositionType, Point3I>, 
                        Same<SizeType, Size3I>>,
                    Both<
                        Same<PositionType, Point3F>, 
                        Same<SizeType, Size3F>>
                 >::value)
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

            KMP_NODISCARD bool ContainsPoint(const PositionType& point, const typename PositionType::value_type& offset = PositionType::value_type()) const noexcept
            {
                const auto [minX, maxX] = std::minmax({position.x, position.x + size.x});
                const auto [minY, maxY] = std::minmax({position.y, position.y + size.y});
                const auto [minZ, maxZ] = std::minmax({position.z, position.z + size.z});

                return (point.x >= minX - offset) && (point.x <= maxX + offset) &&
                       (point.y >= minY - offset) && (point.y <= maxY + offset) &&
                       (point.z >= minZ - offset) && (point.z <= maxZ + offset);
            }

            PositionType position;
            SizeType size;
        };
        //--------------------------------------------------------------------------


        using Rect2I = Rect2<Point2I, Size2I>;
        using Rect2F = Rect2<Point2F, Size2F>;

        using Rect3I = Rect3<Point3I, Size3I>;
        using Rect3F = Rect3<Point3F, Size3F>;
        //--------------------------------------------------------------------------
    }
}