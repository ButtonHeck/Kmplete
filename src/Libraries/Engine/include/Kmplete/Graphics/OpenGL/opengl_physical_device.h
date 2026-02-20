#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Graphics/physical_device.h"
#include "Kmplete/Graphics/OpenGL/opengl_logical_device.h"


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
            KMP_API ~OpenGLPhysicalDevice();

            KMP_NODISCARD KMP_API const LogicalDevice& GetLogicalDevice() const noexcept override;

        protected:
            void _QueryGPUInfo() override;

        private:
            UPtr<OpenGLLogicalDevice> _logicalDevice;
        };
        //--------------------------------------------------------------------------
    }
}