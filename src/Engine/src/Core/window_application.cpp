#include "Kmplete/Core/window_application.h"

namespace Kmplete
{
    constexpr static auto WindowSettingsEntryName = "Window";

    WindowApplication::WindowApplication()
        : _backend(nullptr)
        , _mainWindow(nullptr)
    {}
    //--------------------------------------------------------------------------

    bool WindowApplication::Initialize(const std::string& settingsFilePath)
    {
        if (!Application::Initialize(settingsFilePath))
        {
            return false;
        }

        _backend = WindowBackend::Create();
        if (!_backend || !_backend->Initialize())
        {
            return false;
        }

        _mainWindow = _backend->CreateWindow();
        if (!_mainWindow || !_mainWindow->Initialize())
        {
            return false;
        }

        LoadSettings();

        return true;
    }
    //--------------------------------------------------------------------------

    void WindowApplication::Finalize()
    {
        SaveSettings();

        _backend->Finalize();

        Application::Finalize();
    }
    //--------------------------------------------------------------------------

    void WindowApplication::SaveSettings() const
    {
        auto settings = _settingsManager->PutSettings(WindowSettingsEntryName);
        settings->StartSaveObject();
        _mainWindow->SaveSettings(settings);
        settings->EndSaveObject();
    }
    //--------------------------------------------------------------------------

    void WindowApplication::LoadSettings()
    {
        _mainWindow->LoadSettings(_settingsManager->GetSettings(WindowSettingsEntryName));
    }
    //--------------------------------------------------------------------------
}