#pragma once

#include "main_frame_listener.h"

#include "Kmplete/Base/pointers.h"
#include "Kmplete/Application/window_application.h"
#include "Kmplete/Log/log_class_macro.h"


namespace Kmplete
{
    class TriangleApplication : public WindowApplication
    {
        KMP_LOG_CLASSNAME(TriangleApplication)
        KMP_DISABLE_COPY_MOVE(TriangleApplication)

    public:
        explicit TriangleApplication(const WindowApplicationParameters& parameters);
        ~TriangleApplication();

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