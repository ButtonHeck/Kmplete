#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Base/pointers.h"
#include "Kmplete/Assets/font_asset.h"
#include "Kmplete/Utils/string_id.h"
#include "Kmplete/Log/log_class_macro.h"


struct FT_LibraryRec_;

namespace Kmplete
{
    namespace Assets
    {
        class FontAssetManager
        {
            KMP_LOG_CLASSNAME(FontAssetManager)

        public:
            static constexpr Utils::StringID DefaultFontSID = 0;

            KMP_DISABLE_COPY_MOVE(FontAssetManager)

            KMP_API FontAssetManager();
            KMP_API ~FontAssetManager();

            KMP_API bool CreateAsset(Utils::StringID fontSid, BinaryBuffer&& fontData);
            KMP_API bool CreateAsset(Utils::StringID fontSid, const Filepath& filepath);

            KMP_NODISCARD KMP_API const Assets::FontAsset& GetAsset(Utils::StringID fontSid) const;
            KMP_NODISCARD KMP_API Assets::FontAsset& GetAsset(Utils::StringID fontSid);

            KMP_API void RemoveAssets(const Vector<Utils::StringID>& sids);
            KMP_NODISCARD KMP_API bool RemoveAsset(Utils::StringID sid);

            KMP_NODISCARD KMP_API UInt64 GetAssetsCount() const noexcept;

        private:
            void Initialize();
            void Finalize();

            KMP_NODISCARD bool CreateDefaultFontAsset();
            KMP_NODISCARD bool AddFontToStorage(Utils::StringID sid, BinaryBuffer&& fontData);

        private:
            FT_LibraryRec_* _freetypeLibInstance;
            HashMap<Utils::StringID, UPtr<Assets::FontAsset>> _fonts;
        };
        //--------------------------------------------------------------------------
    }
}