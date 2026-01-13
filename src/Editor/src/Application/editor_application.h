#pragma once

#include "Application/editor_frame_listener.h"

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
        KMP_DISABLE_COPY_MOVE(EditorApplication)

    public:
        explicit EditorApplication(const WindowApplicationParameters& parameters);
        virtual ~EditorApplication();

    private:
        void _Initialize();
        void _Finalize();

        void _SaveSettings() const;
        void _LoadSettings();

    private:
        Window& _mainWindow;
        UPtr<EditorFrameListener> _uiFrameListener;
    };
    //--------------------------------------------------------------------------
}