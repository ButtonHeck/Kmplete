#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Graphics/texture_manager.h"
#include "Kmplete/Profile/profiler.h"
#include "Kmplete/Log/log_class_macro.h"


namespace Kmplete
{
    namespace Assets
    {
        class AssetsManager
        {
            KMP_LOG_CLASSNAME(AssetsManager)
            KMP_PROFILE_CONSTRUCTOR_DECLARE()

        public:
            KMP_DISABLE_COPY_MOVE(AssetsManager)

            KMP_API explicit AssetsManager(const Filepath& applicationPath, GraphicsBackendType type);
            ~AssetsManager() = default;

            KMP_NODISCARD KMP_API TextureManager& GetTextureManager();

        private:
            const Filepath _dataPath;
            UPtr<TextureManager> _textureManager;
        };
        //--------------------------------------------------------------------------
    }
}