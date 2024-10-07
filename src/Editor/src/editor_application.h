#pragma once

#include "Kmplete/Core/pointers.h"
#include "Kmplete/Application/window_application.h"

namespace Kmplete
{
    class EditorApplication : public WindowApplication
    {
    public:
        KMP_DISABLE_COPY_MOVE(EditorApplication)

        EditorApplication(const std::string& settingsFilePath, const std::string& defaultSettingsName = "Kmplete_settings.json");
        virtual ~EditorApplication();

        KMP_NODISCARD std::string GetApplicationName() const KMP_NOEXCEPT override;

        void Run() override;

    protected:
        void OnEvent(Event& event) override;

        KMP_NODISCARD bool OnWindowCloseEvent(WindowCloseEvent& event) override;
        KMP_NODISCARD bool OnWindowFramebufferRefreshEvent(WindowFramebufferRefreshEvent& event) override;

        KMP_NODISCARD bool OnKeyPressEvent(KeyPressEvent& event) override;

    private:
        void Initialize();
        void Finalize();
        void SaveSettings() const;
        void LoadSettings();
    };
    //--------------------------------------------------------------------------
}