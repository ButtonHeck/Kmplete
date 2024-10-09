#pragma once

#include "Kmplete/Core/kmplete_api.h"
#include "Kmplete/Core/pointers.h"
#include "Kmplete/Core/settings_manager.h"
#include "Kmplete/Event/event.h"
#include "Kmplete/Event/key_event.h"

#include <string>
#include <filesystem>

namespace Kmplete
{
    class Application
    {
    public:
        KMP_DISABLE_COPY_MOVE(Application)

        KMP_API Application(const std::string& settingsFilePath, const std::string& defaultSettingsName = "Kmplete_settings.json");
        KMP_API virtual ~Application();

        KMP_NODISCARD KMP_API virtual std::string GetApplicationName() const KMP_NOEXCEPT = 0;

        KMP_API virtual void Run() = 0;

        KMP_API virtual void SaveSettings(const std::filesystem::path& path = std::filesystem::path()) const;
        KMP_API virtual void LoadSettings(const std::filesystem::path& path = std::filesystem::path());

    protected:
        virtual void OnEvent(Event&) {}

        KMP_NODISCARD virtual bool OnKeyPressEvent(KeyPressEvent&) { return true; }
        KMP_NODISCARD virtual bool OnKeyReleaseEvent(KeyReleaseEvent&) { return true; }
        KMP_NODISCARD virtual bool OnKeyCharEvent(KeyCharEvent&) { return true; }

    private:
        void Initialize(const std::string& settingsFilePath, const std::string& defaultSettingsName);
        void Finalize();
        void SaveSettingsInternal() const;
        void LoadSettingsInternal();

    protected:
        UPtr<SettingsManager> _settingsManager;
    };
    //--------------------------------------------------------------------------

    KMP_NODISCARD extern UPtr<Application> CreateApplication(const std::string& settingsFilePath);
}