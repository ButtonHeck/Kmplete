#include "Kmplete/Graphics/graphics_backend_manager.h"
#include "Kmplete/Graphics/OpenGL/opengl_graphics_backend.h"
#include "Kmplete/Core/settings.h"
#include "Kmplete/Core/log.h"
#include "Kmplete/Core/assertion.h"

namespace Kmplete
{
    constexpr static auto GraphicsBackendManagerSettingsEntryName = "GraphicsBackendManager";
    constexpr static auto GraphicsBackendManagerAPIStr = "API";

    GraphicsBackendManager::GraphicsBackendManager() noexcept
        : _type(GraphicsBackend::BackendType::Unknown)
        , _backend(nullptr)
    {}
    //--------------------------------------------------------------------------

    void GraphicsBackendManager::SaveSettings(Settings& settings) const
    {
        settings.StartSaveObject(GraphicsBackendManagerSettingsEntryName);
        settings.SaveString(GraphicsBackendManagerAPIStr, GraphicsBackend::BackendTypeToString(_type));
        settings.EndSaveObject();
    }
    //--------------------------------------------------------------------------

    void GraphicsBackendManager::LoadSettings(Settings& settings)
    {
        settings.StartLoadObject(GraphicsBackendManagerSettingsEntryName);
        _type = GraphicsBackend::StringToBackendType(settings.GetString(GraphicsBackendManagerAPIStr, GraphicsBackend::DefaultAPIStr));
        settings.EndLoadObject();
    }
    //--------------------------------------------------------------------------

    Ptr<GraphicsBackend> GraphicsBackendManager::CreateBackend()
    {
        if (_type == GraphicsBackend::BackendType::OpenGL)
        {
            KMP_LOG_CORE_INFO("GraphicsBackendManager: creating OpenGL backend");
            _backend = CreatePtr<OpenGLGraphicsBackend>();
            KMP_ASSERT(_backend);
        }

        return _backend;
    }
    //--------------------------------------------------------------------------
}