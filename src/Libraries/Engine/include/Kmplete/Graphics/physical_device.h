#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"
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
            struct Info
            {
                String vendor;
                String name;
                String driverVersion;
            };

        public:
            PhysicalDevice() = default;
            virtual ~PhysicalDevice() = default;

            KMP_NODISCARD KMP_API const Info& GetInfo() const noexcept;
            KMP_API void PrintInfo() const noexcept;

        protected:
            Info _info;
        };
        //--------------------------------------------------------------------------
    }
}