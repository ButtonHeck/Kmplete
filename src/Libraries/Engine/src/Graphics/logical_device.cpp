#include "Kmplete/Graphics/logical_device.h"


namespace Kmplete
{
    namespace Graphics
    {
        LogicalDevice::LogicalDevice() noexcept
            : _commandPool(nullptr)
            , _swapchain(nullptr)
        {}
        //--------------------------------------------------------------------------
    }
}