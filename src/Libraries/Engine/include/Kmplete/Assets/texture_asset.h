#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Base/pointers.h"
#include "Kmplete/Base/string_id.h"
#include "Kmplete/Base/nullability.h"
#include "Kmplete/Graphics/texture.h"
#include "Kmplete/Assets/asset.h"
#include "Kmplete/Profile/profiler_fwd.h"


namespace Kmplete
{
    namespace Assets
    {
        //! Asset of a texture type containing single Texture object
        //! @see Texture
        //! @see Assets::Asset
        class KMP_API TextureAsset : public Asset
        {
            KMP_PROFILE_CONSTRUCTOR_DECLARE()

        public:
            TextureAsset(StringID sid, NonNull<Graphics::Texture*> texture, TextureSubTypeMaskBits subTypeMask) noexcept;
            ~TextureAsset() = default;

            KMP_NODISCARD const Graphics::Texture& GetTexture() const noexcept;
            KMP_NODISCARD Graphics::Texture& GetTexture() noexcept;

        private:
            UPtr<Graphics::Texture> _texture;
        };
        //--------------------------------------------------------------------------
    }
}