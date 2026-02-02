#pragma once

#include "Kmplete/Base/kmplete_api.h"


namespace Kmplete
{
    namespace Graphics
    {
        //TODO: comments
        class LogicalDevice
        {
            KMP_DISABLE_COPY_MOVE(LogicalDevice)

        public:
            LogicalDevice() = default;
            virtual ~LogicalDevice() = default;
        };
        //--------------------------------------------------------------------------
    }
}