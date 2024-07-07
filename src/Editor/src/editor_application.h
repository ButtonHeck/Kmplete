#pragma once

#include "Kmplete/Core/pointers.h"
#include "Kmplete/Core/window_application.h"

namespace Kmplete
{
    class EditorApplication : public WindowApplication
    {
    public:
        EditorApplication();
        ~EditorApplication();

        std::string GetApplicationName() const override;

        bool Initialize(const std::string& settingsPath) override;
        void Run() override;

    protected:
        void OnEvent(Event& event) override;

        bool OnWindowCloseEvent(WindowCloseEvent& event) override;
        bool OnWindowFramebufferRefreshEvent(WindowFramebufferRefreshEvent& event) override;

        bool OnKeyPressEvent(KeyPressEvent& event) override;

    private:
        void SaveSettings() const;
        void LoadSettings();
    };
    //--------------------------------------------------------------------------
}