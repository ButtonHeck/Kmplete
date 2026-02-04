#include "Kmplete/Graphics/OpenGL/opengl_swapchain.h"
#include "Kmplete/Graphics/OpenGL/opengl_framebuffer.h"


namespace Kmplete
{
    namespace Graphics
    {
        OpenGLSwapchain::OpenGLSwapchain() noexcept
            : Swapchain()
        {
            _framebuffers.push_back(CreateUPtr<OpenGLFramebuffer>());
        }
        //--------------------------------------------------------------------------

        OpenGLSwapchain::~OpenGLSwapchain()
        {
            _framebuffers.clear();
        }
        //--------------------------------------------------------------------------
    }
}