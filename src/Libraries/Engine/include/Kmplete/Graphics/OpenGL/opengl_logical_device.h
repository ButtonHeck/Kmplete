#pragma once

#include "Kmplete/Graphics/logical_device.h"
#include "Kmplete/Base/kmplete_api.h"


namespace Kmplete
{
    namespace Graphics
    {
        class OpenGLLogicalDevice : public LogicalDevice
        {
            KMP_DISABLE_COPY_MOVE(OpenGLLogicalDevice)

        public:
            OpenGLLogicalDevice() = default;
            ~OpenGLLogicalDevice() = default;
        };
        //--------------------------------------------------------------------------
    }
}