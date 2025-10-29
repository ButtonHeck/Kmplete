#pragma once

#include "Kmplete/Graphics/texture.h"
#include "Kmplete/Base/types_aliases.h"


namespace Kmplete
{
    class Image;

    class OpenGLTexture : public Texture
    {
    public:
        KMP_API OpenGLTexture(Utils::StringID sid, const Filepath& filepath, bool flipVertically = false);
        KMP_API OpenGLTexture(Utils::StringID sid, const Image& image);

    private:
        void LoadFromImage(const Image& image);
    };
    //--------------------------------------------------------------------------
}