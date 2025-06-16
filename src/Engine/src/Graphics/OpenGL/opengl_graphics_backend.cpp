#include "Kmplete/Graphics/OpenGL/opengl_graphics_backend.h"
#include "Kmplete/Log/log.h"

#include <glad/glad.h>
#if defined (KMP_WINDOW_BACKEND_GLFW)
    #include <GLFW/glfw3.h>
#endif

namespace Kmplete
{
    OpenGLGraphicsBackend::OpenGLGraphicsBackend()
        : GraphicsBackend(GraphicsBackendType::OpenGL)
    {
        Initialize();
    }
    //--------------------------------------------------------------------------

    void OpenGLGraphicsBackend::Initialize() const
    {
        bool ok = false;

#if defined (KMP_WINDOW_BACKEND_GLFW)
        ok = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
#endif

        if (!ok)
        {
            KMP_LOG_ERROR("OpenGLGraphicsBackend: failed to initialize");
            return;
        }

        KMP_LOG_INFO("OpenGLGraphicsBackend: vendor - {}", reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
        KMP_LOG_INFO("OpenGLGraphicsBackend: renderer - {}", reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
        KMP_LOG_INFO("OpenGLGraphicsBackend: version - {}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));
    }
    //--------------------------------------------------------------------------
}
