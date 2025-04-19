#include "Kmplete/Graphics/graphics_manager.h"
#include "Kmplete/Core/settings.h"
#include "Kmplete/Core/assertion.h"

namespace Kmplete
{
    constexpr static auto SettingsEntryName = "GraphicsManager";
    constexpr static auto APIStr = "API";

    GraphicsManager::GraphicsManager() noexcept
        : _type(GraphicsBackend::BackendType::OpenGL)
        , _backend(nullptr)
    {}
    //--------------------------------------------------------------------------

    void GraphicsManager::SaveSettings(Settings& settings) const
    {
        settings.StartSaveObject(SettingsEntryName);
        settings.SaveString(APIStr, GraphicsBackend::BackendTypeToString(_type));
        settings.EndSaveObject();
    }
    //--------------------------------------------------------------------------

    void GraphicsManager::LoadSettings(Settings& settings)
    {
        settings.StartLoadObject(SettingsEntryName);
        _type = GraphicsBackend::StringToBackendType(settings.GetString(APIStr, GraphicsBackend::DefaultAPIStr));
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