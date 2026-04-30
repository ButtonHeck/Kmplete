#include "texture_application.h"


namespace Kmplete
{
    TextureApplication::TextureApplication(const WindowApplicationParameters& parameters)
        : WindowApplication(parameters)
        , _mainWindow(_windowBackend->GetMainWindow())
        , _mainFrameListener(nullptr)
    {
        _Initialize();
    }
    //--------------------------------------------------------------------------

    TextureApplication::~TextureApplication()
    {
        _Finalize();
    }
    //--------------------------------------------------------------------------

    void TextureApplication::_Initialize()
    {
        _mainWindow.SetTitle(_applicationName.c_str());
        _mainWindow.SetSizeLimits(Math::Size2I{ 1366, 768 }, Math::Size2I{});

        if (!_assetsManager->LoadAssetFile("TextureSandbox_assets.kmpdata"))
        {
            KMP_LOG_ERROR("failed to load TextureSandbox assets");
        }

        _mainFrameListener.reset(new TextureFrameListener(*_frameListenerManager.get(), _mainWindow, *_graphicsBackend.get(), *_assetsManager.get(), _inputManager.get()));
    }
    //--------------------------------------------------------------------------

    void TextureApplication::_Finalize()
    {
        _mainFrameListener.reset();
    }
    //--------------------------------------------------------------------------
}