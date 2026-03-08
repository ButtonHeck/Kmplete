#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Base/pointers.h"
#include "Kmplete/Base/string_id.h"
#include "Kmplete/Base/nullability.h"
#include "Kmplete/Graphics/texture.h"
#include "Kmplete/Assets/asset.h"


namespace Kmplete
{
    namespace Assets
    {
        //! Asset of a texture type containing single Texture object
        //! @see Texture
        //! @see Assets::Asset
        class TextureAsset : public Asset
        {
        public:
            KMP_API TextureAsset(StringID sid, NonNull<Graphics::Texture*> texture) noexcept;

            KMP_NODISCARD KMP_API const Graphics::Texture& GetTexture() const noexcept;
            KMP_NODISCARD KMP_API Graphics::Texture& GetTexture() noexcept;

        private:
            UPtr<Graphics::Texture> _texture;
        };
        //--------------------------------------------------------------------------
    }
}