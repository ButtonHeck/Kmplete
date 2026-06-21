#include "instanced_rendering_application.h"

#include "Kmplete/Application/application_context.h"


namespace Kmplete
{
    InstancedRenderingApplication::InstancedRenderingApplication(const WindowApplicationParameters& parameters)
        : WindowApplication(parameters)
        , _mainWindow(_windowBackend->GetMainWindow())
        , _mainFrameListener(nullptr)
    {
        _Initialize();
    }
    //--------------------------------------------------------------------------

    InstancedRenderingApplication::~InstancedRenderingApplication()
    {
        _Finalize();
    }
    //--------------------------------------------------------------------------

    void InstancedRenderingApplication::_Initialize()
    {
        _mainWindow.SetTitle(ApplicationContext::GetApplicationName().c_str());
        _mainWindow.SetSizeLimits(Math::Size2I{ 1366, 768 }, Math::Size2I{});

        _mainFrameListener.reset(new InstancedRenderingFrameListener(*_frameListenerManager.get(), _mainWindow, *_graphicsBackend.get()));
    }
    //--------------------------------------------------------------------------

    void InstancedRenderingApplication::_Finalize()
    {
        _mainFrameListener.reset();
    }
    //--------------------------------------------------------------------------
}