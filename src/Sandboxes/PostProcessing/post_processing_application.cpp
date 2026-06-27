#include "post_processing_application.h"

#include "Kmplete/Application/application_context.h"


namespace Kmplete
{
    PostProcessingApplication::PostProcessingApplication(const WindowApplicationParameters& parameters)
        : WindowApplication(parameters)
        , _mainWindow(_windowBackend->GetMainWindow())
        , _mainFrameListener(nullptr)
    {
        _Initialize();
    }
    //--------------------------------------------------------------------------

    PostProcessingApplication::~PostProcessingApplication()
    {
        _Finalize();
    }
    //--------------------------------------------------------------------------

    void PostProcessingApplication::_Initialize()
    {
        _mainWindow.SetTitle(ApplicationContext::GetApplicationName().c_str());
        _mainWindow.SetSizeLimits(Math::Size2I{ 1366, 768 }, Math::Size2I{});

        _mainFrameListener.reset(new PostProcessingFrameListener(*_frameListenerManager.get(), _mainWindow, *_graphicsBackend.get()));
    }
    //--------------------------------------------------------------------------

    void PostProcessingApplication::_Finalize()
    {
        _mainFrameListener.reset();
    }
    //--------------------------------------------------------------------------
}