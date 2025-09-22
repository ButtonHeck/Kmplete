#pragma once

#include "editor_ui.h"
#include "Kmplete/Base/pointers.h"
#include "Kmplete/Application/window_application.h"
#include "Kmplete/Profile/profiler.h"
#include "Kmplete/Log/log_class_macro.h"

namespace Kmplete
{
    class EditorApplication : public WindowApplication
    {
        KMP_LOG_CLASSNAME(EditorApplication)
        KMP_PROFILE_CONSTRUCTOR_DECLARE()

    public:
        KMP_DISABLE_COPY_MOVE(EditorApplication)

        explicit EditorApplication(const ApplicationParameters& applicationParameters);
        virtual ~EditorApplication();

        void SaveSettings(const Filepath& filepath = Filepath()) const override;
        void LoadSettings(const Filepath& filepath = Filepath()) override;

    private:
        void Initialize();
        void Finalize();
        void SaveSettingsInternal() const;
        void LoadSettingsInternal();

    private:
        Window& _mainWindow;
        UPtr<EditorUI> _ui;
    };
    //--------------------------------------------------------------------------
}