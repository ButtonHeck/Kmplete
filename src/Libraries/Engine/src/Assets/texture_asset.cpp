#include "Kmplete/Assets/texture_asset.h"
#include "Kmplete/Core/assertion.h"


namespace Kmplete
{
    namespace Assets
    {
        TextureAsset::TextureAsset(StringID sid, NonNull<Graphics::Texture*> texture) noexcept
            : Asset(AssetType::Texture, sid)
            , _texture(texture)
        {
            KMP_ASSERT(_texture);
        }
        //--------------------------------------------------------------------------

        const Graphics::Texture& TextureAsset::GetTexture() const noexcept
        {
            KMP_ASSERT(_texture);

            return *_texture;
        }
        //--------------------------------------------------------------------------

        Graphics::Texture& TextureAsset::GetTexture() noexcept
        {
            KMP_ASSERT(_texture);

            return *_texture;
        }
        //--------------------------------------------------------------------------
    }
}