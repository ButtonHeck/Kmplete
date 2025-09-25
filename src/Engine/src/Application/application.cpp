#include "Kmplete/Application/application.h"
#include "Kmplete/Log/log.h"
#include "Kmplete/Core/settings_document.h"
#include "Kmplete/Core/assertion.h"
#include "Kmplete/Utils/function_utils.h"
#include "Kmplete/Filesystem/filesystem.h"

#include <stdexcept>


namespace Kmplete
{
    constexpr static auto SettingsEntryName = "Application";

    Application::Application(const ApplicationParameters& parameters)
        : KMP_PROFILE_CONSTRUCTOR_START_BASE_CLASS("Application::Application(const ApplicationParameters&)")
          _applicationName(parameters.applicationName)
        , _running(false)
        , _systemMetricsManager(nullptr)
        , _localizationManager(nullptr)
        , _settingsManager(nullptr)
    {
        Initialize(parameters);

        KMP_PROFILE_CONSTRUCTOR_END()
    }
    //--------------------------------------------------------------------------

    Application::~Application()
    {
        KMP_PROFILE_FUNCTION();

        Finalize();
    }
    //--------------------------------------------------------------------------

    const String& Application::GetApplicationName() const noexcept
    {
        return _applicationName;
    }
    //--------------------------------------------------------------------------

    void Application::AddFrameListener(NonNull<ApplicationFrameListener*> frameListener)
    {
        _frameListeners.push_back(frameListener);
    }
    //--------------------------------------------------------------------------

    void Application::Initialize(const ApplicationParameters& parameters)
    {
        KMP_PROFILE_FUNCTION();

#if !defined (KMP_CONFIG_TYPE_PRODUCTION)
        {
            KMP_PROFILE_SCOPE("Application logger boot");
            Log::Boot(_applicationName);
        }
#endif

        const auto applicationPath = Filesystem::GetCurrentFilepath();
        if (!Filesystem::FilepathExists(applicationPath))
        {
            throw std::runtime_error("Application filepath initialization failed");
        }

        _systemMetricsManager = CreateUPtr<SystemMetricsManager>();
        KMP_ASSERT(_systemMetricsManager);

        _localizationManager = CreateUPtr<LocalizationManager>();
        KMP_ASSERT(_localizationManager);

        const auto defaultTranslationsPath = Filesystem::ToGenericU8String(applicationPath / LocalesDirectory);
        KMP_ASSERT(defaultTranslationsPath != LocalesDirectory);
        _localizationManager->AddMessagesPath(defaultTranslationsPath);
        _localizationManager->AddMessagesDomain(KMP_TR_DOMAIN_ENGINE);

        _settingsManager = CreateUPtr<SettingsManager>(parameters.settingsFilepath.empty()
            ? applicationPath / parameters.defaultSettingsFileName
            : parameters.settingsFilepath);
        KMP_ASSERT(_settingsManager);

        LoadSettings();

#if !defined (KMP_CONFIG_TYPE_PRODUCTION)
        {
            KMP_PROFILE_SCOPE("Application logger initialization");
            Log::Initialize(_applicationName);
        }
#endif

        FillDictionary();
        _localizationManager->AddLocaleChangedCallback(KMP_BIND(Application::FillDictionary));
    }
    //--------------------------------------------------------------------------

    void Application::Finalize()
    {
        KMP_PROFILE_FUNCTION();

        SaveSettings();

        _frameListeners.clear();

        _settingsManager.reset();
        _localizationManager.reset();
        _systemMetricsManager.reset();

#if !defined (KMP_CONFIG_TYPE_PRODUCTION)
        {
            KMP_PROFILE_SCOPE("Application logger finalization");
            Log::Finalize();
        }
#endif
    }
    //--------------------------------------------------------------------------

    void Application::SaveSettings() const
    {
        KMP_PROFILE_FUNCTION();

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
    }
    //--------------------------------------------------------------------------

    void Application::LoadSettings()
    {
        KMP_PROFILE_FUNCTION();

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
    }
    //--------------------------------------------------------------------------

    void Application::FillDictionary()
    {
        KMP_PROFILE_FUNCTION();

        _localizationManager->Translate(KMP_TR_DOMAIN_ENGINE, "English");
        _localizationManager->Translate(KMP_TR_DOMAIN_ENGINE, "Russian");
    }
    //--------------------------------------------------------------------------
}