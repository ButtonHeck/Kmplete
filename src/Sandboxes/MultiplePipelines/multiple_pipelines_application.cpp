#include "multiple_pipelines_application.h"


namespace Kmplete
{
    MultiplePipelinesApplication::MultiplePipelinesApplication(const WindowApplicationParameters& parameters)
        : WindowApplication(parameters)
        , _mainWindow(_windowBackend->GetMainWindow())
        , _mainFrameListener(nullptr)
    {
        _Initialize();
    }
    //--------------------------------------------------------------------------

    MultiplePipelinesApplication::~MultiplePipelinesApplication()
    {
        _Finalize();
    }
    //--------------------------------------------------------------------------

    void MultiplePipelinesApplication::_Initialize()
    {
        _mainWindow.SetTitle(_applicationName.c_str());
        _mainWindow.SetSizeLimits(Math::Size2I{ 1366, 768 }, Math::Size2I{});

        _mainFrameListener.reset(new MultiplePipelinesFrameListener(*_frameListenerManager.get(), _mainWindow, *_graphicsBackend.get()));
    }
    //--------------------------------------------------------------------------

    void MultiplePipelinesApplication::_Finalize()
    {
        _mainFrameListener.reset();
    }
    //--------------------------------------------------------------------------
}