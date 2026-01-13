#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Log/log_class_macro.h"
#include "Kmplete/Profile/profiler.h"


struct FT_LibraryRec_;
struct FT_FaceRec_;


namespace Kmplete
{
    //! Font object class, wrapper for FreeType library "Face" type, can be created both from
    //! a file or from a binary buffer.
    class Font
    {
        KMP_LOG_CLASSNAME(Font)
        KMP_PROFILE_CONSTRUCTOR_DECLARE()
        KMP_DISABLE_COPY_MOVE(Font)

    public:
        KMP_API Font(FT_LibraryRec_& freetypeLib, BinaryBuffer&& fontBuffer);
        KMP_API Font(FT_LibraryRec_& freetypeLib, const Filepath& filepath);
        KMP_API ~Font();

        KMP_NODISCARD KMP_API const BinaryBuffer& GetBuffer() const noexcept;

        KMP_API bool SetPointSize(UInt8 size, UInt32 dpi = 0);
        KMP_API bool SetPixelSize(UInt8 size);

    public:
        struct Parameters
        {
            enum class Style
            {
                Regular    = 0,
                Italic     = 1 << 0,
                Bold       = 1 << 1
            };

            struct SizeMetrics
            {
                UInt16 xPixelsPerEM = 0;
                UInt16 yPixelsPerEM = 0;
                Int32 xScale = 0;
                Int32 yScale = 0;
                Int32 height = 0;
            };

            String familyName = "";
            Style style = Style::Regular;
            bool scalable = true;
            bool monospace = false;
            Int32 numGlyphs = 0;
            UInt16 unitsPerEM = 0;
            Int16 lineHeight = 0;
            Int16 ascender = 0;
            Int16 descender = 0;
            Int16 maxAdvance = 0;
            SizeMetrics sizeMetrics = {};
        };

        KMP_NODISCARD KMP_API const Parameters& GetParameters() const noexcept;

    private:
        void _UpdateParameters() noexcept;
        void _UpdateSizeMetrics() noexcept;

    private:
        FT_FaceRec_* _freetypeFace;
        BinaryBuffer _fontBuffer;
        Parameters _parameters;
    };
    //--------------------------------------------------------------------------
}