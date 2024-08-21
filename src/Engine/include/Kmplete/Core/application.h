#pragma once

#include "Kmplete/Core/kmplete_api.h"
#include "Kmplete/Core/pointers.h"
#include "Kmplete/Core/settings_manager.h"
#include "Kmplete/Event/event.h"
#include "Kmplete/Event/key_event.h"

#include <string>

namespace Kmplete
{
    class Application
    {
    public:
        KMP_DISABLE_COPY_MOVE(Application)

        KMP_API Application();
        KMP_API virtual ~Application() = default;

        KMP_NODISCARD KMP_API virtual std::string GetApplicationName() const KMP_NOEXCEPT = 0;

        KMP_NODISCARD KMP_API virtual bool Initialize(const std::string& settingsFilePath);
        KMP_API virtual void Finalize();

        KMP_API virtual void Run() = 0;

    protected:
        virtual void OnEvent(Event&) {}

        KMP_NODISCARD virtual bool OnKeyPressEvent(KeyPressEvent&) { return true; }
        KMP_NODISCARD virtual bool OnKeyReleaseEvent(KeyReleaseEvent&) { return true; }
        KMP_NODISCARD virtual bool OnKeyCharEvent(KeyCharEvent&) { return true; }

        void SaveSettings() const;
        void LoadSettings();

    protected:
        UPtr<SettingsManager> _settingsManager;
    };
    //--------------------------------------------------------------------------

    KMP_NODISCARD extern UPtr<Application> CreateApplication();
}