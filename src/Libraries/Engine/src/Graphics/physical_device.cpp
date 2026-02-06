#include "Kmplete/Graphics/physical_device.h"
#include "Kmplete/Log/log.h"


namespace Kmplete
{
    namespace Graphics
    {
        PhysicalDevice::PhysicalDevice() noexcept
            : _logicalDevice(nullptr)
        {
            _gpuInfo.vendor = "Unknown";
            _gpuInfo.name = "Unknown";
            _gpuInfo.driverVersion = "Unknown";
        }
        //--------------------------------------------------------------------------

        const PhysicalDevice::GPUInfo& PhysicalDevice::GetGPUInfo() const noexcept
        {
            return _gpuInfo;
        }
        //--------------------------------------------------------------------------

        void PhysicalDevice::PrintGPUInfo() const noexcept
        {
            KMP_LOG_INFO("vendor - {}", _gpuInfo.vendor);
            KMP_LOG_INFO("name - {}", _gpuInfo.name);
            KMP_LOG_INFO("driver version - {}", _gpuInfo.driverVersion);
        }
        //--------------------------------------------------------------------------
    }
}