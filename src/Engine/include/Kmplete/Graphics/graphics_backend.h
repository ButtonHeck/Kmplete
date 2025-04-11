#pragma once

#include "Kmplete/Core/kmplete_api.h"

namespace Kmplete
{
    class GraphicsBackend
    {
    public:
        constexpr static auto OpenGLStr = "OpenGL";
        constexpr static auto DefaultAPI = OpenGLStr;

    public:
        KMP_DISABLE_COPY_MOVE(GraphicsBackend)

        GraphicsBackend() = default;
        virtual ~GraphicsBackend() = default;

        KMP_API virtual void Initialize() const = 0;
    };
    //--------------------------------------------------------------------------
}