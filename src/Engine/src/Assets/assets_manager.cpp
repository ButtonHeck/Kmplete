#include "Kmplete/Assets/assets_manager.h"
#include "Kmplete/Assets/assets_interface.h"
#include "Kmplete/Core/assertion.h"


namespace Kmplete
{
    namespace Assets
    {
        AssetsManager::AssetsManager(const Filepath& applicationPath, GraphicsBackendType type)
            : KMP_PROFILE_CONSTRUCTOR_START_BASE_CLASS("AssetsManager::AssetsManager(const Filepath&, GraphicsBackendType)")
              _dataPath(applicationPath / AssetsFolder)
            , _textureManager(CreateUPtr<TextureManager>(type))
        {
            KMP_PROFILE_CONSTRUCTOR_END();
        }
        //--------------------------------------------------------------------------

        TextureManager& AssetsManager::GetTextureManager()
        {
            KMP_ASSERT(_textureManager);
            return *_textureManager;
        }
        //--------------------------------------------------------------------------
    }
}