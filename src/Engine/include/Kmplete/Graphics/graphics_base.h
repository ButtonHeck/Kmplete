#pragma once

#include "Kmplete/Core/kmplete_api.h"

#include <string>

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

    KMP_NODISCARD std::string GraphicsBackendTypeToString(GraphicsBackendType type) noexcept;
    KMP_NODISCARD GraphicsBackendType StringToGraphicsBackendType(const std::string& string) noexcept;

    constexpr static auto OpenGLStr = "OpenGL";
    constexpr static auto UnknownStr = "Unknown";
    constexpr static auto DefaultAPIStr = OpenGLStr;
    //--------------------------------------------------------------------------

    KMP_NODISCARD unsigned int GetMipLevelsCount(int width, int height) noexcept;
}