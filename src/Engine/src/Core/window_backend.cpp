#include "Kmplete/Core/window_backend.h"
#include "Kmplete/Core/window_backend_glfw.h"

namespace Kmplete
{
    Ptr<WindowBackend> WindowBackend::Create()
    {
#ifdef KMP_WINDOW_BACKEND_GLFW
        return CreatePtr<WindowBackendGlfw>();
#else
#error "No window backend is provided!"
#endif
    }
    //--------------------------------------------------------------------------
}
