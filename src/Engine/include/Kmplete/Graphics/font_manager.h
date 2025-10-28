#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Base/optional.h"
#include "Kmplete/Utils/string_id.h"
#include "Kmplete/Log/log_class_macro.h"


namespace Kmplete
{
    class FontManager
    {
        KMP_LOG_CLASSNAME(FontManager)

    public:
        KMP_DISABLE_COPY_MOVE(FontManager)

        FontManager() = default;

        KMP_API bool CreateFontTTF(Utils::StringID fontSid, const BinaryBuffer& fontData);

        KMP_NODISCARD KMP_API OptionalRef<const BinaryBuffer> GetFont(Utils::StringID fontSid) const;

    private:
        HashMap<Utils::StringID, BinaryBuffer> _fonts;
    };
    //--------------------------------------------------------------------------
}