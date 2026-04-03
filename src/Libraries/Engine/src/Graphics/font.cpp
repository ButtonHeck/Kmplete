#include "Kmplete/Graphics/font.h"
#include "Kmplete/Filesystem/filesystem.h"
#include "Kmplete/Log/log.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#include <stdexcept>


namespace Kmplete
{
    namespace Graphics
    {
        static constexpr auto DefaultFontPixelSize = 18;


        Font::Font(FT_LibraryRec_& freetypeLib, BinaryBuffer&& fontBuffer)
            : KMP_PROFILE_CONSTRUCTOR_START_BASE_CLASS()
              _freetypeFace(nullptr)
            , _fontBuffer(std::move(fontBuffer))
        {
            const auto freetypeFontInitError = FT_New_Memory_Face(&freetypeLib, _fontBuffer.data(), static_cast<FT_Long>(_fontBuffer.size()), 0, &_freetypeFace);
            if (freetypeFontInitError)
            {
                KMP_LOG_ERROR("failed to load FreeType font from buffer");
                throw std::runtime_error("Font: failed to load FreeType font from buffer");
            }

            _UpdateParameters();
            SetPixelSize(DefaultFontPixelSize);

            KMP_PROFILE_CONSTRUCTOR_END()
        }
        //--------------------------------------------------------------------------

        Font::Font(FT_LibraryRec_& freetypeLib, const Filepath& filepath)
            : Font(freetypeLib, Filesystem::ReadFileAsBinary(filepath))
        {}
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

        bool Font::SetPointSize(UInt8 size, UInt32 dpi)
        {
            KMP_PROFILE_FUNCTION(ProfileLevelMinorVerbose);

            const auto setSizeError = FT_Set_Char_Size(_freetypeFace, size * 64, 0, dpi, dpi);
            if (setSizeError)
            {
                KMP_LOG_ERROR("failed to set point size to {}", size);
                return false;
            }

            _UpdateSizeMetrics();
            return true;
        }
        //--------------------------------------------------------------------------

        bool Font::SetPixelSize(UInt8 size)
        {
            KMP_PROFILE_FUNCTION(ProfileLevelMinorVerbose);

            const auto setSizeError = FT_Set_Pixel_Sizes(_freetypeFace, size, 0);
            if (setSizeError)
            {
                KMP_LOG_ERROR("failed to set pixel size to {}", size);
                return false;
            }

            _UpdateSizeMetrics();
            return true;
        }
        //--------------------------------------------------------------------------

        const BinaryBuffer& Font::GetBuffer() const noexcept
        {
            return _fontBuffer;
        }
        //--------------------------------------------------------------------------

        const Font::Parameters& Font::GetParameters() const noexcept
        {
            return _parameters;
        }
        //--------------------------------------------------------------------------

        bool Font::HasStyle(Parameters::Style flag) const noexcept
        {
            return (_parameters.style & static_cast<UInt8>(flag)) != 0;
        }
        //--------------------------------------------------------------------------

        void Font::_UpdateParameters() noexcept
        {
            _parameters.familyName = _freetypeFace->family_name;

            const auto style = _freetypeFace->style_flags;
            _parameters.style = Parameters::Style::Regular;
            if (style & FT_STYLE_FLAG_BOLD)
            {
                _parameters.style |= Parameters::Style::Bold;
            }
            if (style & FT_STYLE_FLAG_ITALIC)
            {
                _parameters.style |= Parameters::Style::Italic;
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

        void Font::_UpdateSizeMetrics() noexcept
        {
            _parameters.sizeMetrics.xPixelsPerEM = _freetypeFace->size->metrics.x_ppem;
            _parameters.sizeMetrics.yPixelsPerEM = _freetypeFace->size->metrics.y_ppem;
            _parameters.sizeMetrics.xScale = _freetypeFace->size->metrics.x_scale >> 16;
            _parameters.sizeMetrics.yScale = _freetypeFace->size->metrics.y_scale >> 16;
            _parameters.sizeMetrics.height = _freetypeFace->size->metrics.height >> 6;
        }
        //--------------------------------------------------------------------------
    }
}