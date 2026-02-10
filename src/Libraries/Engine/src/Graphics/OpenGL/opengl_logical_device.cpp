#include "Kmplete/Graphics/OpenGL/opengl_logical_device.h"
#include "Kmplete/Graphics/OpenGL/opengl_command_pool.h"
#include "Kmplete/Graphics/OpenGL/opengl_swapchain.h"


namespace Kmplete
{
    namespace Graphics
    {
        OpenGLLogicalDevice::OpenGLLogicalDevice() noexcept
        {
            _commandPool.reset(new OpenGLCommandPool());
            CreateSwapchain();
        }
        //--------------------------------------------------------------------------

        OpenGLLogicalDevice::~OpenGLLogicalDevice()
        {
            DeleteSwapchain();
            _commandPool.reset();
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

        void OpenGLLogicalDevice::RecreateSwapchain()
        {
            DeleteSwapchain();
            CreateSwapchain();
        }
        //--------------------------------------------------------------------------
    }
}