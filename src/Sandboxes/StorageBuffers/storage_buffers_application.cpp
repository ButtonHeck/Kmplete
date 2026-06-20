#include "storage_buffers_application.h"


namespace Kmplete
{
    StorageBuffersApplication::StorageBuffersApplication(const WindowApplicationParameters& parameters)
        : WindowApplication(parameters)
        , _mainWindow(_windowBackend->GetMainWindow())
        , _mainFrameListener(nullptr)
    {
        _Initialize();
    }
    //--------------------------------------------------------------------------

    StorageBuffersApplication::~StorageBuffersApplication()
    {
        _Finalize();
    }
    //--------------------------------------------------------------------------

    void StorageBuffersApplication::_Initialize()
    {
        _mainWindow.SetTitle(GetApplicationName().c_str());
        _mainWindow.SetSizeLimits(Math::Size2I{ 1366, 768 }, Math::Size2I{});

        _mainFrameListener.reset(new StorageBuffersFrameListener(*_frameListenerManager.get(), _mainWindow, *_graphicsBackend.get(), _inputManager.get()));
    }
    //--------------------------------------------------------------------------

    void StorageBuffersApplication::_Finalize()
    {
        _mainFrameListener.reset();
    }
    //--------------------------------------------------------------------------
}