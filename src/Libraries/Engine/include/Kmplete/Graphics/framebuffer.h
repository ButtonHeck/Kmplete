#pragma once

#include "Kmplete/Base/kmplete_api.h"


namespace Kmplete
{
    namespace Graphics
    {
        //TODO: comments
        class Framebuffer
        {
            KMP_DISABLE_COPY_MOVE(Framebuffer)

        public:
            Framebuffer() = default;
            virtual ~Framebuffer() = default;
        };
        //--------------------------------------------------------------------------
    }
}