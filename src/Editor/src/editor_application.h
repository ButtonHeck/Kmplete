#pragma once

#include "editor_ui.h"
#include "Kmplete/Core/pointers.h"
#include "Kmplete/Application/window_application.h"

namespace Kmplete
{
    class EditorApplication : public WindowApplication
    {
    public:
        KMP_DISABLE_COPY_MOVE(EditorApplication)

        explicit EditorApplication(const ApplicationParameters& applicationParameters);
        virtual ~EditorApplication();

        KMP_NODISCARD std::string GetApplicationName() const KMP_NOEXCEPT override;

        void Run() override;

        void SaveSettings(const Path& path = Path()) const override;
        void LoadSettings(const Path& path = Path()) override;

    protected:
        void OnEvent(Event& event) override;

        KMP_NODISCARD bool OnWindowCloseEvent(WindowCloseEvent& event) override;
        KMP_NODISCARD bool OnWindowFramebufferRefreshEvent(WindowFramebufferRefreshEvent& event) override;

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