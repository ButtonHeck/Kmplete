#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"


namespace Kmplete
{
    enum ImageChannels
    {
        Unknown = 0,
        Grey,
        GreyAlpha,
        RGB,
        RGBAlpha
    };
    //--------------------------------------------------------------------------

    enum class GraphicsBackendType
    {
        OpenGL,
        Unknown
    };

    KMP_NODISCARD KMP_API String GraphicsBackendTypeToString(GraphicsBackendType type) noexcept;
    KMP_NODISCARD KMP_API GraphicsBackendType StringToGraphicsBackendType(const String& string) noexcept;

    static constexpr auto OpenGLStr = "OpenGL";
    static constexpr auto UnknownStr = "Unknown";
    static constexpr auto DefaultAPIStr = OpenGLStr;
    //--------------------------------------------------------------------------

    KMP_NODISCARD unsigned int GetMipLevelsCount(int width, int height) noexcept;
}