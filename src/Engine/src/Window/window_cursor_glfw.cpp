#include "Kmplete/Window/window_cursor_glfw.h"
#include "Kmplete/Graphics/image.h"
#include "Kmplete/Log/log.h"
#include "Kmplete/Profile/profiler.h"

#include <GLFW/glfw3.h>


namespace Kmplete
{
    WindowCursorGlfw::WindowCursorGlfw(const Filepath& filepath, const Math::Point2I& hotspot /*= Math::Point2I()*/)
        : _cursor(nullptr)
    {
        KMP_PROFILE_FUNCTION(ProfileLevelAlways);

        Image img(filepath, ImageChannels::RGBAlpha);
        GLFWimage cursorImg{ img.GetWidth(), img.GetHeight(), img.GetPixels() };

        if (cursorImg.pixels)
        {
            _cursor = glfwCreateCursor(&cursorImg, hotspot.x, hotspot.y);
        }
        else
        {
            KMP_LOG_WARN("cannot create cursor from '{}'", filepath);
            throw std::runtime_error("WindowCursorGlfw creation failed");
        }
    }
    //--------------------------------------------------------------------------

    WindowCursorGlfw::~WindowCursorGlfw()
    {
        KMP_PROFILE_FUNCTION(ProfileLevelAlways);

        if (_cursor)
        {
            glfwDestroyCursor(_cursor);
        }
    }
    //--------------------------------------------------------------------------

    NonNull<void*> WindowCursorGlfw::GetImplPointer() const noexcept
    {
        return _cursor;
    }
    //--------------------------------------------------------------------------
}
