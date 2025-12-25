#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Base/string_id.h"
#include "Kmplete/Graphics/font.h"
#include "Kmplete/Assets/asset.h"


struct FT_LibraryRec_;


namespace Kmplete
{
    namespace Assets
    {
        //! Asset of a font type containing single font object
        //! @see Font
        //! @see Assets::Asset
        class FontAsset : public Asset
        {
        public:
            KMP_API FontAsset(StringID sid, FT_LibraryRec_& freetypeLib, BinaryBuffer&& fontBuffer);
            KMP_API FontAsset(StringID sid, FT_LibraryRec_& freetypeLib, const Filepath& filepath);

            KMP_NODISCARD KMP_API const Font& GetFont() const noexcept;
            KMP_NODISCARD KMP_API Font& GetFont() noexcept;

        private:
            Font _font;
        };
        //--------------------------------------------------------------------------
    }
}