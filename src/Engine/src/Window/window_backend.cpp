#include "Kmplete/Window/window_backend.h"
#include "Kmplete/Window/window_backend_glfw.h"
#include "Kmplete/Profile/profiler.h"

namespace Kmplete
{
    UPtr<WindowBackend> WindowBackend::Create()
    {
        KMP_PROFILE_FUNCTION();

#if defined (KMP_WINDOW_BACKEND_GLFW)
        return CreateUPtr<WindowBackendGlfw>();
#else
    #error "No window backend is provided!"
#endif
    }
    //--------------------------------------------------------------------------
}
