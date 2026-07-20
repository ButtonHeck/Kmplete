#pragma once

#include "Kmplete/Base/kmplete_api.h"


namespace Kmplete
{
    namespace Graphics
    {
        //! Base class of a graphics API swapchain abstraction
        class KMP_API Swapchain
        {
            KMP_DISABLE_COPY_MOVE(Swapchain)

        public:
            Swapchain() = default;
            virtual ~Swapchain() = default;

            virtual void StartFrame(float /*frameTimestep*/) {};
            virtual void EndFrame() {};
        };
        //--------------------------------------------------------------------------
    }
}