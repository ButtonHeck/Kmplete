#include "Kmplete/Graphics/graphics_backend.h"
#include "Kmplete/Graphics/image.h"
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
            case GraphicsBackendType::Vulkan:
                return CreateUPtr<VulkanGraphicsBackend>(window);
            default:
                KMP_LOG_CRITICAL("cannot create graphics backend instance for graphics backend '{}'", GraphicsBackendTypeToString(graphicsBackendType));
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

        Nullable<Texture*> GraphicsBackend::CreateTexture(const Filepath& filepath, bool flipVertically /*= false*/)
        {
            KMP_PROFILE_FUNCTION(ProfileLevelAlways);

            try
            {
                Image image(filepath, flipVertically);
                return CreateTexture(image);
            }
            catch (const std::exception&)
            {
                KMP_LOG_ERROR("failed to create texture '{}'", filepath);
            }

            return nullptr;
        }
        //--------------------------------------------------------------------------
    }
}