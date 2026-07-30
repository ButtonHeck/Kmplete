#include "Kmplete/Graphics/graphics_chain_unit.h"
#include "Kmplete/Graphics/graphics_chain_handler.h"


namespace Kmplete
{
    namespace Graphics
    {
        GraphicsChainUnit::GraphicsChainUnit(GraphicsChainHandler& handler, StringID sid)
            : _handler(handler)
            , _sid(sid)
        {
            _handler.AddHandler(this);
        }
        //--------------------------------------------------------------------------
    }
}