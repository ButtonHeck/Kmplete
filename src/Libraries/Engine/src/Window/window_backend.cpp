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
        : _graphicsBackendType(Graphics::GraphicsBackendType::OpenGL)
    {}
    //--------------------------------------------------------------------------

    void WindowBackend::SetGraphicsBackendType(Graphics::GraphicsBackendType type) noexcept
    {
        _graphicsBackendType = type;
    }
    //--------------------------------------------------------------------------

    Graphics::GraphicsBackendType WindowBackend::GetGraphicsBackendType() const noexcept
    {
        return _graphicsBackendType;
    }
    //--------------------------------------------------------------------------
}
