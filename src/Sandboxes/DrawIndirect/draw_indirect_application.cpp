#include "draw_indirect_application.h"


namespace Kmplete
{
    DrawIndirectApplication::DrawIndirectApplication(const WindowApplicationParameters& parameters)
        : WindowApplication(parameters)
        , _mainWindow(_windowBackend->GetMainWindow())
        , _mainFrameListener(nullptr)
    {
        _Initialize();
    }
    //--------------------------------------------------------------------------

    DrawIndirectApplication::~DrawIndirectApplication()
    {
        _Finalize();
    }
    //--------------------------------------------------------------------------

    void DrawIndirectApplication::_Initialize()
    {
        _mainWindow.SetTitle(_applicationName.c_str());
        _mainWindow.SetSizeLimits(Math::Size2I{ 1366, 768 }, Math::Size2I{});

        _mainFrameListener.reset(new DrawIndirectFrameListener(*_frameListenerManager.get(), _mainWindow, *_graphicsBackend.get(), _dataPath));
    }
    //--------------------------------------------------------------------------

    void DrawIndirectApplication::_Finalize()
    {
        _mainFrameListener.reset();
    }
    //--------------------------------------------------------------------------
}