#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Graphics/graphics_chain_unit.h"
#include "Kmplete/Graphics/graphics_chain_handler.h"


namespace Kmplete
{
    namespace Graphics
    {
        //! Base class of a graphics API swapchain abstraction
        class KMP_API Swapchain : public GraphicsChainUnit
        {
            KMP_DISABLE_COPY_MOVE(Swapchain)

        public:
            explicit Swapchain(GraphicsChainHandler& chainHandler);
            virtual ~Swapchain() = default;
        };
        //--------------------------------------------------------------------------
    }
}