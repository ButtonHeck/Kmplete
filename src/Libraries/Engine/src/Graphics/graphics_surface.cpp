#include "Kmplete/Graphics/graphics_surface.h"
#include "Kmplete/Graphics/OpenGL/opengl_graphics_surface.h"
#include "Kmplete/Graphics/Vulkan/vulkan_graphics_surface.h"
#include "Kmplete/Window/window.h"
#include "Kmplete/Profile/profiler.h"
#include "Kmplete/Log/log.h"


namespace Kmplete
{
    namespace Graphics
    {
        UPtr<GraphicsSurface> GraphicsSurface::Create(Window& window)
        {
            KMP_PROFILE_FUNCTION(ProfileLevelAlways);

            const auto graphicsBackendType = window.GetGraphicsBackendType();

            switch (graphicsBackendType)
            {
            case GraphicsBackendType::OpenGL:
                return CreateUPtr<OpenGLGraphicsSurface>(window);
            case GraphicsBackendType::Vulkan:
                return CreateUPtr<VulkanGraphicsSurface>(window);
            default:
                KMP_LOG_ERROR("cannot create graphics surface for graphics backend '{}'", GraphicsBackendTypeToString(graphicsBackendType));
                return nullptr;
            }
        }
        //--------------------------------------------------------------------------

        GraphicsSurface::GraphicsSurface(Window& window)
            : _window(window)
        {}
        //--------------------------------------------------------------------------
    }
}