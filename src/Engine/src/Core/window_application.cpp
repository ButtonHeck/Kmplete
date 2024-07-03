#include "Kmplete/Core/window_application.h"

namespace Kmplete
{
    WindowApplication::WindowApplication()
    {}
    //--------------------------------------------------------------------------

    bool WindowApplication::Initialize(const std::string& configPath)
    {
        if (!Application::Initialize(configPath))
        {
            return false;
        }

        return true;
    }
    //--------------------------------------------------------------------------
}