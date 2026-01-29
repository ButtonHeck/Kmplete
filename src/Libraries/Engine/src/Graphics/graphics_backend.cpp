#include "Kmplete/Graphics/graphics_backend.h"
#include "Kmplete/Graphics/OpenGL/opengl_graphics_backend.h"
#include "Kmplete/Graphics/Vulkan/vulkan_graphics_backend.h"
#include "Kmplete/Window/window.h"
#include "Kmplete/Log/log.h"
#include "Kmplete/Profile/profiler.h"


namespace Kmplete
{
    namespace Graphics
    {
        UPtr<GraphicsBackend> GraphicsBackend::Create(Window& window)
        {
            KMP_PROFILE_FUNCTION(ProfileLevelAlways);

            const auto graphicsBackendType = window.GetGraphicsBackendType();

            switch (graphicsBackendType)
            {
            case GraphicsBackendType::OpenGL:
                return CreateUPtr<OpenGLGraphicsBackend>(window);
            case GraphicsBackendType::Vulkan:
                return CreateUPtr<VulkanGraphicsBackend>(window);
            default:
                KMP_LOG_ERROR("cannot create graphics backend instance for graphics backend '{}'", GraphicsBackendTypeToString(graphicsBackendType));
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
}