#include "Kmplete/Assets/texture_asset.h"


namespace Kmplete
{
    namespace Assets
    {
        TextureAsset::TextureAsset(StringID sid, NonNull<Texture*> texture) noexcept
            : Asset(AssetType::Texture, sid)
            , _texture(texture)
        {}
        //--------------------------------------------------------------------------

        const Texture& TextureAsset::GetTexture() const noexcept
        {
            return *_texture;
        }
        //--------------------------------------------------------------------------

        Texture& TextureAsset::GetTexture() noexcept
        {
            return *_texture;
        }
        //--------------------------------------------------------------------------
    }
}