#pragma once

#include "Kmplete/Graphics/graphics_backend.h"
#include "Kmplete/Log/log_class_macro.h"


namespace Kmplete
{
    //! OpenGL backend implementation, loading is delegated to GLAD init function.
    //! Indirectly depends on the window backend, thus a window backend should be 
    //! initialized before this
    //! @see GraphicsBackend
    class OpenGLGraphicsBackend : public GraphicsBackend
    {
        KMP_LOG_CLASSNAME(OpenGLGraphicsBackend)
        KMP_DISABLE_COPY_MOVE(OpenGLGraphicsBackend)

    public:
        KMP_API OpenGLGraphicsBackend();

    private:
        void _Initialize();
    };
    //--------------------------------------------------------------------------
}