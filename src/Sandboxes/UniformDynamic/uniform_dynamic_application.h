#pragma once

#include "uniform_dynamic_frame_listener.h"

#include "Kmplete/Base/pointers.h"
#include "Kmplete/Application/window_application.h"
#include "Kmplete/Log/log_class_macro.h"


namespace Kmplete
{
    class UniformDynamicApplication : public WindowApplication
    {
        KMP_LOG_CLASSNAME(UniformDynamicApplication)
        KMP_DISABLE_COPY_MOVE(UniformDynamicApplication)

    public:
        explicit UniformDynamicApplication(const WindowApplicationParameters& parameters);
        ~UniformDynamicApplication();

    private:
        void _Initialize();
        void _Finalize();

    private:
        Window& _mainWindow;
        UPtr<UniformDynamicFrameListener> _mainFrameListener;
    };
    //--------------------------------------------------------------------------
}