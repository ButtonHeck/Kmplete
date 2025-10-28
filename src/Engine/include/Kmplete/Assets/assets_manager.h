#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Graphics/texture_manager.h"
#include "Kmplete/Graphics/font_manager.h"
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
            KMP_API ~AssetsManager();

            KMP_NODISCARD KMP_API TextureManager& GetTextureManager() noexcept;
            KMP_NODISCARD KMP_API FontManager& GetFontManager() noexcept;

            KMP_NODISCARD KMP_API bool LoadAssetFile(const Filepath& filepath, bool loadBinaries = true);

            KMP_NODISCARD KMP_API bool LoadAssets(const Vector<Utils::StringID>& assetsSids);
            KMP_NODISCARD KMP_API bool UnloadAssets(const Vector<Utils::StringID>& assetsSids);

        private:
            void Initialize(GraphicsBackendType type);
            void Finalize();

            void LoadAssetFileHeaders(const BinaryBuffer& fileBuffer, AssetCount assetCount, const Filepath& filepath);
            KMP_NODISCARD bool LoadAssetFileBinaries(const BinaryBuffer& fileBuffer, AssetCount assetCount);

            Vector<AssetLookupInfo> GetSortedByFileAssetsInfos(const Vector<Utils::StringID>& assetsSids) const;
            KMP_NODISCARD bool LoadAssetsEntriesBinaries(const Vector<AssetLookupInfo>& lookupVector);
            KMP_NODISCARD bool LoadAssetEntryBinary(const BinaryBuffer& fileBuffer, const AssetEntryHeader& assetHeader);

        private:
            const Filepath _dataPath;
            UPtr<TextureManager> _textureManager;
            UPtr<FontManager> _fontManager;
            HashMap<Utils::StringID, AssetLookupInfo> _lookupMap;
        };
        //--------------------------------------------------------------------------
    }
}