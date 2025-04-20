#pragma once

#include "Kmplete/Core/kmplete_api.h"
#include "Kmplete/Core/pointers.h"
#include "Kmplete/Utils/string_id.h"
#include "Kmplete/Graphics/texture.h"
#include "Kmplete/Graphics/graphics_backend.h"

#include <unordered_map>

namespace Kmplete
{
    class TextureManager
    {
    public:
        KMP_DISABLE_COPY_MOVE(TextureManager)

        KMP_API explicit TextureManager(GraphicsBackend::BackendType backendType);

        KMP_NODISCARD KMP_API bool CreateTexture(Utils::StringID textureSid, const Path& filename, bool flipVertically = false);
        KMP_NODISCARD KMP_API Texture& GetTexture(Utils::StringID textureSid);

    private:
        const GraphicsBackend::BackendType _backendType;
        std::unordered_map<Utils::StringID, UPtr<Texture>> _textures;
    };
    //--------------------------------------------------------------------------
}