#pragma once

#include "post_processing_frame_listener.h"

#include "Kmplete/Base/pointers.h"
#include "Kmplete/Application/window_application.h"


namespace Kmplete
{
    class PostProcessingApplication : public WindowApplication
    {
        KMP_DISABLE_COPY_MOVE(PostProcessingApplication)

    public:
        explicit PostProcessingApplication(const WindowApplicationParameters& parameters);
        ~PostProcessingApplication();

    private:
        void _Initialize();
        void _Finalize();

    private:
        Window& _mainWindow;
        UPtr<PostProcessingFrameListener> _mainFrameListener;
    };
    //--------------------------------------------------------------------------
}