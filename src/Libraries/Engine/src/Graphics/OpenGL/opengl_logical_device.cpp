#include "Kmplete/Graphics/OpenGL/opengl_logical_device.h"


namespace Kmplete
{
    namespace Graphics
    {
        OpenGLLogicalDevice::OpenGLLogicalDevice() noexcept
            : _commandPool(new OpenGLCommandPool())
            , _swapchain(nullptr)
        {
            CreateSwapchain();
        }
        //--------------------------------------------------------------------------

        OpenGLLogicalDevice::~OpenGLLogicalDevice()
        {
            DeleteSwapchain();
            _commandPool.reset();
        }
        //--------------------------------------------------------------------------

        const CommandPool& OpenGLLogicalDevice::GetCommandPool() const noexcept
        {
            return *_commandPool.get();
        }
        //--------------------------------------------------------------------------

        const Swapchain& OpenGLLogicalDevice::GetSwapchain() const noexcept
        {
            return *_swapchain.get();
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