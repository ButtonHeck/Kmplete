#include "Kmplete/Graphics/graphics_manager.h"
#include "Kmplete/Core/settings.h"
#include "Kmplete/Core/assertion.h"

namespace Kmplete
{
    constexpr static auto GraphicsManagerSettingsEntryName = "GraphicsManager";
    constexpr static auto GraphicsManagerAPIStr = "API";

    GraphicsManager::GraphicsManager() noexcept
        : _type(GraphicsBackend::BackendType::OpenGL)
        , _backend(nullptr)
    {}
    //--------------------------------------------------------------------------

    void GraphicsManager::SaveSettings(Settings& settings) const
    {
        settings.StartSaveObject(GraphicsManagerSettingsEntryName);
        settings.SaveString(GraphicsManagerAPIStr, GraphicsBackend::BackendTypeToString(_type));
        settings.EndSaveObject();
    }
    //--------------------------------------------------------------------------

    void GraphicsManager::LoadSettings(Settings& settings)
    {
        settings.StartLoadObject(GraphicsManagerSettingsEntryName);
        _type = GraphicsBackend::StringToBackendType(settings.GetString(GraphicsManagerAPIStr, GraphicsBackend::DefaultAPIStr));
        settings.EndLoadObject();
    }
    //--------------------------------------------------------------------------

    bool GraphicsManager::CreateBackend()
    {
        _backend = GraphicsBackend::Create(_type);
        return _backend != nullptr;
    }
    //--------------------------------------------------------------------------

    GraphicsBackend& GraphicsManager::GetBackend()
    {
        KMP_ASSERT(_backend);
        return *_backend;
    }
    //--------------------------------------------------------------------------
}