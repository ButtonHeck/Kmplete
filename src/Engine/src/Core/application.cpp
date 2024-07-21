#include "Kmplete/Core/application.h"
#include "Kmplete/Core/filesystem.h"
#include "Kmplete/Core/settings.h"
#include "Kmplete/Core/log.h"

namespace Kmplete
{
    Application::Application()
        : _settingsManager(nullptr)
    {}
    //--------------------------------------------------------------------------

    bool Application::Initialize(const std::string& settingsFilePath)
    {
        Log::InitializeTemporarySink();

        if (!Filesystem::Initialize())
        {
            return false;
        }

        _settingsManager.reset(new SettingsManager(settingsFilePath.empty() ? Filesystem::GetApplicationPath().append("Kmplete_settings.json") : settingsFilePath));
        if (!_settingsManager->Initialize())
        {
            Log::CoreWarn("Application: failed to load settings");
        }

        Log::Initialize(_settingsManager);

        return true;
    }
    //--------------------------------------------------------------------------

    bool Application::Finalize()
    {
        Log::SaveSettings(_settingsManager);
        SaveSettings();

        _settingsManager->Finalize();

        Log::Finalize();

        return true;
    }
    //--------------------------------------------------------------------------
}