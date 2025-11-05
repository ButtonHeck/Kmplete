#include "Kmplete/Graphics/font.h"
#include "Kmplete/Log/log.h"

#include <ft2build.h>
#include FT_FREETYPE_H
#include <stdexcept>


namespace Kmplete
{
    static constexpr auto DefaultFontPixelSize = 18;

    Font::Font(Utils::StringID sid, FT_LibraryRec_& freetypeLib, BinaryBuffer&& fontBuffer)
        : Assets::Asset(Assets::AssetType::FontTTF, sid)
          KMP_PROFILE_CONSTRUCTOR_START_DERIVED_CLASS()
        , _freetypeFace(nullptr)
        , _fontBuffer(std::move(fontBuffer))
    {
        const auto freetypeFontInitError = FT_New_Memory_Face(&freetypeLib, _fontBuffer.data(), static_cast<FT_Long>(_fontBuffer.size()), 0, &_freetypeFace);
        if (freetypeFontInitError)
        {
            KMP_LOG_ERROR("failed to load FreeType font from buffer");
            throw std::runtime_error("Font: failed to load FreeType font from buffer");
        }

        UpdateParameters();
        SetPixelSize(DefaultFontPixelSize);

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

    bool Font::SetPointSize(UInt8 size, UInt32 dpi)
    {
        KMP_PROFILE_FUNCTION(ProfileLevelMinorFunctionsVerbose);

        const auto setSizeError = FT_Set_Char_Size(_freetypeFace, size * 64, 0, dpi, dpi);
        if (setSizeError)
        {
            KMP_LOG_ERROR("failed to set point size to {}", size);
            return false;
        }

        UpdateSizeMetrics();
        return true;
    }
    //--------------------------------------------------------------------------

    bool Font::SetPixelSize(UInt8 size)
    {
        KMP_PROFILE_FUNCTION(ProfileLevelMinorFunctionsVerbose);

        const auto setSizeError = FT_Set_Pixel_Sizes(_freetypeFace, size, 0);
        if (setSizeError)
        {
            KMP_LOG_ERROR("failed to set pixel size to {}", size);
            return false;
        }

        UpdateSizeMetrics();
        return true;
    }
    //--------------------------------------------------------------------------

    const Font::Parameters& Font::GetParameters() const noexcept
    {
        return _parameters;
    }
    //--------------------------------------------------------------------------

    void Font::UpdateParameters() noexcept
    {
        _parameters.familyName = _freetypeFace->family_name;

        const auto style = _freetypeFace->style_flags;
        if (style & FT_STYLE_FLAG_BOLD)
        {
            _parameters.style = Parameters::Style::Bold;
        }
        else if (style & FT_STYLE_FLAG_ITALIC)
        {
            _parameters.style = Parameters::Style::Italic;
        }
        else
        {
            _parameters.style = Parameters::Style::Regular;
        }

        _parameters.scalable = _freetypeFace->face_flags & FT_FACE_FLAG_SCALABLE;
        _parameters.monospace = _freetypeFace->face_flags & FT_FACE_FLAG_FIXED_WIDTH;
        _parameters.numGlyphs = _freetypeFace->num_glyphs;
        _parameters.unitsPerEM = _freetypeFace->units_per_EM;
        _parameters.lineHeight = _freetypeFace->height;
        _parameters.ascender = _freetypeFace->ascender;
        _parameters.descender = _freetypeFace->descender;
        _parameters.maxAdvance = _freetypeFace->max_advance_width;

    }
    //--------------------------------------------------------------------------

    void Font::UpdateSizeMetrics() noexcept
    {
        _parameters.sizeMetrics.xPixelsPerEM = _freetypeFace->size->metrics.x_ppem;
        _parameters.sizeMetrics.yPixelsPerEM = _freetypeFace->size->metrics.y_ppem;
        _parameters.sizeMetrics.xScale = _freetypeFace->size->metrics.x_scale >> 16;
        _parameters.sizeMetrics.yScale = _freetypeFace->size->metrics.y_scale >> 16;
        _parameters.sizeMetrics.height = _freetypeFace->size->metrics.height >> 6;
    }
    //--------------------------------------------------------------------------
}