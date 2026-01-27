#include "Kmplete/Graphics/graphics_backend.h"
#include "Kmplete/Graphics/OpenGL/opengl_graphics_backend.h"
#include "Kmplete/Graphics/Vulkan/vulkan_graphics_backend.h"
#include "Kmplete/Window/window.h"


namespace Kmplete
{
    UPtr<GraphicsBackend> GraphicsBackend::Create(Window& window)
    {
        const auto graphicsBackendType = window.GetGraphicsBackendType();

        switch (graphicsBackendType)
        {
        case GraphicsBackendType::OpenGL:
            return CreateUPtr<OpenGLGraphicsBackend>(window);
        case GraphicsBackendType::Vulkan:
            return CreateUPtr<VulkanGraphicsBackend>(window);
        default:
            return nullptr;
        }
    }
    //--------------------------------------------------------------------------

    GraphicsBackend::GraphicsBackend(Window& window)
        : _window(window)
    {}
    //--------------------------------------------------------------------------

    GraphicsBackendType GraphicsBackend::GetType() const noexcept
    {
        return _window.GetGraphicsBackendType();
    }
    //--------------------------------------------------------------------------
}