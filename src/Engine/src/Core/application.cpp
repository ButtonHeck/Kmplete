#include "Kmplete/Core/application.h"
#include "Kmplete/Core/filesystem.h"
#include "Kmplete/Core/settings.h"

namespace Kmplete
{
    Application::Application()
    {}
    //--------------------------------------------------------------------------

    bool Application::Initialize(const std::string& configPath)
    {
        Filesystem::Initialize();

        _settings.reset(new Settings(GetApplicationName()));

        return true;
    }
    //--------------------------------------------------------------------------
}