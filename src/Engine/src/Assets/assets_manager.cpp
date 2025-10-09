#include "Kmplete/Assets/assets_manager.h"
#include "Kmplete/Assets/assets_interface.h"


namespace Kmplete
{
    namespace Assets
    {
        AssetsManager::AssetsManager(const Filepath& applicationPath)
            : KMP_PROFILE_CONSTRUCTOR_START_BASE_CLASS("AssetsManager::AssetsManager(const Filepath&)")
              _dataPath(applicationPath / AssetsFolder)
        {
            KMP_PROFILE_CONSTRUCTOR_END();
        }
        //--------------------------------------------------------------------------
    }
}