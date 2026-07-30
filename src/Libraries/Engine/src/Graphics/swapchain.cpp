#include "Kmplete/Graphics/swapchain.h"


namespace Kmplete
{
    namespace Graphics
    {
        Swapchain::Swapchain(GraphicsChainHandler& chainHandler)
            : GraphicsChainUnit(chainHandler, GraphicsChainHandler::SwapchainUnitSID)
        {}
        //--------------------------------------------------------------------------
    }
}