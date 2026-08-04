#include "text_rendering_application.h"

#include "Kmplete/Application/application_context.h"


namespace Kmplete
{
    TextRenderingApplication::TextRenderingApplication(const WindowApplicationParameters& parameters)
        : WindowApplication(parameters)
        , _mainWindow(_windowBackend->GetMainWindow())
        , _mainFrameListener(nullptr)
    {
        _Initialize();
    }
    //--------------------------------------------------------------------------

    TextRenderingApplication::~TextRenderingApplication()
    {
        _Finalize();
    }
    //--------------------------------------------------------------------------

    void TextRenderingApplication::_Initialize()
    {
        _mainWindow.SetTitle(ApplicationContext::GetApplicationName().c_str());
        _mainWindow.SetSizeLimits(Math::Size2I{ 1366, 768 }, Math::Size2I{});

        _mainFrameListener.reset(new TextRenderingFrameListener(*_frameListenerManager.get(), _mainWindow, *_graphicsBackend.get(), *_assetsManager.get()));
    }
    //--------------------------------------------------------------------------

    void TextRenderingApplication::_Finalize()
    {
        _mainFrameListener.reset();
    }
    //--------------------------------------------------------------------------
}