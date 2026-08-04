#pragma once

#include "text_rendering_frame_listener.h"

#include "Kmplete/Base/pointers.h"
#include "Kmplete/Application/window_application.h"


namespace Kmplete
{
    class TextRenderingApplication : public WindowApplication
    {
        KMP_DISABLE_COPY_MOVE(TextRenderingApplication)

    public:
        explicit TextRenderingApplication(const WindowApplicationParameters& parameters);
        ~TextRenderingApplication();

    private:
        void _Initialize();
        void _Finalize();

    private:
        Window& _mainWindow;
        UPtr<TextRenderingFrameListener> _mainFrameListener;
    };
    //--------------------------------------------------------------------------
}