#include "Kmplete/Graphics/OpenGL/opengl_logical_device.h"
#include "Kmplete/Graphics/OpenGL/opengl_swapchain.h"


namespace Kmplete
{
    namespace Graphics
    {
        OpenGLLogicalDevice::OpenGLLogicalDevice() noexcept
        {
            _swapchain.reset(new OpenGLSwapchain());
        }
        //--------------------------------------------------------------------------

        OpenGLLogicalDevice::~OpenGLLogicalDevice()
        {
            _swapchain.reset();
        }
        //--------------------------------------------------------------------------
    }
}