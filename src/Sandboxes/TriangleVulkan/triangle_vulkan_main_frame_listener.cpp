#include "triangle_vulkan_main_frame_listener.h"

#include "Kmplete/Utils/function_utils.h"


namespace Kmplete
{
    MainFrameListener::MainFrameListener(FrameListenerManager& frameListenerManager, Window& mainWindow)
        : FrameListener(frameListenerManager, "main_frame_listener"_sid, 0)
        , _mainWindow(mainWindow)
        , _windowFramebufferResizeHandler(_eventDispatcher, KMP_BIND(MainFrameListener::OnWindowFramebufferResizeEvent))
    {
        _Initialize();
    }
    //--------------------------------------------------------------------------

    MainFrameListener::~MainFrameListener()
    {
        _Finalize();
    }
    //--------------------------------------------------------------------------

    void MainFrameListener::_Initialize()
    {
    }
    //--------------------------------------------------------------------------

    void MainFrameListener::_Finalize()
    {}
    //--------------------------------------------------------------------------

    bool MainFrameListener::OnWindowFramebufferResizeEvent(Events::WindowFramebufferResizeEvent&)
    {
        return true;
    }
    //--------------------------------------------------------------------------

    void MainFrameListener::Update(KMP_MB_UNUSED float frameTimestep, KMP_MB_UNUSED bool applicationIsIconified)
    {}
    //--------------------------------------------------------------------------

    void MainFrameListener::Render()
    {}
    //--------------------------------------------------------------------------
}