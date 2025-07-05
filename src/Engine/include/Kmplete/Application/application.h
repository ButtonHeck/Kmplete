#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/pointers.h"
#include "Kmplete/Base/types.h"
#include "Kmplete/Log/log.h"
#include "Kmplete/Core/program_options.h"
#include "Kmplete/Core/settings_manager.h"
#include "Kmplete/Core/system_metrics_manager.h"
#include "Kmplete/Localization/localization_manager.h"
#include "Kmplete/Event/event.h"
#include "Kmplete/Event/key_event.h"

namespace Kmplete
{
    struct ApplicationParameters
    {
        const String applicationName;
        const Path settingsPath;
        const String defaultSettingsFileName = "Kmplete_settings.json";
    };
    //--------------------------------------------------------------------------

    class Application
    {
    public:
        KMP_DISABLE_COPY_MOVE(Application)

        KMP_API explicit Application(const ApplicationParameters& applicationParameters);
        KMP_API virtual ~Application();

        KMP_NODISCARD KMP_API const String& GetApplicationName() const noexcept;

        KMP_API virtual void Run() = 0;

        KMP_API virtual void SaveSettings(const Path& path = Path()) const;
        KMP_API virtual void LoadSettings(const Path& path = Path());

    protected:
        virtual void OnEvent(Event&) {}

        KMP_NODISCARD virtual bool OnKeyPressEvent(KeyPressEvent&) { return true; }
        KMP_NODISCARD virtual bool OnKeyReleaseEvent(KeyReleaseEvent&) { return true; }
        KMP_NODISCARD virtual bool OnKeyCharEvent(KeyCharEvent&) { return true; }

    private:
        void Initialize(const ApplicationParameters& applicationParameters);
        void Finalize();
        void SaveSettingsInternal() const;
        void LoadSettingsInternal();

        void FillDictionary();

    protected:
        const String _applicationName;

        UPtr<SystemMetricsManager> _systemMetricsManager;
        UPtr<LocalizationManager> _localizationManager;
        UPtr<SettingsManager> _settingsManager;

#if !defined (KMP_PRODUCTION_BUILD)
        Log::LogSettings _logSettings;
#endif
    };
    //--------------------------------------------------------------------------

    KMP_NODISCARD extern UPtr<Application> CreateApplication(const ProgramOptions& programOptions);
}