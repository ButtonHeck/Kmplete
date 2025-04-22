#pragma once

#include "Kmplete/Core/kmplete_api.h"
#include "Kmplete/Core/pointers.h"
#include "Kmplete/Core/filesystem.h"
#include "Kmplete/Utils/string_id.h"
#include "Kmplete/Graphics/texture.h"
#include "Kmplete/Graphics/graphics_base.h"

#include <unordered_map>

namespace Kmplete
{
    class TextureManager
    {
    public:
        KMP_DISABLE_COPY_MOVE(TextureManager)

        KMP_API explicit TextureManager(GraphicsBackendType backendType);

        KMP_API bool CreateTexture(Utils::StringID textureSid, const Path& filename, bool flipVertically = false);
        KMP_NODISCARD KMP_API Texture& GetTexture(Utils::StringID textureSid);

    private:
        bool CreateErrorTexture(const Path& filename, bool flipVertically);

    private:
        const GraphicsBackendType _backendType;
        UPtr<Texture> _errorTexture;
        std::unordered_map<Utils::StringID, UPtr<Texture>> _textures;
    };
    //--------------------------------------------------------------------------
}