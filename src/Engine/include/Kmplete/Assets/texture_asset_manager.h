#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/pointers.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Utils/string_id.h"
#include "Kmplete/Assets/texture_asset.h"
#include "Kmplete/Graphics/graphics_base.h"
#include "Kmplete/Log/log_class_macro.h"


namespace Kmplete
{
    class Image;


    namespace Assets
    {
        class TextureAssetManager
        {
            KMP_LOG_CLASSNAME(TextureAssetManager)

        public:
            static constexpr Utils::StringID ErrorTextureSID = 0;

            KMP_DISABLE_COPY_MOVE(TextureAssetManager)

            KMP_API explicit TextureAssetManager(GraphicsBackendType backendType);

            KMP_API bool CreateAsset(Utils::StringID textureSid, const Filepath& filepath, bool flipVertically = false);
            KMP_API bool CreateAsset(Utils::StringID textureSid, const Image& image);

            KMP_NODISCARD KMP_API const Assets::TextureAsset& GetAsset(Utils::StringID textureSid) const;
            KMP_NODISCARD KMP_API Assets::TextureAsset& GetAsset(Utils::StringID textureSid);

            KMP_API void RemoveAssets(const Vector<Utils::StringID>& sids);
            KMP_NODISCARD KMP_API bool RemoveAsset(Utils::StringID sid);

            KMP_NODISCARD KMP_API UInt64 GetAssetsCount() const noexcept;

        private:
            KMP_NODISCARD bool _CreateErrorTextureAsset();
            KMP_NODISCARD bool _TextureSidIsValid(Utils::StringID textureSid);

        private:
            const GraphicsBackendType _backendType;
            HashMap<Utils::StringID, UPtr<Assets::TextureAsset>> _textures;
        };
        //--------------------------------------------------------------------------
    }
}