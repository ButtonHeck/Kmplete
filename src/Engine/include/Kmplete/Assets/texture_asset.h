#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Base/pointers.h"
#include "Kmplete/Base/string_id.h"
#include "Kmplete/Graphics/texture.h"
#include "Kmplete/Assets/asset.h"


namespace Kmplete
{
    namespace Assets
    {
        class TextureAsset : public Asset
        {
        public:
            KMP_API TextureAsset(StringID sid, NonNull<Texture*> texture) noexcept;

            KMP_NODISCARD KMP_API const Texture& GetTexture() const noexcept;
            KMP_NODISCARD KMP_API Texture& GetTexture() noexcept;

        private:
            UPtr<Texture> _texture;
        };
        //--------------------------------------------------------------------------
    }
}