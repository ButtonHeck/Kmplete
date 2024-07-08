#include "Kmplete/Core/window_application.h"

namespace Kmplete
{
    WindowApplication::WindowApplication()
    {}
    //--------------------------------------------------------------------------

    bool WindowApplication::Initialize(const std::string& settingsFilePath)
    {
        if (!Application::Initialize(settingsFilePath))
        {
            return false;
        }

        return true;
    }
    //--------------------------------------------------------------------------
}