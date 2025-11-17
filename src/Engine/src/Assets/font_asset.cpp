#include "Kmplete/Assets/font_asset.h"


namespace Kmplete
{
    namespace Assets
    {
        FontAsset::FontAsset(Utils::StringID sid, FT_LibraryRec_& freetypeLib, BinaryBuffer&& fontBuffer)
            : Asset(AssetType::Font, sid)
            , _font(freetypeLib, std::move(fontBuffer))
        {}
        //--------------------------------------------------------------------------

        FontAsset::FontAsset(Utils::StringID sid, FT_LibraryRec_& freetypeLib, const Filepath& filepath)
            : Asset(AssetType::Font, sid)
            , _font(freetypeLib, filepath)
        {}
        //--------------------------------------------------------------------------

        const Font& FontAsset::GetFont() const noexcept
        {
            return _font;
        }
        //--------------------------------------------------------------------------

        Font& FontAsset::GetFont() noexcept
        {
            return _font;
        }
        //--------------------------------------------------------------------------
    }
}