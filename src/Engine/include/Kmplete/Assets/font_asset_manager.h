#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Base/pointers.h"
#include "Kmplete/Base/string_id.h"
#include "Kmplete/Assets/font_asset.h"
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
            static constexpr StringID DefaultFontSID = 0;

            KMP_DISABLE_COPY_MOVE(FontAssetManager)

            KMP_API FontAssetManager();
            KMP_API ~FontAssetManager();

            KMP_API bool CreateAsset(StringID fontSid, BinaryBuffer&& fontData);
            KMP_API bool CreateAsset(StringID fontSid, const Filepath& filepath);

            KMP_NODISCARD KMP_API const Assets::FontAsset& GetAsset(StringID fontSid) const;
            KMP_NODISCARD KMP_API Assets::FontAsset& GetAsset(StringID fontSid);

            KMP_API void RemoveAssets(const Vector<StringID>& sids);
            KMP_NODISCARD KMP_API bool RemoveAsset(StringID sid);

            KMP_NODISCARD KMP_API UInt64 GetAssetsCount() const noexcept;

        private:
            void _Initialize();
            void _Finalize();

            KMP_NODISCARD bool _CreateDefaultFontAsset();
            KMP_NODISCARD bool _AddFontToStorage(StringID sid, BinaryBuffer&& fontData);

        private:
            FT_LibraryRec_* _freetypeLibInstance;
            HashMap<StringID, UPtr<Assets::FontAsset>> _fonts;
        };
        //--------------------------------------------------------------------------
    }
}