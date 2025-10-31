#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/pointers.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Utils/string_id.h"
#include "Kmplete/Graphics/texture.h"
#include "Kmplete/Graphics/graphics_base.h"
#include "Kmplete/Log/log_class_macro.h"


namespace Kmplete
{
    class Image;

    class TextureManager
    {
        KMP_LOG_CLASSNAME(TextureManager)

    public:
        KMP_DISABLE_COPY_MOVE(TextureManager)

        KMP_API explicit TextureManager(GraphicsBackendType backendType);

        KMP_API bool CreateTexture(Utils::StringID textureSid, const Filepath& filepath, bool flipVertically = false);
        KMP_API bool CreateTexture(Utils::StringID textureSid, const Image& image);

        KMP_NODISCARD KMP_API const Texture& GetTexture(Utils::StringID textureSid) const;

        KMP_API void RemoveTextures(const Vector<Utils::StringID>& sids);
        KMP_NODISCARD KMP_API bool RemoveTexture(Utils::StringID sid);

    private:
        KMP_NODISCARD bool CreateErrorTexture();
        KMP_NODISCARD bool TextureSidIsValid(Utils::StringID textureSid);

    private:
        const GraphicsBackendType _backendType;
        HashMap<Utils::StringID, UPtr<Texture>> _textures;
    };
    //--------------------------------------------------------------------------
}