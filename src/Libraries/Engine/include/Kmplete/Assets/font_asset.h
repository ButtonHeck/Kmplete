#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Base/string_id.h"
#include "Kmplete/Graphics/font.h"
#include "Kmplete/Assets/asset.h"
#include "Kmplete/Profile/profiler_fwd.h"


struct FT_LibraryRec_;


namespace Kmplete
{
    namespace Assets
    {
        //! Asset of a font type containing single font object
        //! @see Font
        //! @see Assets::Asset
        class KMP_API FontAsset : public Asset
        {
            KMP_PROFILE_CONSTRUCTOR_DECLARE()

        public:
            FontAsset(StringID sid, FT_LibraryRec_& freetypeLib, BinaryBuffer&& fontBuffer);
            FontAsset(StringID sid, FT_LibraryRec_& freetypeLib, const Filepath& filepath);
            ~FontAsset() = default;

            KMP_NODISCARD const Graphics::Font& GetFont() const noexcept;
            KMP_NODISCARD Graphics::Font& GetFont() noexcept;

        private:
            Graphics::Font _font;
        };
        //--------------------------------------------------------------------------
    }
}