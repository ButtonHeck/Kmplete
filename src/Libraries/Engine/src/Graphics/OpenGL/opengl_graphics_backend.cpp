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
        {
            _Initialize();
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
                KMP_LOG_ERROR("failed to initialize");
                throw std::runtime_error("OpenGLGraphicsBackend: failed to initialize");
            }

            //TODO: remove this call and move SetVSync and its settings to upper-level (e.g. Renderer)
            _window.SetVSync(true);

            KMP_LOG_INFO("vendor - {}", reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
            KMP_LOG_INFO("renderer - {}", reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
            KMP_LOG_INFO("version - {}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));
        }
        //--------------------------------------------------------------------------
    }
}
