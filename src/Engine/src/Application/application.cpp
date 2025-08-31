#include "Kmplete/Application/application.h"
#include "Kmplete/Core/settings.h"
#include "Kmplete/Core/assertion.h"
#include "Kmplete/Utils/function_utils.h"
#include "Kmplete/Filesystem/filesystem.h"

#include <stdexcept>

namespace Kmplete
{
    constexpr static auto SettingsEntryName = "Application";

    Application::Application(const ApplicationParameters& applicationParameters)
        : KMP_PROFILE_CONSTRUCTOR_START_BASE_CLASS("Application::Application(const ApplicationParameters&)")
          _applicationName(applicationParameters.applicationName)
        , _systemMetricsManager(nullptr)
        , _localizationManager(nullptr)
        , _settingsManager(nullptr)
    {
        Initialize(applicationParameters);

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

    void Application::SaveSettings(const Filepath& filepath /*= Filepath()*/) const
    {
        KMP_PROFILE_FUNCTION();

        if (!filepath.empty())
        {
            _settingsManager->SetFilepath(filepath);
        }

        SaveSettingsInternal();
    }
    //--------------------------------------------------------------------------

    void Application::LoadSettings(const Filepath& filepath /*= Filepath()*/)
    {
        KMP_PROFILE_FUNCTION();

        if (!filepath.empty())
        {
            _settingsManager->SetFilepath(filepath);
        }

        LoadSettingsInternal();
    }
    //--------------------------------------------------------------------------

    void Application::Initialize(const ApplicationParameters& applicationParameters)
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

        _settingsManager = CreateUPtr<SettingsManager>(applicationParameters.settingsFilepath.empty() 
            ? applicationPath / applicationParameters.defaultSettingsFileName
            : applicationParameters.settingsFilepath);
        KMP_ASSERT(_settingsManager);

        LoadSettingsInternal();

#if !defined (KMP_CONFIG_TYPE_PRODUCTION)
        {
            KMP_PROFILE_SCOPE("Application logger initialization");
            Log::Initialize(_logSettings, _applicationName);
        }
#endif

        FillDictionary();
        _localizationManager->AddLocaleChangedCallback(KMP_BIND(Application::FillDictionary));
    }
    //--------------------------------------------------------------------------

    void Application::Finalize()
    {
        KMP_PROFILE_FUNCTION();

        SaveSettingsInternal();

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

    void Application::SaveSettingsInternal() const
    {
        KMP_PROFILE_FUNCTION();

        auto settings = _settingsManager->PutSettings(SettingsEntryName);
        if (!settings)
        {
            KMP_LOG_WARN("failed to create settings entry for saving");
            return;
        }

#if !defined (KMP_CONFIG_TYPE_PRODUCTION)
        settings->StartSaveObject(SettingsEntryName);
        settings->SaveString(Log::FilenameStr, Utils::NarrowToUtf8(_logSettings.filename));
        settings->SaveBool(Log::EnabledStr, _logSettings.enabled);
        settings->SaveBool(Log::TruncateStr, _logSettings.truncate);
        settings->SaveBool(Log::OutputConsoleStr, _logSettings.outputConsole);
        settings->SaveBool(Log::OutputFileStr, _logSettings.outputFile);
        settings->SaveBool(Log::OutputStringBufferStr, _logSettings.outputStringBuffer);
        settings->SaveInt(Log::LevelStr, _logSettings.level);
        settings->SaveInt(Log::LevelFlushStr, _logSettings.levelFlush);
        settings->EndSaveObject();
#endif

        _localizationManager->SaveSettings(*settings);

        _settingsManager->SaveSettings();
    }
    //--------------------------------------------------------------------------

    void Application::LoadSettingsInternal()
    {
        KMP_PROFILE_FUNCTION();

        _settingsManager->LoadSettings();

        const auto settings = _settingsManager->GetSettings(SettingsEntryName);
        if (!settings)
        {
            KMP_LOG_WARN("failed to get settings entry for loading");
            return;
        }

#if !defined (KMP_CONFIG_TYPE_PRODUCTION)
        settings->StartLoadObject(SettingsEntryName);
        _logSettings.filename = Utils::Utf8ToNarrow(settings->GetString(Log::FilenameStr, "Kmplete_log.txt"));
        _logSettings.enabled = settings->GetBool(Log::EnabledStr, true);
        _logSettings.truncate = settings->GetBool(Log::TruncateStr, false);
        _logSettings.outputConsole = settings->GetBool(Log::OutputConsoleStr, true);
        _logSettings.outputFile = settings->GetBool(Log::OutputFileStr, true);
        _logSettings.outputStringBuffer = settings->GetBool(Log::OutputStringBufferStr, false);
        _logSettings.level = settings->GetInt(Log::LevelStr, spdlog::level::trace);
        _logSettings.levelFlush = settings->GetInt(Log::LevelFlushStr, spdlog::level::trace);
        settings->EndLoadObject();
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