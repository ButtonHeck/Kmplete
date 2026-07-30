#include "Kmplete/Graphics/renderer.h"


namespace Kmplete
{
    namespace Graphics
    {
        Renderer::Renderer(GraphicsChainHandler& chainHandler)
            : GraphicsChainUnit(chainHandler, GraphicsChainHandler::RendererUnitSID)
        {}
        //--------------------------------------------------------------------------
    }
}