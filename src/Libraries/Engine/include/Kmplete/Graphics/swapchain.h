#pragma once

#include "Kmplete/Base/kmplete_api.h"


namespace Kmplete
{
    namespace Graphics
    {
        //! Base class of a graphics API swapchain abstraction
        class Swapchain
        {
            KMP_DISABLE_COPY_MOVE(Swapchain)

        public:
            Swapchain() = default;
            virtual ~Swapchain() = default;

            KMP_API virtual void StartFrame(float /*frameTimestep*/) {};
            KMP_API virtual void EndFrame() {};
        };
        //--------------------------------------------------------------------------
    }
}