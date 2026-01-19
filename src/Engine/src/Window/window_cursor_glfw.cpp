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

        try
        {
            Image img(filepath, ImageChannels::RGBAlpha);
            GLFWimage cursorImg{ img.GetWidth(), img.GetHeight(), img.GetPixels() };

            if (cursorImg.pixels && cursorImg.width > 0 && cursorImg.height > 0)
            {
                _cursor = glfwCreateCursor(&cursorImg, hotspot.x, hotspot.y);
            }
            else
            {
                KMP_LOG_WARN("cannot create cursor object from '{}'", filepath);
                throw std::runtime_error("WindowCursorGlfw object creation failed");
            }
        }
        catch (const std::exception&)
        {
            KMP_LOG_WARN("cannot create cursor image from '{}'", filepath);
            throw std::runtime_error("WindowCursorGlfw image creation failed");
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
