#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/pointers.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Log/log_class_macro.h"
#include "Kmplete/Core/program_options.h"
#include "Kmplete/Core/settings_manager.h"
#include "Kmplete/Core/system_metrics_manager.h"
#include "Kmplete/Localization/localization_manager.h"
#include "Kmplete/Profile/profiler.h"


namespace Kmplete
{
    struct ApplicationParameters
    {
        const String applicationName;
        const Filepath settingsFilepath;
        const String defaultSettingsFileName = "Kmplete_settings.json";
    };
    //--------------------------------------------------------------------------

    class Application
    {
        KMP_LOG_CLASSNAME(Application)
        KMP_PROFILE_CONSTRUCTOR_DECLARE()

    public:
        KMP_DISABLE_COPY_MOVE(Application)

        KMP_API explicit Application(const ApplicationParameters& parameters);
        KMP_API virtual ~Application();

        KMP_NODISCARD KMP_API const String& GetApplicationName() const noexcept;

        KMP_API virtual void Run() = 0;

    private:
        void Initialize(const ApplicationParameters& parameters);
        void Finalize();

        void SaveSettings() const;
        void LoadSettings();

        void FillDictionary();

    protected:
        const String _applicationName;
        bool _running;

        UPtr<SystemMetricsManager> _systemMetricsManager;
        UPtr<LocalizationManager> _localizationManager;
        UPtr<SettingsManager> _settingsManager;
    };
    //--------------------------------------------------------------------------

    KMP_NODISCARD extern UPtr<Application> CreateApplication(const ProgramOptions& programOptions);
}