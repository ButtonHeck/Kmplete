#include "Kmplete/Graphics/graphics_base.h"

#include <cmath>

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

    unsigned int GetMipLevelsCount(int width, int height) noexcept
    {
        return static_cast<unsigned int>(std::log2(std::max(width, height)) + 1);
    }
    //--------------------------------------------------------------------------
}