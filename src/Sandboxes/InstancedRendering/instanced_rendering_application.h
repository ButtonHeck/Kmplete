#pragma once

#include "instanced_rendering_frame_listener.h"

#include "Kmplete/Base/pointers.h"
#include "Kmplete/Application/window_application.h"


namespace Kmplete
{
    class InstancedRenderingApplication : public WindowApplication
    {
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