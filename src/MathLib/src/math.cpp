#include "Kmplete/Math/math.h"

namespace Kmplete
{
    Rect2I::Rect2I(const Point2I& position, const Size2I& size) noexcept
        : position(position)
        , size(size)
    {}
    //--------------------------------------------------------------------------

    Point2I Rect2I::GetCenter() const noexcept
    {
        return (position + size) / 2;
    }
    //--------------------------------------------------------------------------

    bool Rect2I::ContainsPoint(const Point2I& point) const noexcept
    {
        return point.x >= position.x && 
               point.x <= (position.x + size.x) &&
               point.y >= position.y &&
               point.y <= (position.y + size.y);
    }
    //--------------------------------------------------------------------------
}