#include "Kmplete/Core/application.h"
#include "Kmplete/Core/filesystem.h"
#include "Kmplete/Core/settings.h"
#include "Kmplete/Core/log.h"

namespace Kmplete
{
    Application::Application()
    {}
    //--------------------------------------------------------------------------

    bool Application::Initialize(const std::string& settingsPath)
    {
        Filesystem::Initialize();
        Log::InitializeTemporarySink();

        _settings.reset(new Settings(settingsPath.empty() ? Filesystem::GetCurrentPath().append("Kmplete_settings.json") : settingsPath));
        if (!_settings->Initialize())
        {
            return false;
        }

        LogSettings logSettings;
        logSettings.LoadSettings(_settings);

        Log::Initialize(logSettings);

        return true;
    }
    //--------------------------------------------------------------------------
}