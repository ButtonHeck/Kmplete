#include "Kmplete/Graphics/graphics_chain_unit.h"
#include "Kmplete/Graphics/graphics_chain_handler.h"


namespace Kmplete
{
    namespace Graphics
    {
        GraphicsChainUnit::GraphicsChainUnit(GraphicsChainHandler& chainHandler, StringID sid)
            : _chainHandler(chainHandler)
            , _sid(sid)
        {
            _chainHandler.AddHandler(this);
        }
        //--------------------------------------------------------------------------
    }
}