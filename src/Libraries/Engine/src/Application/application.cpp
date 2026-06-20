#include "Kmplete/Application/application.h"
#include "Kmplete/Application/application_context.h"
#include "Kmplete/Log/log.h"
#include "Kmplete/Core/settings_document.h"
#include "Kmplete/Core/assertion.h"
#include "Kmplete/Utils/function_utils.h"
#include "Kmplete/Filesystem/filesystem.h"
#include "Kmplete/Base/exception.h"
#include "Kmplete/Profile/profiler.h"


namespace Kmplete
{
    static constexpr auto SettingsEntryName = "Application";


    Application::Application(const ApplicationParameters& parameters)
        : KMP_PROFILE_CONSTRUCTOR_START_BASE_CLASS()
          _running(false)
        , _systemMetricsManager(nullptr)
        , _localizationManager(nullptr)
        , _settingsManager(nullptr)
    {
        _Initialize(parameters);

        KMP_PROFILE_CONSTRUCTOR_END()
    }
    //--------------------------------------------------------------------------

    Application::~Application() KMP_PROFILING(ProfileLevelAlways)
    {
        _Finalize();
    }}
    //--------------------------------------------------------------------------

    void Application::_Initialize(const ApplicationParameters& parameters)
    {
        ApplicationContext::_Initialize(parameters);

        const auto& applicationName = ApplicationContext::GetApplicationName();
        const auto& applicationPath = ApplicationContext::GetApplicationPath();
        const auto& applicationDataPath = ApplicationContext::GetApplicationDataPath();

#if !defined (KMP_CONFIG_TYPE_PRODUCTION)
        {
            KMP_PROFILE_SCOPE("Application logger boot", ProfileLevelAlways);
            Log::Boot(applicationName);
        }
#endif

        if (!Filesystem::FilepathExists(applicationPath))
        {
            throw RuntimeError("Application filepath initialization failed");
        }
        if (!Filesystem::FilepathExists(applicationDataPath))
        {
            throw RuntimeError("Application data filepath initialization failed");
        }

        _systemMetricsManager = CreateUPtr<SystemMetricsManager>();
        KMP_ASSERT(_systemMetricsManager);

        _localizationManager = CreateUPtr<LocalizationManager>();
        KMP_ASSERT(_localizationManager);

        const auto defaultTranslationsPath = Filesystem::ToGenericU8String(applicationPath / LocalesDirectory);
        KMP_ASSERT(defaultTranslationsPath != LocalesDirectory);
        _localizationManager->AddMessagesPath(defaultTranslationsPath);
        const auto engineDomainAdded = _localizationManager->AddMessagesDomain(KMP_TR_DOMAIN_ENGINE);
        KMP_ASSERT(engineDomainAdded);

        _settingsManager = CreateUPtr<SettingsManager>(parameters.settingsFilepath.empty()
            ? applicationPath / parameters.defaultSettingsFileName
            : parameters.settingsFilepath);
        KMP_ASSERT(_settingsManager);

        _LoadSettings();

#if !defined (KMP_CONFIG_TYPE_PRODUCTION)
        {
            KMP_PROFILE_SCOPE("Application logger initialization", ProfileLevelAlways);
            Log::Initialize(applicationName);
        }
#endif

        _FillDictionary();
        _localizationManager->AddLocaleChangedCallback(KMP_BIND(Application::_FillDictionary));
    }
    //--------------------------------------------------------------------------

    void Application::_Finalize()
    {
        KMP_ASSERT(_settingsManager && _localizationManager && _systemMetricsManager);

        _SaveSettings();

        _settingsManager.reset();
        _localizationManager.reset();
        _systemMetricsManager.reset();

#if !defined (KMP_CONFIG_TYPE_PRODUCTION)
        {
            KMP_PROFILE_SCOPE("Application logger finalization", ProfileLevelAlways);
            Log::Finalize();
        }
#endif
    }
    //--------------------------------------------------------------------------

    void Application::_SaveSettings() const KMP_PROFILING(ProfileLevelImportant)
    {
        KMP_ASSERT(_settingsManager && _localizationManager);

        auto settings = _settingsManager->PutSettingsDocument(SettingsEntryName);
        if (!settings)
        {
            KMP_LOG_WARN("failed to create settings entry for saving");
            return;
        }

#if !defined (KMP_CONFIG_TYPE_PRODUCTION)
        const auto& logSettings = Log::GetSettings();
        settings->get().StartSaveObject(SettingsEntryName);
        settings->get().SaveString(Log::FilenameStr, Utils::NarrowToUtf8(logSettings.filename));
        settings->get().SaveBool(Log::EnabledStr, logSettings.enabled);
        settings->get().SaveBool(Log::TruncateStr, logSettings.truncate);
        settings->get().SaveBool(Log::OutputConsoleStr, logSettings.outputConsole);
        settings->get().SaveBool(Log::OutputFileStr, logSettings.outputFile);
        settings->get().SaveBool(Log::OutputStringBufferStr, logSettings.outputStringBuffer);
        settings->get().SaveInt(Log::LevelStr, logSettings.level);
        settings->get().SaveInt(Log::LevelFlushStr, logSettings.levelFlush);
        settings->get().EndSaveObject();
#endif

        _localizationManager->SaveSettings(*settings);

        _settingsManager->SaveSettings();
    }}
    //--------------------------------------------------------------------------

    void Application::_LoadSettings() KMP_PROFILING(ProfileLevelImportant)
    {
        KMP_ASSERT(_settingsManager && _localizationManager);

        _settingsManager->LoadSettings();

        const auto settings = _settingsManager->GetSettingsDocument(SettingsEntryName);
        if (!settings)
        {
            KMP_LOG_WARN("failed to get settings entry for loading");
            return;
        }

#if !defined (KMP_CONFIG_TYPE_PRODUCTION)
        settings->get().StartLoadObject(SettingsEntryName);

        Log::LogSettings logSettings;
        logSettings.filename = Utils::Utf8ToNarrow(settings->get().GetString(Log::FilenameStr, "Kmplete_log.txt"));
        logSettings.enabled = settings->get().GetBool(Log::EnabledStr, true);
        logSettings.truncate = settings->get().GetBool(Log::TruncateStr, false);
        logSettings.outputConsole = settings->get().GetBool(Log::OutputConsoleStr, true);
        logSettings.outputFile = settings->get().GetBool(Log::OutputFileStr, true);
        logSettings.outputStringBuffer = settings->get().GetBool(Log::OutputStringBufferStr, false);
        logSettings.level = settings->get().GetInt(Log::LevelStr, spdlog::level::trace);
        logSettings.levelFlush = settings->get().GetInt(Log::LevelFlushStr, spdlog::level::trace);
        Log::SetSettings(logSettings);

        settings->get().EndLoadObject();
#endif

        _localizationManager->LoadSettings(*settings);
    }}
    //--------------------------------------------------------------------------

    void Application::_FillDictionary() KMP_PROFILING(ProfileLevelMinor)
    {
        KMP_ASSERT(_localizationManager);

        _localizationManager->Translate(KMP_TR_DOMAIN_ENGINE, "English");
        _localizationManager->Translate(KMP_TR_DOMAIN_ENGINE, "Russian");
    }}
    //--------------------------------------------------------------------------
}