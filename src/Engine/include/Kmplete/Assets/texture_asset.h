#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Base/pointers.h"
#include "Kmplete/Utils/string_id.h"
#include "Kmplete/Graphics/texture.h"
#include "Kmplete/Assets/asset.h"


namespace Kmplete
{
    namespace Assets
    {
        class TextureAsset : public Asset
        {
        public:
            KMP_API TextureAsset(Utils::StringID sid, NonNull<Kmplete::Texture*> texture);

            KMP_NODISCARD KMP_API const Kmplete::Texture& GetTexture() const noexcept;
            KMP_NODISCARD KMP_API Kmplete::Texture& GetTexture() noexcept;

        private:
            UPtr<Kmplete::Texture> _texture;
        };
        //--------------------------------------------------------------------------
    }
}