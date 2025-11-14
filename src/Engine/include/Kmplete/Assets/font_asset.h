#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Graphics/font.h"
#include "Kmplete/Assets/asset.h"
#include "Kmplete/Utils/string_id.h"


struct FT_LibraryRec_;

namespace Kmplete
{
    namespace Assets
    {
        class FontAsset : public Font, public Asset
        {
        public:
            KMP_API FontAsset(Utils::StringID sid, FT_LibraryRec_& freetypeLib, BinaryBuffer&& fontBuffer);
            KMP_API FontAsset(Utils::StringID sid, FT_LibraryRec_& freetypeLib, const Filepath& filepath);
        };
        //--------------------------------------------------------------------------
    }
}