#pragma once

#include "Kmplete/Window/window_cursor.h"
#include "Kmplete/Math/math.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Log/log_class_macro.h"

struct GLFWcursor;

namespace Kmplete
{
    class WindowCursorGlfw : public WindowCursor
    {
        KMP_LOG_CLASSNAME(WindowCursorGlfw)

    public:
        KMP_DISABLE_COPY_MOVE(WindowCursorGlfw)

        KMP_API explicit WindowCursorGlfw(const Filepath& filepath, const Point2I& hotspot = Point2I());
        KMP_API ~WindowCursorGlfw();

        KMP_NODISCARD KMP_API virtual NonNull<void*> GetImplPointer() const noexcept override;

    private:
        GLFWcursor* _cursor;
    };
    //--------------------------------------------------------------------------
}