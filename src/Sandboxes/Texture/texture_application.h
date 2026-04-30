#pragma once

#include "texture_frame_listener.h"

#include "Kmplete/Base/pointers.h"
#include "Kmplete/Application/window_application.h"
#include "Kmplete/Log/log_class_macro.h"


namespace Kmplete
{
    class TextureApplication : public WindowApplication
    {
        KMP_LOG_CLASSNAME(TextureApplication)
        KMP_DISABLE_COPY_MOVE(TextureApplication)

    public:
        explicit TextureApplication(const WindowApplicationParameters& parameters);
        ~TextureApplication();

    private:
        void _Initialize();
        void _Finalize();

    private:
        Window& _mainWindow;
        UPtr<TextureFrameListener> _mainFrameListener;
    };
    //--------------------------------------------------------------------------
}