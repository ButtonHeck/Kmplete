#include "Kmplete/Graphics/OpenGL/opengl_graphics_backend.h"
#include "Kmplete/Log/log.h"
#include "Kmplete/Profile/profiler.h"

#include <glad/glad.h>
#if defined (KMP_WINDOW_BACKEND_GLFW)
    #include <GLFW/glfw3.h>
#endif

#include <stdexcept>


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
        KMP_PROFILE_FUNCTION(ProfileLevelAlways);

        bool ok = false;

#if defined (KMP_WINDOW_BACKEND_GLFW)
        ok = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
#endif

        if (!ok)
        {
            KMP_LOG_ERROR("failed to initialize");
            throw std::runtime_error("OpenGLGraphicsBackend: failed to initialize");
        }

        KMP_LOG_INFO("vendor - {}", reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
        KMP_LOG_INFO("renderer - {}", reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
        KMP_LOG_INFO("version - {}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));
    }
    //--------------------------------------------------------------------------
}
