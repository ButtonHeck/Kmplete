#include "Kmplete/Graphics/graphics_backend.h"
#include "Kmplete/Graphics/OpenGL/opengl_graphics_backend.h"

namespace Kmplete
{
    std::string GraphicsBackend::BackendTypeToString(BackendType type) noexcept
    {
        switch (type)
        {
        case BackendType::OpenGL:
            return OpenGLStr;
        default:
            return UnknownStr;
        }
    }
    //--------------------------------------------------------------------------

    GraphicsBackend::BackendType GraphicsBackend::StringToBackendType(const std::string& string) noexcept
    {
        if (string == OpenGLStr)
        {
            return BackendType::OpenGL;
        }

        return BackendType::Unknown;
    }
    //--------------------------------------------------------------------------

    UPtr<GraphicsBackend> GraphicsBackend::Create(BackendType type)
    {
        switch (type)
        {
        case BackendType::OpenGL:
            return CreateUPtr<OpenGLGraphicsBackend>();
        default:
            return nullptr;
        }
    }
    //--------------------------------------------------------------------------
}