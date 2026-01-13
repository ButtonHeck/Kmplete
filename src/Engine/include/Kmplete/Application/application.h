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
    //! Parameters for Kmplete application creation
    //! @see Application
    struct ApplicationParameters
    {
        const String applicationName;
        const Filepath settingsFilepath;
        const String defaultSettingsFileName = "Kmplete_settings.json";
    };
    //--------------------------------------------------------------------------


    //! Base class of the Kmplete application, which is responsible for
    //! initialization, settings saving and loading of the subsystems
    //! that do not require window creation, additionally it handles logging
    //! initialization and its settings management
    class Application
    {
        KMP_LOG_CLASSNAME(Application)
        KMP_PROFILE_CONSTRUCTOR_DECLARE()
        KMP_DISABLE_COPY_MOVE(Application)

    public:
        KMP_API explicit Application(const ApplicationParameters& parameters);
        KMP_API virtual ~Application();

        KMP_NODISCARD KMP_API const String& GetApplicationName() const noexcept;

        KMP_API virtual void Run() = 0;

    private:
        void _Initialize(const ApplicationParameters& parameters);
        void _Finalize();

        void _SaveSettings() const;
        void _LoadSettings();

        void _FillDictionary();

    protected:
        const String _applicationName;
        const Filepath _applicationPath;
        bool _running;

        UPtr<SystemMetricsManager> _systemMetricsManager;
        UPtr<LocalizationManager> _localizationManager;
        UPtr<SettingsManager> _settingsManager;
    };
    //--------------------------------------------------------------------------


    //! Client application should define this function in its code, the function
    //! itself is called from main.h header file
    KMP_NODISCARD extern UPtr<Application> CreateApplication(const ProgramOptions& programOptions);
}