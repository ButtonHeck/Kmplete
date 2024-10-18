#include "Kmplete/Application/application.h"
#include "Kmplete/Core/filesystem.h"
#include "Kmplete/Core/settings.h"
#include "Kmplete/Core/log.h"

#include <stdexcept>

namespace Kmplete
{
    constexpr static auto ApplicationSettingsEntryName = "Application";

    Application::Application(const std::string& settingsFilePath, const std::string& defaultSettingsName)
        : _settingsManager(nullptr)
        , _systemMetricsManager(nullptr)
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

    void Application::SaveSettings(const std::filesystem::path& path) const
    {
        if (!path.empty())
        {
            _settingsManager->SetFilename(path);
        }

        SaveSettingsInternal();
    }
    //--------------------------------------------------------------------------

    void Application::LoadSettings(const std::filesystem::path& path)
    {
        if (!path.empty())
        {
            _settingsManager->SetFilename(path);
        }

        LoadSettingsInternal();
    }
    //--------------------------------------------------------------------------

    void Application::Initialize(const std::string& settingsFilePath, const std::string& defaultSettingsName)
    {
        Log::InitializeTemporarySink();

        _systemMetricsManager.reset(new SystemMetricsManager());

        if (!Filesystem::Initialize())
        {
            throw std::runtime_error("Application initialization failed");
        }

        _settingsManager.reset(new SettingsManager(settingsFilePath.empty() ? Filesystem::GetApplicationPath().append(defaultSettingsName) : settingsFilePath));

        LoadSettingsInternal();

        Log::Initialize();
    }
    //--------------------------------------------------------------------------

    void Application::Finalize()
    {
        SaveSettingsInternal();

        _settingsManager.reset();
        Log::Finalize();
    }
    //--------------------------------------------------------------------------

    void Application::SaveSettingsInternal() const
    {
        auto settings = _settingsManager->PutSettings(ApplicationSettingsEntryName);
        if (!settings)
        {
            Log::CoreWarn("Application: failed to create settings entry for saving");
            return;
        }

        Log::SaveSettings(settings);

        _settingsManager->SaveSettings();
    }
    //--------------------------------------------------------------------------

    void Application::LoadSettingsInternal()
    {
        _settingsManager->LoadSettings();

        const auto settings = _settingsManager->GetSettings(ApplicationSettingsEntryName);
        if (!settings)
        {
            Log::CoreWarn("Application: failed to get settings entry for loading");
            return;
        }

        Log::LoadSettings(settings);
    }
    //--------------------------------------------------------------------------
}