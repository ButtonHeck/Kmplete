#include "push_constants_application.h"


namespace Kmplete
{
    PushConstantsApplication::PushConstantsApplication(const WindowApplicationParameters& parameters)
        : WindowApplication(parameters)
        , _mainWindow(_windowBackend->GetMainWindow())
        , _mainFrameListener(nullptr)
    {
        _Initialize();
    }
    //--------------------------------------------------------------------------

    PushConstantsApplication::~PushConstantsApplication()
    {
        _Finalize();
    }
    //--------------------------------------------------------------------------

    void PushConstantsApplication::_Initialize()
    {
        _mainWindow.SetTitle(_applicationName.c_str());
        _mainWindow.SetSizeLimits(Math::Size2I{ 1366, 768 }, Math::Size2I{});

        _mainFrameListener.reset(new PushConstantsFrameListener(*_frameListenerManager.get(), _mainWindow, *_graphicsBackend.get(), _dataPath));
    }
    //--------------------------------------------------------------------------

    void PushConstantsApplication::_Finalize()
    {
        _mainFrameListener.reset();
    }
    //--------------------------------------------------------------------------
}