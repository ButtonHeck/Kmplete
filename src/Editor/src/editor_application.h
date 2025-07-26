#pragma once

#include "editor_ui.h"
#include "Kmplete/Base/pointers.h"
#include "Kmplete/Application/window_application.h"
#include "Kmplete/Profile/profiler.h"

namespace Kmplete
{
    class EditorApplication : public WindowApplication
    {
        KMP_PROFILE_CONSTRUCTOR_DECLARE()

    public:
        KMP_DISABLE_COPY_MOVE(EditorApplication)

        explicit EditorApplication(const ApplicationParameters& applicationParameters);
        virtual ~EditorApplication();

        void Run() override;

        void SaveSettings(const Filepath& filepath = Filepath()) const override;
        void LoadSettings(const Filepath& filepath = Filepath()) override;

    protected:
        void OnEvent(Event& event) override;

        KMP_NODISCARD bool OnWindowCloseEvent(WindowCloseEvent& event) override;
        KMP_NODISCARD bool OnWindowFramebufferRefreshEvent(WindowFramebufferRefreshEvent& event) override;
        KMP_NODISCARD bool OnWindowContentScaleEvent(WindowContentScaleEvent& event) override;

        KMP_NODISCARD bool OnKeyPressEvent(KeyPressEvent& event) override;

    private:
        void Initialize();
        void Finalize();
        void SaveSettingsInternal() const;
        void LoadSettingsInternal();

    private:
        UPtr<EditorUI> _ui;
    };
    //--------------------------------------------------------------------------
}