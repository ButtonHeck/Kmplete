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

        return true;
    }
    //--------------------------------------------------------------------------

    void WindowApplication::SaveSettings() const
    {
        _mainWindow->SaveSettings(_settings);
    }
    //--------------------------------------------------------------------------

    void WindowApplication::LoadSettings()
    {
        _mainWindow->LoadSettings(_settings);
    }
    //--------------------------------------------------------------------------
}