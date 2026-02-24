#include "Kmplete/Graphics/OpenGL/opengl_logical_device.h"


namespace Kmplete
{
    namespace Graphics
    {
        OpenGLLogicalDevice::OpenGLLogicalDevice() noexcept
            : _commandPool(new OpenGLCommandPool())
            , _swapchain(nullptr)
        {
            _CreateSwapchain();
        }
        //--------------------------------------------------------------------------

        OpenGLLogicalDevice::~OpenGLLogicalDevice()
        {
            _DeleteSwapchain();
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

        void OpenGLLogicalDevice::_CreateSwapchain()
        {
            _swapchain.reset(new OpenGLSwapchain());
        }
        //--------------------------------------------------------------------------

        void OpenGLLogicalDevice::_DeleteSwapchain()
        {
            _swapchain.reset();
        }
        //--------------------------------------------------------------------------
    }
}