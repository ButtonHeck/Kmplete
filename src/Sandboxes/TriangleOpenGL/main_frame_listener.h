#pragma once

#include "Kmplete/Application/frame_listener.h"
#include "Kmplete/Window/window.h"
#include "Kmplete/Base/kmplete_api.h"


namespace Kmplete
{
    class MainFrameListener : public FrameListener
    {
        KMP_LOG_CLASSNAME(MainFrameListener)
        KMP_PROFILE_CONSTRUCTOR_DECLARE()
        KMP_DISABLE_COPY_MOVE(MainFrameListener)

    public:
        MainFrameListener(FrameListenerManager& frameListenerManager, Window& mainWindow);
        ~MainFrameListener();

        void Update(float frameTimestep, bool applicationIsIconified) override;
        void Render() override;

    private:
        Window& _mainWindow;
    };
    //--------------------------------------------------------------------------
}