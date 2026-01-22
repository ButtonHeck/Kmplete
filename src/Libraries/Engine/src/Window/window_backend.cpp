#include "Kmplete/Window/window_backend.h"
#include "Kmplete/Window/window_backend_glfw.h"
#include "Kmplete/Profile/profiler.h"


namespace Kmplete
{
    UPtr<WindowBackend> WindowBackend::Create()
    {
        KMP_PROFILE_FUNCTION(ProfileLevelAlways);

#if defined (KMP_WINDOW_BACKEND_GLFW)
        return CreateUPtr<WindowBackendGlfw>();
#else
    #error "No window backend is provided!"
#endif
    }
    //--------------------------------------------------------------------------

    WindowBackend::WindowBackend()
        : _graphicsBackendType(GraphicsBackendType::OpenGL)
    {}
    //--------------------------------------------------------------------------

    void WindowBackend::SetGraphicsBackendType(GraphicsBackendType graphicsBackendType)
    {
        _graphicsBackendType = graphicsBackendType;
    }
    //--------------------------------------------------------------------------
}
