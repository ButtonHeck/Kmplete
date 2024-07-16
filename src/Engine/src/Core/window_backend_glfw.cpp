#include "Kmplete/Core/window_backend_glfw.h"
#include "Kmplete/Core/window_glfw.h"

#include <GLFW/glfw3.h>

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