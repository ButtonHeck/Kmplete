#include "Kmplete/Assets/texture_asset.h"
#include "Kmplete/Core/assertion.h"
#include "Kmplete/Profile/profiler.h"


namespace Kmplete
{
    namespace Assets
    {
        TextureAsset::TextureAsset(StringID sid, NonNull<Graphics::Texture*> texture, TextureSubTypeMaskBits subTypeMask) noexcept
            : Asset(AssetType::Texture, sid, subTypeMask)
              KMP_PROFILE_CONSTRUCTOR_START_DERIVED_CLASS()
            , _texture(texture)
        {
            KMP_ASSERT(_texture);
            KMP_PROFILE_CONSTRUCTOR_END()
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