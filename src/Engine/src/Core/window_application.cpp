#include "Kmplete/Core/window_application.h"

namespace Kmplete
{
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

        Application::Finalize();
    }
    //--------------------------------------------------------------------------

    void WindowApplication::SaveSettings() const
    {
        auto settings = _settingsManager->PutSettings("Window");
        settings->StartSave();
        _mainWindow->SaveSettings(settings);
        settings->EndSave();
    }
    //--------------------------------------------------------------------------

    void WindowApplication::LoadSettings()
    {
        _mainWindow->LoadSettings(_settingsManager->GetSettings("Window"));
    }
    //--------------------------------------------------------------------------
}