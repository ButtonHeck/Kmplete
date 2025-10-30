#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Assets/asset.h"
#include "Kmplete/Log/log_class_macro.h"
#include "Kmplete/Profile/profiler.h"


struct FT_LibraryRec_;
struct FT_FaceRec_;

namespace Kmplete
{
    class Font : public Assets::Asset
    {
        KMP_LOG_CLASSNAME(Font)
        KMP_PROFILE_CONSTRUCTOR_DECLARE()

    public:
        KMP_DISABLE_COPY_MOVE(Font)

        KMP_API Font(Utils::StringID sid, FT_LibraryRec_& freetypeLib, BinaryBuffer&& fontBuffer);
        KMP_API ~Font();

        KMP_NODISCARD KMP_API const BinaryBuffer& GetBuffer() const noexcept;

    private:
        FT_FaceRec_* _freetypeFace;
        BinaryBuffer _fontBuffer;
    };
    //--------------------------------------------------------------------------
}