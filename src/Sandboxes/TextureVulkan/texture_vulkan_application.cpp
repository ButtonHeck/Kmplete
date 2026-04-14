#include "texture_vulkan_application.h"


namespace Kmplete
{
    TextureVulkanApplication::TextureVulkanApplication(const WindowApplicationParameters& parameters)
        : WindowApplication(parameters)
        , _mainWindow(_windowBackend->GetMainWindow())
        , _mainFrameListener(nullptr)
    {
        _Initialize();
    }
    //--------------------------------------------------------------------------

    TextureVulkanApplication::~TextureVulkanApplication()
    {
        _Finalize();
    }
    //--------------------------------------------------------------------------

    void TextureVulkanApplication::_Initialize()
    {
        _mainWindow.SetTitle(_applicationName.c_str());
        _mainWindow.SetSizeLimits(Math::Size2I{ 1366, 768 }, Math::Size2I{});

        if (!_assetsManager->LoadAssetFile("TextureVulkan_assets.kmpdata"))
        {
            KMP_LOG_ERROR("failed to load TextureVulkan assets");
        }

        _mainFrameListener.reset(new MainFrameListener(*_frameListenerManager.get(), _mainWindow, *_graphicsBackend.get(), *_assetsManager.get(), _inputManager.get()));
    }
    //--------------------------------------------------------------------------

    void TextureVulkanApplication::_Finalize()
    {
        _mainFrameListener.reset();
    }
    //--------------------------------------------------------------------------
}