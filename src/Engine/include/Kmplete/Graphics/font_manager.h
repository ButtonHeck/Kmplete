#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Base/pointers.h"
#include "Kmplete/Graphics/font.h"
#include "Kmplete/Utils/string_id.h"
#include "Kmplete/Log/log_class_macro.h"


struct FT_LibraryRec_;

namespace Kmplete
{
    class FontManager
    {
        KMP_LOG_CLASSNAME(FontManager)

    public:
        KMP_DISABLE_COPY_MOVE(FontManager)

        KMP_API FontManager();
        KMP_API ~FontManager();

        KMP_API bool CreateFontTTF(Utils::StringID fontSid, BinaryBuffer&& fontData);

        KMP_NODISCARD KMP_API const Font& GetFont(Utils::StringID fontSid) const;

        KMP_API void RemoveFonts(const Vector<Utils::StringID>& sids);
        KMP_NODISCARD KMP_API bool RemoveFont(Utils::StringID sid);

    private:
        void Initialize();
        void Finalize();

        KMP_NODISCARD bool CreateDefaultFont();
        KMP_NODISCARD bool AddFontToStorage(Utils::StringID sid, BinaryBuffer&& fontData);

    private:
        FT_LibraryRec_* _freetypeLibInstance;
        HashMap<Utils::StringID, UPtr<Font>> _fonts;
    };
    //--------------------------------------------------------------------------
}