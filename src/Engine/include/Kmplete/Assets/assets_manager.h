#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Graphics/texture_manager.h"
#include "Kmplete/Assets/assets_interface.h"
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

            KMP_API AssetsManager(const Filepath& applicationPath, GraphicsBackendType type);
            ~AssetsManager() = default;

            KMP_NODISCARD KMP_API TextureManager& GetTextureManager() noexcept;

            KMP_NODISCARD KMP_API bool LoadAssetFile(const Filepath& filepath, bool loadBinaries = true);

            KMP_API void LoadAssets(const Vector<Utils::StringID>& assetsSids);
            KMP_API void UnloadAssets(const Vector<Utils::StringID>& assetsSids);

        private:
            void LoadAssetFileHeaderData(const BinaryBuffer& fileBuffer, AssetCount assetCount, const Filepath& filepath);
            void LoadAssetFileBinaryData(const BinaryBuffer& fileBuffer, AssetCount assetCount);
            void LoadAssetBinary(const BinaryBuffer& fileBuffer, const AssetDataEntryHeader& assetHeader);

        private:
            const Filepath _dataPath;
            UPtr<TextureManager> _textureManager;
            HashMap<Utils::StringID, AssetLookupInfo> _lookupMap;
        };
        //--------------------------------------------------------------------------
    }
}