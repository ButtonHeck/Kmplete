#pragma once

#include "editor_frame_listener.h"
#include "system_metrics_frame_listener.h"
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

    private:
        void Initialize();
        void Finalize();

        void SaveSettings() const;
        void LoadSettings();

    private:
        Window& _mainWindow;
        UPtr<EditorFrameListener> _uiFrameListener;
        UPtr<SystemMetricsFrameListener> _systemMetricsFrameListener;
    };
    //--------------------------------------------------------------------------
}