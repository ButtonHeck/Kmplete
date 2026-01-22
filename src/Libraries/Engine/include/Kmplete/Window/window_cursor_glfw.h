#pragma once

#include "Kmplete/Window/window_cursor.h"
#include "Kmplete/Math/geometry.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Log/log_class_macro.h"


struct GLFWcursor;


namespace Kmplete
{
    //! GLFW implementation wrapper of a window cursor.
    //! @see WindowCursor
    //! @see Window
    class WindowCursorGlfw : public WindowCursor
    {
        KMP_LOG_CLASSNAME(WindowCursorGlfw)
        KMP_DISABLE_COPY_MOVE(WindowCursorGlfw)

    public:
        KMP_API explicit WindowCursorGlfw(const Filepath& filepath, const Math::Point2I& hotspot = Math::Point2I());
        KMP_API ~WindowCursorGlfw();

        KMP_NODISCARD KMP_API virtual NonNull<void*> GetImplPointer() const noexcept override;

    private:
        GLFWcursor* _cursor;
    };
    //--------------------------------------------------------------------------
}