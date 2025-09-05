#pragma once

#include "Kmplete/Window/window_cursor.h"
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

        KMP_API explicit WindowCursorGlfw(const Filepath& filepath, int hotspotX = 0, int hotspotY = 0);
        KMP_API ~WindowCursorGlfw();

        KMP_NODISCARD KMP_API virtual NonNull<void*> GetImplPointer() const noexcept override;

    private:
        GLFWcursor* _cursor;
    };
    //--------------------------------------------------------------------------
}