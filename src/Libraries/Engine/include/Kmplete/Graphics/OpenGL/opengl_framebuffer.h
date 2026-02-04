#pragma once

#include "Kmplete/Graphics/framebuffer.h"
#include "Kmplete/Base/kmplete_api.h"


namespace Kmplete
{
    namespace Graphics
    {
        //TODO: comments
        class OpenGLFramebuffer : public Framebuffer
        {
            KMP_DISABLE_COPY_MOVE(OpenGLFramebuffer)

        public:
            OpenGLFramebuffer() = default;
            ~OpenGLFramebuffer() = default;
        };
        //--------------------------------------------------------------------------
    }
}