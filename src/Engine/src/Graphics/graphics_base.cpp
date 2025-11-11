#include "Kmplete/Graphics/graphics_base.h"


namespace Kmplete
{
    String GraphicsBackendTypeToString(GraphicsBackendType type) noexcept
    {
        switch (type)
        {
        case GraphicsBackendType::OpenGL:
            return OpenGLStr;
        default:
            return UnknownStr;
        }
    }
    //--------------------------------------------------------------------------

    GraphicsBackendType StringToGraphicsBackendType(const String& string) noexcept
    {
        if (string == OpenGLStr)
        {
            return GraphicsBackendType::OpenGL;
        }

        return GraphicsBackendType::Unknown;
    }
    //--------------------------------------------------------------------------
}