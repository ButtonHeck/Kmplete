#include "Kmplete/Core/window_application.h"

namespace Kmplete
{
    constexpr static auto WindowAppSettingsEntryName = "WindowApplication";

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

        _mainWindow = _backend->CreateMainWindow();
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

        _mainWindow.reset();
        _backend->Finalize();

        Application::Finalize();
    }
    //--------------------------------------------------------------------------

    void WindowApplication::SaveSettings() const
    {
        auto settings = _settingsManager->PutSettings(WindowAppSettingsEntryName);
        settings->StartSaveObject();
        _backend->SaveSettings(settings);
        settings->EndSaveObject();
    }
    //--------------------------------------------------------------------------

    void WindowApplication::LoadSettings()
    {
        _backend->LoadSettings(_settingsManager->GetSettings(WindowAppSettingsEntryName));
    }
    //--------------------------------------------------------------------------
}