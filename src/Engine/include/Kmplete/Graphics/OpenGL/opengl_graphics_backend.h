#pragma once

#include "Kmplete/Graphics/graphics_backend.h"
#include "Kmplete/Log/log_class_macro.h"

namespace Kmplete
{
    class OpenGLGraphicsBackend : public GraphicsBackend
    {
        KMP_LOG_CLASSNAME(OpenGLGraphicsBackend)

    public:
        KMP_DISABLE_COPY_MOVE(OpenGLGraphicsBackend)

        KMP_API OpenGLGraphicsBackend();

        KMP_API void Initialize() const override;
    };
    //--------------------------------------------------------------------------
}