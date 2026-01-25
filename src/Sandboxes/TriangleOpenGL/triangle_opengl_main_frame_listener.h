#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Application/frame_listener.h"
#include "Kmplete/Window/window.h"
#include "Kmplete/Event/window_events.h"
#include "Kmplete/Event/event_handler_guard.h"


namespace Kmplete
{
    class MainFrameListener : public FrameListener
    {
        KMP_LOG_CLASSNAME(MainFrameListener)
        KMP_DISABLE_COPY_MOVE(MainFrameListener)

    public:
        MainFrameListener(FrameListenerManager& frameListenerManager, Window& mainWindow);
        ~MainFrameListener();

        void Update(float frameTimestep, bool applicationIsIconified) override;
        void Render() override;

    private:
        bool OnWindowFramebufferResizeEvent(Events::WindowFramebufferResizeEvent& event);

        void _Initialize();
        void _Finalize();

    private:
        Window& _mainWindow;
        Events::EventHandlerGuard<Events::WindowFramebufferResizeEvent> _windowFramebufferResizeHandler;

        unsigned int _vao;
        unsigned int _vbo;
        unsigned int _ebo;
        unsigned int _shaderProgramId;
    };
    //--------------------------------------------------------------------------
}