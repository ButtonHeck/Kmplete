#include "Kmplete/Graphics/graphics_backend_manager.h"
#include "Kmplete/Graphics/OpenGL/opengl_graphics_backend.h"
#include "Kmplete/Core/settings.h"
#include "Kmplete/Core/log.h"
#include "Kmplete/Core/assertion.h"

namespace Kmplete
{
    constexpr static auto GraphicsBackendManagerSettingsEntryName = "GraphicsBackendManager";
    constexpr static auto GraphicsBackendManagerAPIStr = "API";

    constexpr static auto OpenGLStr = "OpenGL";
    constexpr static auto DefaultAPI = OpenGLStr;

    GraphicsBackendManager::GraphicsBackendManager() noexcept
        : _apiName(DefaultAPI)
        , _backend(nullptr)
    {}
    //--------------------------------------------------------------------------

    void GraphicsBackendManager::SaveSettings(Settings& settings) const
    {
        settings.StartSaveObject(GraphicsBackendManagerSettingsEntryName);
        settings.SaveString(GraphicsBackendManagerAPIStr, _apiName);
        settings.EndSaveObject();
    }
    //--------------------------------------------------------------------------

    void GraphicsBackendManager::LoadSettings(Settings& settings)
    {
        settings.StartLoadObject(GraphicsBackendManagerSettingsEntryName);
        _apiName = settings.GetString(GraphicsBackendManagerAPIStr, DefaultAPI);
        settings.EndLoadObject();
    }
    //--------------------------------------------------------------------------

    Ptr<GraphicsBackend> GraphicsBackendManager::CreateBackend()
    {
        if (_apiName == OpenGLStr)
        {
            KMP_LOG_CORE_INFO("GraphicsBackendManager: creating OpenGL backend");
            _backend = CreatePtr<OpenGLGraphicsBackend>();
            KMP_ASSERT(_backend);
        }

        return _backend;
    }
    //--------------------------------------------------------------------------
}