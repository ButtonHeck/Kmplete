#include "Kmplete/Graphics/OpenGL/opengl_logical_device.h"
#include "Kmplete/Graphics/OpenGL/opengl_swapchain.h"


namespace Kmplete
{
    namespace Graphics
    {
        OpenGLLogicalDevice::OpenGLLogicalDevice() noexcept
        {
            CreateSwapchain();
        }
        //--------------------------------------------------------------------------

        OpenGLLogicalDevice::~OpenGLLogicalDevice()
        {
            DeleteSwapchain();
        }
        //--------------------------------------------------------------------------

        void OpenGLLogicalDevice::CreateSwapchain()
        {
            _swapchain.reset(new OpenGLSwapchain());
        }
        //--------------------------------------------------------------------------

        void OpenGLLogicalDevice::DeleteSwapchain()
        {
            _swapchain.reset();
        }
        //--------------------------------------------------------------------------
    }
}