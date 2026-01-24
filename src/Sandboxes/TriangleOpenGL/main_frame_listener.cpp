#include "main_frame_listener.h"

#include <glad/glad.h>


namespace Kmplete
{
    MainFrameListener::MainFrameListener(FrameListenerManager& frameListenerManager, Window& mainWindow)
        : FrameListener(frameListenerManager, "main_frame_listener"_sid, 0)
        , _mainWindow(mainWindow)
    {}
    //--------------------------------------------------------------------------

    MainFrameListener::~MainFrameListener()
    {}
    //--------------------------------------------------------------------------

    void MainFrameListener::Update(KMP_MB_UNUSED float frameTimestep, KMP_MB_UNUSED bool applicationIsIconified)
    {}
    //--------------------------------------------------------------------------

    void MainFrameListener::Render()
    {
        glClearColor(0.2f, 0.2f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    }
    //--------------------------------------------------------------------------
}