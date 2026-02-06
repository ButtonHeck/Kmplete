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
            _QueryGPUInfo();
            PrintGPUInfo();

            _logicalDevice.reset(new OpenGLLogicalDevice());
        }
        //--------------------------------------------------------------------------

        OpenGLPhysicalDevice::~OpenGLPhysicalDevice()
        {
            _logicalDevice.reset();
        }
        //--------------------------------------------------------------------------

        void OpenGLPhysicalDevice::_QueryGPUInfo()
        {
            _gpuInfo.vendor = String(reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
            _gpuInfo.name = String(reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
            _gpuInfo.driverVersion = String(reinterpret_cast<const char*>(glGetString(GL_VERSION)));
        }
        //--------------------------------------------------------------------------
    }
}