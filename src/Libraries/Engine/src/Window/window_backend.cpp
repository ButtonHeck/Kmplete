#include "Kmplete/Window/window_backend.h"
#include "Kmplete/Window/window_backend_glfw.h"
#include "Kmplete/Profile/profiler.h"


namespace Kmplete
{
    UPtr<WindowBackend> WindowBackend::Create(Graphics::GraphicsBackendType graphicsBackendType) KMP_PROFILING(ProfileLevelAlways)
    {
#if defined (KMP_WINDOW_BACKEND_GLFW)
        return CreateUPtr<WindowBackendGlfw>(graphicsBackendType);
#else
    #error "No window backend is provided!"
#endif
    }}
    //--------------------------------------------------------------------------

    WindowBackend::WindowBackend(Graphics::GraphicsBackendType graphicsBackendType)
        : _graphicsBackendType(graphicsBackendType)
    {}
    //--------------------------------------------------------------------------
}
