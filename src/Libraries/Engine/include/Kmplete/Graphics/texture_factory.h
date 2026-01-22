#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/nullability.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Graphics/graphics_base.h"
#include "Kmplete/Graphics/texture.h"
#include "Kmplete/Log/log_class_macro.h"


namespace Kmplete
{
    class Image;


    //! Factory for creation API-specific textures from either a filepath or an Image object
    //! @see Image
    //! @see Assets::TextureAssetManager for example usage
    class TextureFactory
    {
        KMP_LOG_CLASSNAME(TextureFactory)

    public:
        KMP_NODISCARD KMP_API static Nullable<Texture*> CreateTexture(GraphicsBackendType backendType, const Filepath& filepath, bool flipVertically = false);
        KMP_NODISCARD KMP_API static Nullable<Texture*> CreateTexture(GraphicsBackendType backendType, const Image& image);
    };
    //--------------------------------------------------------------------------
}