#include "uniform_buffers_application.h"


namespace Kmplete
{
    UniformBuffersApplication::UniformBuffersApplication(const WindowApplicationParameters& parameters)
        : WindowApplication(parameters)
        , _mainWindow(_windowBackend->GetMainWindow())
        , _mainFrameListener(nullptr)
    {
        _Initialize();
    }
    //--------------------------------------------------------------------------

    UniformBuffersApplication::~UniformBuffersApplication()
    {
        _Finalize();
    }
    //--------------------------------------------------------------------------

    void UniformBuffersApplication::_Initialize()
    {
        _mainWindow.SetTitle(GetApplicationName().c_str());
        _mainWindow.SetSizeLimits(Math::Size2I{ 1366, 768 }, Math::Size2I{});

        _mainFrameListener.reset(new UniformBuffersFrameListener(*_frameListenerManager.get(), _mainWindow, *_graphicsBackend.get(), _inputManager.get()));
    }
    //--------------------------------------------------------------------------

    void UniformBuffersApplication::_Finalize()
    {
        _mainFrameListener.reset();
    }
    //--------------------------------------------------------------------------
}