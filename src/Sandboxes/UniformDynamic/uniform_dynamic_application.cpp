#include "uniform_dynamic_application.h"


namespace Kmplete
{
    UniformDynamicApplication::UniformDynamicApplication(const WindowApplicationParameters& parameters)
        : WindowApplication(parameters)
        , _mainWindow(_windowBackend->GetMainWindow())
        , _mainFrameListener(nullptr)
    {
        _Initialize();
    }
    //--------------------------------------------------------------------------

    UniformDynamicApplication::~UniformDynamicApplication()
    {
        _Finalize();
    }
    //--------------------------------------------------------------------------

    void UniformDynamicApplication::_Initialize()
    {
        _mainWindow.SetTitle(_applicationName.c_str());
        _mainWindow.SetSizeLimits(Math::Size2I{ 1366, 768 }, Math::Size2I{});

        _mainFrameListener.reset(new UniformDynamicFrameListener(*_frameListenerManager.get(), _mainWindow, *_graphicsBackend.get(), _inputManager.get()));
    }
    //--------------------------------------------------------------------------

    void UniformDynamicApplication::_Finalize()
    {
        _mainFrameListener.reset();
    }
    //--------------------------------------------------------------------------
}