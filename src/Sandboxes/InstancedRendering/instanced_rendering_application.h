#pragma once

#include "instanced_rendering_frame_listener.h"

#include "Kmplete/Base/pointers.h"
#include "Kmplete/Application/window_application.h"
#include "Kmplete/Log/log_class_macro.h"


namespace Kmplete
{
    class InstancedRenderingApplication : public WindowApplication
    {
        KMP_LOG_CLASSNAME(InstancedRenderingApplication)
        KMP_DISABLE_COPY_MOVE(InstancedRenderingApplication)

    public:
        explicit InstancedRenderingApplication(const WindowApplicationParameters& parameters);
        ~InstancedRenderingApplication();

    private:
        void _Initialize();
        void _Finalize();

    private:
        Window& _mainWindow;
        UPtr<InstancedRenderingFrameListener> _mainFrameListener;
    };
    //--------------------------------------------------------------------------
}