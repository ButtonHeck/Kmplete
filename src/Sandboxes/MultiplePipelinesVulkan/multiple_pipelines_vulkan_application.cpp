#include "multiple_pipelines_vulkan_application.h"


namespace Kmplete
{
    MultiplePipelinesVulkanApplication::MultiplePipelinesVulkanApplication(const WindowApplicationParameters& parameters)
        : WindowApplication(parameters)
        , _mainWindow(_windowBackend->GetMainWindow())
        , _mainFrameListener(nullptr)
    {
        _Initialize();
    }
    //--------------------------------------------------------------------------

    MultiplePipelinesVulkanApplication::~MultiplePipelinesVulkanApplication()
    {
        _Finalize();
    }
    //--------------------------------------------------------------------------

    void MultiplePipelinesVulkanApplication::_Initialize()
    {
        _mainWindow.SetTitle(_applicationName.c_str());
        _mainWindow.SetSizeLimits(Math::Size2I{ 1366, 768 }, Math::Size2I{});

        _mainFrameListener.reset(new MainFrameListener(*_frameListenerManager.get(), _mainWindow, *_graphicsBackend.get()));
    }
    //--------------------------------------------------------------------------

    void MultiplePipelinesVulkanApplication::_Finalize()
    {
        _mainFrameListener.reset();
    }
    //--------------------------------------------------------------------------
}