#pragma once

#include "Kmplete/Core/kmplete_api.h"
#include "Kmplete/Core/pointers.h"
#include "Kmplete/Event/event.h"
#include "Kmplete/Event/key_event.h"

#include <string>

namespace Kmplete
{
    class Settings;

    class KMP_API Application
    {
    public:
        Application();

        virtual ~Application() = default;

        KMP_NODISCARD virtual std::string GetApplicationName() const = 0;

        KMP_NODISCARD virtual bool Initialize(const std::string& settingsFilePath);
        virtual void Run() = 0;

    protected:
        virtual void OnEvent(Event& event) {}

        KMP_NODISCARD virtual bool OnKeyPressEvent(KeyPressEvent& event) { return true; }
        KMP_NODISCARD virtual bool OnKeyReleaseEvent(KeyReleaseEvent& event) { return true; }
        KMP_NODISCARD virtual bool OnKeyCharEvent(KeyCharEvent& event) { return true; }

    protected:
        Ptr<Settings> _settings;
    };
    //--------------------------------------------------------------------------

    extern KMP_NODISCARD UPtr<Application> CreateApplication();
}