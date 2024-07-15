#include "Kmplete/Core/application.h"
#include "Kmplete/Core/filesystem.h"
#include "Kmplete/Core/settings.h"
#include "Kmplete/Core/log.h"

namespace Kmplete
{
    Application::Application()
        : _settings(nullptr)
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

        Log::Initialize(_settings);

        return true;
    }
    //--------------------------------------------------------------------------

    bool Application::Finalize()
    {
        _settings->StartSave();
        Log::Finalize(_settings);
        SaveSettings();
        _settings->EndSave();

        return true;
    }
    //--------------------------------------------------------------------------
}