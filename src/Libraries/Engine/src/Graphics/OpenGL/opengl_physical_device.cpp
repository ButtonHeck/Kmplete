#include "Kmplete/Graphics/OpenGL/opengl_physical_device.h"
#include "Kmplete/Graphics/OpenGL/opengl_logical_device.h"

#include <glad/glad.h>


namespace Kmplete
{
    namespace Graphics
    {
        OpenGLPhysicalDevice::OpenGLPhysicalDevice()
            : PhysicalDevice()
        {
            _info.vendor = String(reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
            _info.name = String(reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
            _info.driverVersion = String(reinterpret_cast<const char*>(glGetString(GL_VERSION)));

            GLint samples = 0;
            glGetIntegerv(GL_MAX_SAMPLES, &samples);
            _info.msaaSamples = samples;

            PrintInfo();

            _logicalDevice.reset(new OpenGLLogicalDevice());
        }
        //--------------------------------------------------------------------------

        OpenGLPhysicalDevice::~OpenGLPhysicalDevice()
        {
            _logicalDevice.reset();
        }
        //--------------------------------------------------------------------------
    }
}