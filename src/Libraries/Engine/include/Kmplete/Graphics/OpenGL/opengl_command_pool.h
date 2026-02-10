#pragma once

#include "Kmplete/Graphics/command_pool.h"
#include "Kmplete/Base/kmplete_api.h"


namespace Kmplete
{
    namespace Graphics
    {
        //TODO: comments
        class OpenGLCommandPool : public CommandPool
        {
            KMP_DISABLE_COPY_MOVE(OpenGLCommandPool)

        public:
            OpenGLCommandPool() = default;
            ~OpenGLCommandPool() = default;
        };
        //--------------------------------------------------------------------------
    }
}