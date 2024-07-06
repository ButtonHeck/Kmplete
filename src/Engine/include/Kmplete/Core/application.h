#pragma once

#include "kmplete_export.h"
#include "Kmplete/Event/event.h"
#include "Kmplete/Event/key_event.h"

#include <string>

namespace Kmplete
{
    class KMPLETE_API Application
    {
    public:
        Application();

        virtual ~Application() = default;

        virtual std::string GetApplicationName() const = 0;

        virtual bool Initialize(const std::string& configPath);
        virtual void Run() = 0;

    protected:
        virtual void OnEvent(Event& event) {}

        virtual bool OnKeyPressEvent(KeyPressEvent& event) { return true; }
        virtual bool OnKeyReleaseEvent(KeyReleaseEvent& event) { return true; }
        virtual bool OnKeyCharEvent(KeyCharEvent& event) { return true; }
    };
    //--------------------------------------------------------------------------

    extern Application* CreateApplication();
}