#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"

namespace Kmplete
{
    enum ImageChannels
    {
        Grey = 1,
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

    KMP_NODISCARD String GraphicsBackendTypeToString(GraphicsBackendType type) noexcept;
    KMP_NODISCARD GraphicsBackendType StringToGraphicsBackendType(const String& string) noexcept;

    constexpr static auto OpenGLStr = "OpenGL";
    constexpr static auto UnknownStr = "Unknown";
    constexpr static auto DefaultAPIStr = OpenGLStr;
    //--------------------------------------------------------------------------

    KMP_NODISCARD unsigned int GetMipLevelsCount(int width, int height) noexcept;
}