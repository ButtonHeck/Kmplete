#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Graphics/graphics_chain_unit.h"
#include "Kmplete/Graphics/graphics_chain_handler.h"


namespace Kmplete
{
    namespace Graphics
    {
        //! Base class of a graphics API renderer abstraction
        class KMP_API Renderer : public GraphicsChainUnit
        {
            KMP_DISABLE_COPY_MOVE(Renderer)

        public:
            explicit Renderer(GraphicsChainHandler& chainHandler);
            virtual ~Renderer() = default;
        };
        //--------------------------------------------------------------------------
    }
}