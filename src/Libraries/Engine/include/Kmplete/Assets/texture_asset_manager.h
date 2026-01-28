#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/pointers.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Base/string_id.h"
#include "Kmplete/Assets/texture_asset.h"
#include "Kmplete/Graphics/graphics_base.h"
#include "Kmplete/Log/log_class_macro.h"


namespace Kmplete
{
    namespace Graphics
    {
        class Image;
    }


    namespace Assets
    {
        //! Manager of texture assets, responsible for managing lifetime of contained asset objects,
        //! adding/deleting texture assets.
        //! If this manager has successfully been created - then there is the asset with StringID = 0 that holds
        //! "Error" texture (little pink/black square)
        //! @see Assets::TextureAsset
        class TextureAssetManager
        {
            KMP_LOG_CLASSNAME(TextureAssetManager)
            KMP_DISABLE_COPY_MOVE(TextureAssetManager)

        public:
            static constexpr StringID ErrorTextureSID = 0;

            KMP_API explicit TextureAssetManager(Graphics::GraphicsBackendType backendType);

            KMP_API bool CreateAsset(StringID textureSid, const Filepath& filepath, bool flipVertically = false);
            KMP_API bool CreateAsset(StringID textureSid, const Graphics::Image& image);

            KMP_NODISCARD KMP_API const Assets::TextureAsset& GetAsset(StringID textureSid) const;
            KMP_NODISCARD KMP_API Assets::TextureAsset& GetAsset(StringID textureSid);

            KMP_API void RemoveAssets(const Vector<StringID>& sids);
            KMP_NODISCARD KMP_API bool RemoveAsset(StringID sid);

            KMP_NODISCARD KMP_API UInt64 GetAssetsCount() const noexcept;

        private:
            KMP_NODISCARD bool _CreateErrorTextureAsset();
            KMP_NODISCARD bool _TextureSidIsValid(StringID textureSid);

        private:
            const Graphics::GraphicsBackendType _backendType;
            HashMap<StringID, UPtr<Assets::TextureAsset>> _textures;
        };
        //--------------------------------------------------------------------------
    }
}