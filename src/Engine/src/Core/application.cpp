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

        Log::CoreTrace("Application: created");
    }
    //--------------------------------------------------------------------------

    Application::~Application()
    {
        Finalize();

        Log::CoreTrace("Application: destroyed");
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

        LoadSettings();

        Log::Initialize();
    }
    //--------------------------------------------------------------------------

    void Application::Finalize()
    {
        SaveSettings();

        _settingsManager.reset();
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

        _settingsManager->SaveSettings();
    }
    //--------------------------------------------------------------------------

    void Application::LoadSettings()
    {
        _settingsManager->LoadSettings();

        const auto settings = _settingsManager->GetSettings(ApplicationSettingsEntryName);
        if (!settings)
        {
            return;
        }

        Log::LoadSettings(settings);
    }
    //--------------------------------------------------------------------------
}