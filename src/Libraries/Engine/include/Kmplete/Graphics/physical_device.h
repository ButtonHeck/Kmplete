#pragma once

#include "Kmplete/Base/kmplete_api.h"


namespace Kmplete
{
    namespace Graphics
    {
        //TODO: comments
        class PhysicalDevice
        {
            KMP_DISABLE_COPY_MOVE(PhysicalDevice)

        public:
            PhysicalDevice() = default;
            virtual ~PhysicalDevice() = default;
        };
        //--------------------------------------------------------------------------
    }
}