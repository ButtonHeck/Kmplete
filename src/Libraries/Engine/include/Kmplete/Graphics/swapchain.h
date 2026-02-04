#pragma once

#include "Kmplete/Base/kmplete_api.h"


namespace Kmplete
{
    namespace Graphics
    {
        //TODO: comments
        class Swapchain
        {
            KMP_DISABLE_COPY_MOVE(Swapchain)

        public:
            Swapchain() = default;
            virtual ~Swapchain() = default;
        };
        //--------------------------------------------------------------------------
    }
}