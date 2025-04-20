#include "Kmplete/Graphics/graphics_backend.h"
#include "Kmplete/Graphics/OpenGL/opengl_graphics_backend.h"

namespace Kmplete
{
    UPtr<GraphicsBackend> GraphicsBackend::Create(GraphicsBackendType type)
    {
        switch (type)
        {
        case GraphicsBackendType::OpenGL:
            return CreateUPtr<OpenGLGraphicsBackend>();
        default:
            return nullptr;
        }
    }
    //--------------------------------------------------------------------------

    GraphicsBackend::GraphicsBackend(GraphicsBackendType type)
        : _type(type)
        , _textureManager(CreateUPtr<TextureManager>(type))
    {}
    //--------------------------------------------------------------------------

    GraphicsBackendType GraphicsBackend::GetType() const noexcept
    {
        return _type;
    }
    //--------------------------------------------------------------------------
}