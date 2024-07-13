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

        KMP_NODISCARD std::string GetApplicationName() const KMP_NOEXCEPT override;

        KMP_NODISCARD bool Initialize(const std::string& settingsPath) override;
        void Run() override;

    protected:
        void OnEvent(Event& event) override;

        KMP_NODISCARD bool OnWindowCloseEvent(WindowCloseEvent& event) override;
        KMP_NODISCARD bool OnWindowFramebufferRefreshEvent(WindowFramebufferRefreshEvent& event) override;

        KMP_NODISCARD bool OnKeyPressEvent(KeyPressEvent& event) override;

    private:
        void SaveSettings() const;
        void LoadSettings();
    };
    //--------------------------------------------------------------------------
}