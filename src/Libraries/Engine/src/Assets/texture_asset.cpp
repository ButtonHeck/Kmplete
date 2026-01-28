#include "Kmplete/Assets/texture_asset.h"


namespace Kmplete
{
    namespace Assets
    {
        TextureAsset::TextureAsset(StringID sid, NonNull<Graphics::Texture*> texture) noexcept
            : Asset(AssetType::Texture, sid)
            , _texture(texture)
        {}
        //--------------------------------------------------------------------------

        const Graphics::Texture& TextureAsset::GetTexture() const noexcept
        {
            return *_texture;
        }
        //--------------------------------------------------------------------------

        Graphics::Texture& TextureAsset::GetTexture() noexcept
        {
            return *_texture;
        }
        //--------------------------------------------------------------------------
    }
}