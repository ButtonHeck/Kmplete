#include "Kmplete/Application/application.h"
#include "Kmplete/Core/settings.h"
#include "Kmplete/Core/log.h"
#include "Kmplete/Core/assertion.h"
#include "Kmplete/Utils/function_utils.h"

#include <stdexcept>

namespace Kmplete
{
    constexpr static auto SettingsEntryName = "Application";

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
        KMP_ASSERT(_systemMetricsManager);

        _localizationManager = CreateUPtr<LocalizationManager>();
        KMP_ASSERT(_localizationManager);

        const auto defaultTranslationsPath = Filesystem::ToGenericU8String(Filesystem::GetApplicationPath().append(LocalesDirectory));
        KMP_ASSERT(defaultTranslationsPath != LocalesDirectory);
        _localizationManager->AddMessagesPath(defaultTranslationsPath);
        _localizationManager->AddMessagesDomain(KMP_TR_DOMAIN_ENGINE);

        _settingsManager = CreateUPtr<SettingsManager>(applicationParameters.settingsPath.empty() 
            ? Filesystem::GetApplicationPath().append(applicationParameters.defaultSettingsFileName) 
            : applicationParameters.settingsPath);
        KMP_ASSERT(_settingsManager);

        LoadSettingsInternal();

#if !defined (KMP_LOG_DISABLED) && !defined (KMP_PRODUCTION_BUILD)
        Log::Initialize();
#endif

        FillDictionary();
        _localizationManager->AddLocaleChangedCallback(KMP_BIND(Application::FillDictionary));
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
        auto settings = _settingsManager->PutSettings(SettingsEntryName);
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

        const auto settings = _settingsManager->GetSettings(SettingsEntryName);
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

    void Application::FillDictionary()
    {
        _localizationManager->Translate(KMP_TR_DOMAIN_ENGINE, "English");
        _localizationManager->Translate(KMP_TR_DOMAIN_ENGINE, "Russian");
    }
    //--------------------------------------------------------------------------
}