#include "Kmplete/Core/window.h"
#include "Kmplete/Core/window_glfw.h"

namespace Kmplete
{
    Window* Window::Create()
    {
#ifdef KMP_WINDOW_BACKEND_GLFW
        return new WindowGlfw();
#else
#error "No window backend is provided!"
#endif
    }
    //--------------------------------------------------------------------------
}