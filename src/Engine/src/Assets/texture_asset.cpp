#include "Kmplete/Assets/texture_asset.h"


namespace Kmplete
{
    namespace Assets
    {
        TextureAsset::TextureAsset(Utils::StringID sid, NonNull<Kmplete::Texture*> texture) noexcept
            : Asset(AssetType::Texture, sid)
            , _texture(texture)
        {}
        //--------------------------------------------------------------------------

        const Kmplete::Texture& TextureAsset::GetTexture() const noexcept
        {
            return *_texture;
        }
        //--------------------------------------------------------------------------

        Kmplete::Texture& TextureAsset::GetTexture() noexcept
        {
            return *_texture;
        }
        //--------------------------------------------------------------------------
    }
}