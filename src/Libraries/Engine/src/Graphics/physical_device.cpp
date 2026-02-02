#include "Kmplete/Graphics/physical_device.h"
#include "Kmplete/Log/log.h"


namespace Kmplete
{
    namespace Graphics
    {
        PhysicalDevice::PhysicalDevice() noexcept
            : _logicalDevice(nullptr)
        {
            _info.vendor = "Unknown";
            _info.name = "Unknown";
            _info.driverVersion = "Unknown";

            _info.msaaSamples = 0;
        }
        //--------------------------------------------------------------------------

        const PhysicalDevice::Info& PhysicalDevice::GetInfo() const noexcept
        {
            return _info;
        }
        //--------------------------------------------------------------------------

        void PhysicalDevice::PrintInfo() const noexcept
        {
            KMP_LOG_INFO("vendor - {}", _info.vendor);
            KMP_LOG_INFO("name - {}", _info.name);
            KMP_LOG_INFO("driver version - {}", _info.driverVersion);
            KMP_LOG_INFO("MSAA samples - {}", _info.msaaSamples);
        }
        //--------------------------------------------------------------------------
    }
}