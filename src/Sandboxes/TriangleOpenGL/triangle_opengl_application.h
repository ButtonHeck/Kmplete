#pragma once

#include "triangle_opengl_main_frame_listener.h"

#include "Kmplete/Base/pointers.h"
#include "Kmplete/Application/window_application.h"
#include "Kmplete/Log/log_class_macro.h"


namespace Kmplete
{
    class TriangleOpenGLApplication : public WindowApplication
    {
        KMP_LOG_CLASSNAME(TriangleOpenGLApplication)
        KMP_DISABLE_COPY_MOVE(TriangleOpenGLApplication)

    public:
        explicit TriangleOpenGLApplication(const WindowApplicationParameters& parameters);
        ~TriangleOpenGLApplication();

    private:
        void _Initialize();
        void _Finalize();

        void _SaveSettings() const;
        void _LoadSettings();

    private:
        Window& _mainWindow;
        UPtr<MainFrameListener> _mainFrameListener;
    };
    //--------------------------------------------------------------------------
}