#include "Kmplete/Assets/font_asset.h"


namespace Kmplete
{
    namespace Assets
    {
        FontAsset::FontAsset(Utils::StringID sid, FT_LibraryRec_& freetypeLib, BinaryBuffer&& fontBuffer)
            : Font(freetypeLib, std::move(fontBuffer))
            , Asset(AssetType::FontTTF, sid)
        {}
        //--------------------------------------------------------------------------

        FontAsset::FontAsset(Utils::StringID sid, FT_LibraryRec_& freetypeLib, const Filepath& filepath)
            : Font(freetypeLib, filepath)
            , Asset(AssetType::FontTTF, sid)
        {}
        //--------------------------------------------------------------------------
    }
}