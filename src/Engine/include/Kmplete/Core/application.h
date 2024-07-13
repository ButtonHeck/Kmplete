#pragma once

#include "Kmplete/Core/kmplete_api.h"
#include "Kmplete/Core/pointers.h"
#include "Kmplete/Event/event.h"
#include "Kmplete/Event/key_event.h"

#include <string>

namespace Kmplete
{
    class Settings;

    class Application
    {
    public:
        KMP_API Application();

        KMP_API virtual ~Application() = default;

        KMP_API KMP_NODISCARD virtual std::string GetApplicationName() const KMP_NOEXCEPT = 0;

        KMP_API KMP_NODISCARD virtual bool Initialize(const std::string& settingsFilePath);
        KMP_API virtual void Run() = 0;

    protected:
        virtual void OnEvent(Event&) {}

        KMP_NODISCARD virtual bool OnKeyPressEvent(KeyPressEvent&) { return true; }
        KMP_NODISCARD virtual bool OnKeyReleaseEvent(KeyReleaseEvent&) { return true; }
        KMP_NODISCARD virtual bool OnKeyCharEvent(KeyCharEvent&) { return true; }

    protected:
        Ptr<Settings> _settings;
    };
    //--------------------------------------------------------------------------

    extern KMP_NODISCARD UPtr<Application> CreateApplication();
}