#include "Kmplete/Graphics/font.h"
#include "Kmplete/Log/log.h"

#include <ft2build.h>
#include FT_FREETYPE_H
#include <stdexcept>


namespace Kmplete
{
    Font::Font(Utils::StringID sid, FT_LibraryRec_& freetypeLib, BinaryBuffer&& fontBuffer)
        : Assets::Asset(Assets::AssetType::FontTTF, sid)
          KMP_PROFILE_CONSTRUCTOR_START_DERIVED_CLASS("Font::Font(Utils::StringID, FT_LibraryRec_&, const BinaryBuffer&)")
        , _freetypeFace(nullptr)
        , _fontBuffer(std::move(fontBuffer))
    {
        const auto freetypeFontInitError = FT_New_Memory_Face(&freetypeLib, _fontBuffer.data(), static_cast<FT_Long>(_fontBuffer.size()), 0, &_freetypeFace);
        if (freetypeFontInitError)
        {
            KMP_LOG_ERROR("failed to load FreeType font from buffer");
            throw std::runtime_error("Font: failed to load FreeType font from buffer");
        }

        KMP_PROFILE_CONSTRUCTOR_END()
    }
    //--------------------------------------------------------------------------

    Font::~Font()
    {
        KMP_PROFILE_FUNCTION(ProfileLevelAlways);

        const auto freetypeFontDoneError = FT_Done_Face(_freetypeFace);
        if (freetypeFontDoneError)
        {
            KMP_LOG_ERROR("failed to destroy FreeType font instance");
        }
    }
    //--------------------------------------------------------------------------

    const BinaryBuffer& Font::GetBuffer() const noexcept
    {
        return _fontBuffer;
    }
    //--------------------------------------------------------------------------
}