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
    class FontManager
    {
        KMP_LOG_CLASSNAME(FontManager)

    public:
        static constexpr Utils::StringID DefaultFontSID = 0;

        KMP_DISABLE_COPY_MOVE(FontManager)

        KMP_API FontManager();
        KMP_API ~FontManager();

        KMP_API bool CreateFontAsset(Utils::StringID fontSid, BinaryBuffer&& fontData);
        KMP_API bool CreateFontAsset(Utils::StringID fontSid, const Filepath& filepath);

        KMP_NODISCARD KMP_API const Assets::FontAsset& GetFontAsset(Utils::StringID fontSid) const;
        KMP_NODISCARD KMP_API Assets::FontAsset& GetFontAsset(Utils::StringID fontSid);

        KMP_API void RemoveFontsAssets(const Vector<Utils::StringID>& sids);
        KMP_NODISCARD KMP_API bool RemoveFontAsset(Utils::StringID sid);

        KMP_NODISCARD KMP_API UInt64 FontsAssetsCount() const noexcept;

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