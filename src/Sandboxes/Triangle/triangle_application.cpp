#include "triangle_application.h"

#include "Kmplete/Application/application_context.h"


namespace Kmplete
{
    TriangleApplication::TriangleApplication(const WindowApplicationParameters& parameters)
        : WindowApplication(parameters)
        , _mainWindow(_windowBackend->GetMainWindow())
        , _mainFrameListener(nullptr)
    {
        _Initialize();
    }
    //--------------------------------------------------------------------------

    TriangleApplication::~TriangleApplication()
    {
        _Finalize();
    }
    //--------------------------------------------------------------------------

    void TriangleApplication::_Initialize()
    {
        _mainWindow.SetTitle(ApplicationContext::GetApplicationName().c_str());
        _mainWindow.SetSizeLimits(Math::Size2I{ 1366, 768 }, Math::Size2I{});

        _mainFrameListener.reset(new TriangleFrameListener(*_frameListenerManager.get(), _mainWindow, *_graphicsBackend.get(), *_assetsManager.get(), _inputManager.get()));
    }
    //--------------------------------------------------------------------------

    void TriangleApplication::_Finalize()
    {
        _mainFrameListener.reset();
    }
    //--------------------------------------------------------------------------
}