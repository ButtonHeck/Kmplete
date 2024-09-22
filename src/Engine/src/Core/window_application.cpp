#include "Kmplete/Core/window_application.h"

namespace Kmplete
{
    constexpr static auto WindowAppSettingsEntryName = "WindowApplication";

    WindowApplication::WindowApplication(const std::string& settingsFilePath, const std::string& defaultSettingsName)
        : Application(settingsFilePath, defaultSettingsName)
        , _backend(WindowBackend::Create())
        , _mainWindow(nullptr)
    {
        Initialize();
    }
    //--------------------------------------------------------------------------

    WindowApplication::~WindowApplication()
    {
        Finalize();
    }
    //--------------------------------------------------------------------------

    void WindowApplication::Initialize()
    {
        LoadSettings();

        if (!_backend || !_backend->Initialize())
        {
            throw std::exception("WindowApplication backend initialization failed");
        }

        _mainWindow = _backend->GetMainWindow();
        if (!_mainWindow)
        {
            throw std::exception("WindowApplication main window initialization failed");
        }
    }
    //--------------------------------------------------------------------------

    void WindowApplication::Finalize()
    {
        SaveSettings();

        _mainWindow.reset();
        _backend->Finalize();
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