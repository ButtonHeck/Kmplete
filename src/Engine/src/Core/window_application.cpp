#include "Kmplete/Core/window_application.h"

namespace Kmplete
{
    constexpr static auto WindowAppSettingsEntryName = "WindowApplication";

    WindowApplication::WindowApplication()
        : _backend(WindowBackend::Create())
        , _mainWindow(nullptr)
    {}
    //--------------------------------------------------------------------------

    bool WindowApplication::Initialize(const std::string& settingsFilePath, const std::string& defaultSettingsName)
    {
        if (!Application::Initialize(settingsFilePath, defaultSettingsName))
        {
            return false;
        }

        LoadSettings();

        if (!_backend || !_backend->Initialize())
        {
            return false;
        }

        _mainWindow = _backend->GetMainWindow();
        if (!_mainWindow)
        {
            return false;
        }

        return true;
    }
    //--------------------------------------------------------------------------

    void WindowApplication::Finalize()
    {
        SaveSettings();

        _mainWindow.reset();
        _backend->Finalize();

        Application::Finalize();
    }
    //--------------------------------------------------------------------------

    void WindowApplication::SaveSettings() const
    {
        auto settings = _settingsManager->PutSettings(WindowAppSettingsEntryName);
        if (!settings)
        {
            return;
        }

        settings->StartSaveObject();
        _backend->SaveSettings(settings);
        settings->EndSaveObject();
    }
    //--------------------------------------------------------------------------

    void WindowApplication::LoadSettings() const
    {
        const auto settings = _settingsManager->GetSettings(WindowAppSettingsEntryName);
        if (!settings)
        {
            return;
        }

        _backend->LoadSettings(settings);
    }
    //--------------------------------------------------------------------------
}