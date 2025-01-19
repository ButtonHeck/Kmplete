#include "Kmplete/Core/window_backend.h"
#include "Kmplete/Core/window_backend_glfw.h"

namespace Kmplete
{
    UPtr<WindowBackend> WindowBackend::Create()
    {
#if defined (KMP_WINDOW_BACKEND_GLFW)
        return CreateUPtr<WindowBackendGlfw>();
#else
    #error "No window backend is provided!"
#endif
    }
    //--------------------------------------------------------------------------
}
