#include "Kmplete/Core/application.h"
#include "Kmplete/Core/filesystem.h"
#include "Kmplete/Core/settings.h"
#include "Kmplete/Core/log.h"

namespace Kmplete
{
    Application::Application()
    {}
    //--------------------------------------------------------------------------

    bool Application::Initialize(const std::string& settingsFilePath)
    {
        Log::InitializeTemporarySink();

        if (!Filesystem::Initialize())
        {
            return false;
        }

        _settings.reset(new Settings(settingsFilePath.empty() ? Filesystem::GetApplicationPath().append("Kmplete_settings.json") : settingsFilePath));
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