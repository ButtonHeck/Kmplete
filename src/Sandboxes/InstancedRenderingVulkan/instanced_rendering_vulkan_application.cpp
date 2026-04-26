#include "instanced_rendering_vulkan_application.h"


namespace Kmplete
{
    InstancedRenderingVulkanApplication::InstancedRenderingVulkanApplication(const WindowApplicationParameters& parameters)
        : WindowApplication(parameters)
        , _mainWindow(_windowBackend->GetMainWindow())
        , _mainFrameListener(nullptr)
    {
        _Initialize();
    }
    //--------------------------------------------------------------------------

    InstancedRenderingVulkanApplication::~InstancedRenderingVulkanApplication()
    {
        _Finalize();
    }
    //--------------------------------------------------------------------------

    void InstancedRenderingVulkanApplication::_Initialize()
    {
        _mainWindow.SetTitle(_applicationName.c_str());
        _mainWindow.SetSizeLimits(Math::Size2I{ 1366, 768 }, Math::Size2I{});

        _mainFrameListener.reset(new MainFrameListener(*_frameListenerManager.get(), _mainWindow, *_graphicsBackend.get()));
    }
    //--------------------------------------------------------------------------

    void InstancedRenderingVulkanApplication::_Finalize()
    {
        _mainFrameListener.reset();
    }
    //--------------------------------------------------------------------------
}