#include "Kmplete/Assets/font_asset.h"
#include "Kmplete/Profile/profiler.h"


namespace Kmplete
{
    namespace Assets
    {
        FontAsset::FontAsset(StringID sid, FT_LibraryRec_& freetypeLib, BinaryBuffer&& fontBuffer)
            : Asset(AssetType::Font, sid)
              KMP_PROFILE_CONSTRUCTOR_START_DERIVED_CLASS()
            , _font(freetypeLib, std::move(fontBuffer))
        {
            KMP_PROFILE_CONSTRUCTOR_END()
        }
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