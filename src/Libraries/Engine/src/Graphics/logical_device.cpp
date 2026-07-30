#include "Kmplete/Graphics/logical_device.h"


namespace Kmplete
{
    namespace Graphics
    {
        LogicalDevice::LogicalDevice(GraphicsChainHandler& chainHandler)
            : GraphicsChainUnit(chainHandler, GraphicsChainHandler::LogicalDeviceUnitSID)
        {}
        //--------------------------------------------------------------------------
    }
}