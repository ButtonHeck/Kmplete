#pragma once

#include "Kmplete/Graphics/physical_device.h"
#include "Kmplete/Base/kmplete_api.h"


namespace Kmplete
{
    namespace Graphics
    {
        //TODO: comments
        class OpenGLPhysicalDevice : public PhysicalDevice
        {
            KMP_DISABLE_COPY_MOVE(OpenGLPhysicalDevice)

        public:
            OpenGLPhysicalDevice() = default;
            ~OpenGLPhysicalDevice() = default;
        };
        //--------------------------------------------------------------------------
    }
}