#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Assets/texture_asset_manager.h"
#include "Kmplete/Assets/font_asset_manager.h"
#include "Kmplete/Assets/assets_interface.h"
#include "Kmplete/Profile/profiler.h"
#include "Kmplete/Log/log_class_macro.h"


namespace Kmplete
{
    namespace Assets
    {
        //! Manager for application assets. Responsible for managing lifetime of asset submanagers, handling
        //! assets loading and unloading, loading assets files. All asset files are supposed to be placed in
        //! the Data directory relative to the application executable directory
        //! @see assets_interface.h
        class AssetsManager
        {
            KMP_LOG_CLASSNAME(AssetsManager)
            KMP_PROFILE_CONSTRUCTOR_DECLARE()
            KMP_DISABLE_COPY_MOVE(AssetsManager)

        public:
            KMP_API AssetsManager(const Filepath& applicationPath, GraphicsBackendType type);
            KMP_API ~AssetsManager();

            KMP_NODISCARD KMP_API const TextureAssetManager& GetTextureAssetManager() const noexcept;
            KMP_NODISCARD KMP_API TextureAssetManager& GetTextureAssetManager() noexcept;

            KMP_NODISCARD KMP_API const FontAssetManager& GetFontAssetManager() const noexcept;
            KMP_NODISCARD KMP_API FontAssetManager& GetFontAssetManager() noexcept;

            KMP_NODISCARD KMP_API bool LoadAssetFile(const Filepath& filepath, bool loadBinaries = true);

            KMP_NODISCARD KMP_API bool LoadAssets(const Vector<StringID>& assetsSids);
            KMP_NODISCARD KMP_API bool UnloadAssets(const Vector<StringID>& assetsSids);

        private:
            void _Initialize(GraphicsBackendType type);
            void _Finalize();

            void _LoadAssetFileHeaders(const BinaryBuffer& fileBuffer, AssetCount assetCount, const Filepath& filepath);
            KMP_NODISCARD bool _LoadAssetFileBinaries(const BinaryBuffer& fileBuffer, AssetCount assetCount);

            Vector<AssetLookupInfo> _GetSortedByFileAssetsInfos(const Vector<StringID>& assetsSids) const;
            KMP_NODISCARD bool _LoadAssetsEntriesBinaries(const Vector<AssetLookupInfo>& sortedLookupVector);
            KMP_NODISCARD bool _LoadAssetEntryBinary(const BinaryBuffer& fileBuffer, const AssetEntryHeader& assetHeader);

        private:
            const Filepath _dataPath;
            UPtr<TextureAssetManager> _textureAssetManager;
            UPtr<FontAssetManager> _fontAssetManager;
            HashMap<StringID, AssetLookupInfo> _lookupMap;
        };
        //--------------------------------------------------------------------------
    }
}