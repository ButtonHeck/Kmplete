#pragma once

#include "Kmplete/Graphics/graphics_backend.h"

namespace Kmplete
{
    class OpenGLGraphicsBackend : public GraphicsBackend
    {
    public:
        KMP_DISABLE_COPY_MOVE(OpenGLGraphicsBackend)

        KMP_API OpenGLGraphicsBackend();

        KMP_API void Initialize() const override;

        KMP_NODISCARD KMP_API BackendType GetType() const noexcept override;
    };
    //--------------------------------------------------------------------------
}