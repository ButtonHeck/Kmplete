#include "Kmplete/Core/window_application.h"

namespace Kmplete
{
    WindowApplication::WindowApplication()
        : _window(nullptr)
    {}
    //--------------------------------------------------------------------------

    bool WindowApplication::Initialize(const std::string& settingsFilePath)
    {
        if (!Application::Initialize(settingsFilePath))
        {
            return false;
        }

        _window.reset(Window::Create());
        if (!_window || !_window->Initialize())
        {
            return false;
        }

        return true;
    }
    //--------------------------------------------------------------------------
}