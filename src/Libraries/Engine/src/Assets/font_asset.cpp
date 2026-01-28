#include "Kmplete/Assets/font_asset.h"


namespace Kmplete
{
    namespace Assets
    {
        FontAsset::FontAsset(StringID sid, FT_LibraryRec_& freetypeLib, BinaryBuffer&& fontBuffer)
            : Asset(AssetType::Font, sid)
            , _font(freetypeLib, std::move(fontBuffer))
        {}
        //--------------------------------------------------------------------------

        FontAsset::FontAsset(StringID sid, FT_LibraryRec_& freetypeLib, const Filepath& filepath)
            : Asset(AssetType::Font, sid)
            , _font(freetypeLib, filepath)
        {}
        //--------------------------------------------------------------------------

        const Graphics::Font& FontAsset::GetFont() const noexcept
        {
            return _font;
        }
        //--------------------------------------------------------------------------

        Graphics::Font& FontAsset::GetFont() noexcept
        {
            return _font;
        }
        //--------------------------------------------------------------------------
    }
}