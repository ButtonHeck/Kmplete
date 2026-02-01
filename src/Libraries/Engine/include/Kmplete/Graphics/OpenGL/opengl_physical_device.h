#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Graphics/physical_device.h"


namespace Kmplete
{
    namespace Graphics
    {
        //TODO: comments
        class OpenGLPhysicalDevice : public PhysicalDevice
        {
            KMP_DISABLE_COPY_MOVE(OpenGLPhysicalDevice)

        public:
            KMP_API OpenGLPhysicalDevice();
            ~OpenGLPhysicalDevice() = default;
        };
        //--------------------------------------------------------------------------
    }
}