#include "triangle_vulkan_application.h"


namespace Kmplete
{
    TriangleVulkanApplication::TriangleVulkanApplication(const WindowApplicationParameters& parameters)
        : WindowApplication(parameters)
        , _mainWindow(_windowBackend->GetMainWindow())
        , _mainFrameListener(nullptr)
    {
        _Initialize();
    }
    //--------------------------------------------------------------------------

    TriangleVulkanApplication::~TriangleVulkanApplication()
    {
        _Finalize();
    }
    //--------------------------------------------------------------------------

    void TriangleVulkanApplication::_Initialize()
    {
        _mainWindow.SetTitle(_applicationName.c_str());
        _mainWindow.SetSizeLimits(Math::Size2I{ 1366, 768 }, Math::Size2I{});

        _mainFrameListener.reset(new MainFrameListener(*_frameListenerManager.get(), _mainWindow, *_graphicsBackend.get(), *_assetsManager.get(), _inputManager.get()));

        _LoadSettings();
    }
    //--------------------------------------------------------------------------

    void TriangleVulkanApplication::_Finalize()
    {
        _SaveSettings();

        _mainFrameListener.reset();
    }
    //--------------------------------------------------------------------------

    void TriangleVulkanApplication::_SaveSettings() const
    {
    }
    //--------------------------------------------------------------------------

    void TriangleVulkanApplication::_LoadSettings()
    {
    }
    //--------------------------------------------------------------------------
}