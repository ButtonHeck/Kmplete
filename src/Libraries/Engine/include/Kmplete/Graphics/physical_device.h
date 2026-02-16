#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Base/pointers.h"
#include "Kmplete/Graphics/logical_device.h"
#include "Kmplete/Log/log_class_macro.h"


namespace Kmplete
{
    namespace Graphics
    {
        //TODO: comments
        class PhysicalDevice
        {
            KMP_DISABLE_COPY_MOVE(PhysicalDevice)
            KMP_LOG_CLASSNAME(PhysicalDevice)

        public:
            struct GPUInfo
            {
                String vendor;
                String name;
                String driverVersion;
            };

        public:
            PhysicalDevice() noexcept;
            virtual ~PhysicalDevice() = default;

            KMP_NODISCARD KMP_API const GPUInfo& GetGPUInfo() const noexcept;
            KMP_API void PrintGPUInfo() const noexcept;

            KMP_NODISCARD KMP_API const LogicalDevice& GetLogicalDevice() const noexcept;

        protected:
            virtual void _QueryGPUInfo() = 0;

        protected:
            GPUInfo _gpuInfo;
            UPtr<LogicalDevice> _logicalDevice;
        };
        //--------------------------------------------------------------------------
    }
}