#pragma once

#include "Kmplete/Graphics/swapchain.h"
#include "Kmplete/Base/kmplete_api.h"


namespace Kmplete
{
    namespace Graphics
    {
        //TODO: comments
        class OpenGLSwapchain : public Swapchain
        {
            KMP_DISABLE_COPY_MOVE(OpenGLSwapchain)

        public:
            OpenGLSwapchain() = default;
            ~OpenGLSwapchain() = default;
        };
        //--------------------------------------------------------------------------
    }
}