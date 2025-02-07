#include "Kmplete/Application/application.h"
#include "Kmplete/Core/settings.h"
#include "Kmplete/Core/log.h"

#include <stdexcept>

namespace Kmplete
{
    constexpr static auto ApplicationSettingsEntryName = "Application";

    Application::Application(const ApplicationParameters& applicationParameters)
        : _systemMetricsManager(nullptr)
        , _localizationManager(nullptr)
        , _settingsManager(nullptr)
    {
        Initialize(applicationParameters);
    }
    //--------------------------------------------------------------------------

    Application::~Application()
    {
        Finalize();
    }
    //--------------------------------------------------------------------------

    void Application::SaveSettings(const Path& path) const
    {
        if (!path.empty())
        {
            _settingsManager->SetFilename(path);
        }

        SaveSettingsInternal();
    }
    //--------------------------------------------------------------------------

    void Application::LoadSettings(const Path& path)
    {
        if (!path.empty())
        {
            _settingsManager->SetFilename(path);
        }

        LoadSettingsInternal();
    }
    //--------------------------------------------------------------------------

    void Application::Initialize(const ApplicationParameters& applicationParameters)
    {
#if !defined (KMP_LOG_DISABLED) && !defined (KMP_PRODUCTION_BUILD)
        Log::Boot();
#endif

        if (!Filesystem::Initialize())
        {
            throw std::runtime_error("Application initialization failed");
        }

        _systemMetricsManager = CreateUPtr<SystemMetricsManager>();
        _localizationManager = CreatePtr<LocalizationManager>();
        _settingsManager = CreateUPtr<SettingsManager>(applicationParameters.settingsPath.empty() 
            ? Filesystem::GetApplicationPath().append(applicationParameters.defaultSettingsFileName) 
            : applicationParameters.settingsPath);

        LoadSettingsInternal();

#if !defined (KMP_LOG_DISABLED) && !defined (KMP_PRODUCTION_BUILD)
        Log::Initialize();
#endif

        _localizationManager->AddMessagesDomain(KMP_TR_DOMAIN_ENGINE);
    }
    //--------------------------------------------------------------------------

    void Application::Finalize()
    {
        SaveSettingsInternal();

        _settingsManager.reset();
        _localizationManager.reset();
        _systemMetricsManager.reset();

#if !defined (KMP_LOG_DISABLED) && !defined (KMP_PRODUCTION_BUILD)
        Log::Finalize();
#endif
    }
    //--------------------------------------------------------------------------

    void Application::SaveSettingsInternal() const
    {
        auto settings = _settingsManager->PutSettings(ApplicationSettingsEntryName);
        if (!settings)
        {
            KMP_LOG_CORE_WARN("Application: failed to create settings entry for saving");
            return;
        }

#if !defined (KMP_LOG_DISABLED) && !defined (KMP_PRODUCTION_BUILD)
        Log::SaveSettings(*settings);
#endif

        _localizationManager->SaveSettings(*settings);

        _settingsManager->SaveSettings();
    }
    //--------------------------------------------------------------------------

    void Application::LoadSettingsInternal()
    {
        _settingsManager->LoadSettings();

        const auto settings = _settingsManager->GetSettings(ApplicationSettingsEntryName);
        if (!settings)
        {
            KMP_LOG_CORE_WARN("Application: failed to get settings entry for loading");
            return;
        }

#if !defined (KMP_LOG_DISABLED) && !defined (KMP_PRODUCTION_BUILD)
        Log::LoadSettings(*settings);
#endif

        _localizationManager->LoadSettings(*settings);
    }
    //--------------------------------------------------------------------------
}