#include "Kmplete/Core/application.h"
#include "Kmplete/Core/filesystem.h"
#include "Kmplete/Core/settings.h"
#include "Kmplete/Core/log.h"

namespace Kmplete
{
    constexpr static auto ApplicationSettingsEntryName = "Application";

    Application::Application(const std::string& settingsFilePath, const std::string& defaultSettingsName)
        : _settingsManager(nullptr)
    {
        Initialize(settingsFilePath, defaultSettingsName);
    }
    //--------------------------------------------------------------------------

    Application::~Application()
    {
        Finalize();
    }
    //--------------------------------------------------------------------------

    void Application::Initialize(const std::string& settingsFilePath, const std::string& defaultSettingsName)
    {
        Log::InitializeTemporarySink();

        if (!Filesystem::Initialize())
        {
            throw std::exception("Application initialization failed");
        }

        _settingsManager.reset(new SettingsManager(settingsFilePath.empty() ? Filesystem::GetApplicationPath().append(defaultSettingsName) : settingsFilePath));
        if (!_settingsManager->Initialize())
        {
            Log::CoreWarn("Application: failed to load settings");
        }

        LoadSettings();

        Log::Initialize();
    }
    //--------------------------------------------------------------------------

    void Application::Finalize()
    {
        SaveSettings();

        _settingsManager->Finalize();
        Log::Finalize();
    }
    //--------------------------------------------------------------------------

    void Application::SaveSettings() const
    {
        auto settings = _settingsManager->PutSettings(ApplicationSettingsEntryName);
        if (!settings)
        {
            return;
        }

        settings->StartSaveObject();
        Log::SaveSettings(settings);
        settings->EndSaveObject();
    }
    //--------------------------------------------------------------------------

    void Application::LoadSettings()
    {
        const auto settings = _settingsManager->GetSettings(ApplicationSettingsEntryName);
        if (!settings)
        {
            return;
        }

        Log::LoadSettings(settings);
    }
    //--------------------------------------------------------------------------
}