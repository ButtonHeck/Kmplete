#include "Kmplete/Graphics/OpenGL/opengl_physical_device.h"

#include <glad/glad.h>


namespace Kmplete
{
    namespace Graphics
    {
        OpenGLPhysicalDevice::OpenGLPhysicalDevice()
            : PhysicalDevice()
            , _logicalDevice(nullptr)
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

        const LogicalDevice& OpenGLPhysicalDevice::GetLogicalDevice() const noexcept
        {
            return *_logicalDevice.get();
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