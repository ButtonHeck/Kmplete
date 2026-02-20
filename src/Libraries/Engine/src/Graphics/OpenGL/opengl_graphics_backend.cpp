#include "Kmplete/Graphics/OpenGL/opengl_graphics_backend.h"
#include "Kmplete/Window/window.h"
#include "Kmplete/Log/log.h"
#include "Kmplete/Profile/profiler.h"

#include <glad/glad.h>
#if defined (KMP_WINDOW_BACKEND_GLFW)
    #include <GLFW/glfw3.h>
#endif

#include <stdexcept>


namespace Kmplete
{
    namespace Graphics
    {
        OpenGLGraphicsBackend::OpenGLGraphicsBackend(Window& window)
            : GraphicsBackend(window)
            , _surface(nullptr)
            , _physicalDevice(nullptr)
        {
            _Initialize();
        }
        //--------------------------------------------------------------------------

        OpenGLGraphicsBackend::~OpenGLGraphicsBackend()
        {
            _Finalize();
        }
        //--------------------------------------------------------------------------

        const GraphicsSurface& OpenGLGraphicsBackend::GetGraphicsSurface() const noexcept
        {
            return *_surface.get();
        }
        //--------------------------------------------------------------------------

        const PhysicalDevice& OpenGLGraphicsBackend::GetPhysicalDevice() const noexcept
        {
            return *_physicalDevice.get();
        }
        //--------------------------------------------------------------------------

        void OpenGLGraphicsBackend::StartFrame(KMP_MB_UNUSED float frameTimestep)
        {
        }
        //--------------------------------------------------------------------------

        void OpenGLGraphicsBackend::EndFrame()
        {
            _window.SwapBuffers();
        }
        //--------------------------------------------------------------------------

        void OpenGLGraphicsBackend::_Initialize()
        {
            KMP_PROFILE_FUNCTION(ProfileLevelAlways);

            _window.MakeContextCurrent();

            bool ok = false;

#if defined (KMP_WINDOW_BACKEND_GLFW)
            ok = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
#endif

            if (!ok)
            {
                KMP_LOG_CRITICAL("failed to initialize");
                throw std::runtime_error("OpenGLGraphicsBackend: failed to initialize");
            }

            //TODO: remove this call and move SetVSync and its settings to upper-level (e.g. Renderer)
            _window.SetVSync(true);

            _surface.reset(new OpenGLGraphicsSurface(_window));
            _physicalDevice.reset(new OpenGLPhysicalDevice());
        }
        //--------------------------------------------------------------------------

        void OpenGLGraphicsBackend::_Finalize()
        {
            _physicalDevice.reset();
            _surface.reset();
        }
        //--------------------------------------------------------------------------
    }
}
