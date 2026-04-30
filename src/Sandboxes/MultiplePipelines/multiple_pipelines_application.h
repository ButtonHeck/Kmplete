#pragma once

#include "multiple_pipelines_frame_listener.h"

#include "Kmplete/Base/pointers.h"
#include "Kmplete/Application/window_application.h"
#include "Kmplete/Log/log_class_macro.h"


namespace Kmplete
{
    class MultiplePipelinesApplication : public WindowApplication
    {
        KMP_LOG_CLASSNAME(MultiplePipelinesApplication)
        KMP_DISABLE_COPY_MOVE(MultiplePipelinesApplication)

    public:
        explicit MultiplePipelinesApplication(const WindowApplicationParameters& parameters);
        ~MultiplePipelinesApplication();

    private:
        void _Initialize();
        void _Finalize();

    private:
        Window& _mainWindow;
        UPtr<MultiplePipelinesFrameListener> _mainFrameListener;
    };
    //--------------------------------------------------------------------------
}