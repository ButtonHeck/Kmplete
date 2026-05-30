#pragma once

#include "draw_indirect_frame_listener.h"

#include "Kmplete/Base/pointers.h"
#include "Kmplete/Application/window_application.h"


namespace Kmplete
{
    class DrawIndirectApplication : public WindowApplication
    {
        KMP_DISABLE_COPY_MOVE(DrawIndirectApplication)

    public:
        explicit DrawIndirectApplication(const WindowApplicationParameters& parameters);
        ~DrawIndirectApplication();

    private:
        void _Initialize();
        void _Finalize();

    private:
        Window& _mainWindow;
        UPtr<DrawIndirectFrameListener> _mainFrameListener;
    };
    //--------------------------------------------------------------------------
}