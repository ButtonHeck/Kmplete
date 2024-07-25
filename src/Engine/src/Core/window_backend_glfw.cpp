#include "Kmplete/Core/window_backend_glfw.h"
#include "Kmplete/Core/window_glfw.h"
#include "Kmplete/Core/log.h"

#include <GLFW/glfw3.h>

#ifdef CreateWindow
#pragma push_macro("CreateWindow")
#undef CreateWindow
#define KMP_UNDEF_CreateWindow
#endif

namespace Kmplete
{
    WindowBackendGlfw::~WindowBackendGlfw()
    {
        Finalize();
    }
    //--------------------------------------------------------------------------

    bool WindowBackendGlfw::Initialize() const
    {
        if (!glfwInit())
        {
            const char* description;
            const auto errorCode = glfwGetError(&description);
            Log::CoreCritical("WindowBackendGlfw: initialization error: code '{}', description '{}'", errorCode, description ? description : "");
            return false;
        }

        return true;
    }
    //--------------------------------------------------------------------------

    void WindowBackendGlfw::Finalize() const
    {
        glfwTerminate();
    }
    //--------------------------------------------------------------------------

    Ptr<Window> WindowBackendGlfw::CreateWindow() const
    {
        return CreatePtr<WindowGlfw>();
    }
    //--------------------------------------------------------------------------
}

#ifdef KMP_UNDEF_CreateWindow
#pragma pop_macro("CreateWindow")
#undef KMP_UNDEF_CreateWindow
#endif