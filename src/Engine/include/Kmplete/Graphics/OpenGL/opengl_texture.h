#pragma once

#include "Kmplete/Graphics/texture.h"
#include "Kmplete/Base/types_aliases.h"


namespace Kmplete
{
    class Image;


    class OpenGLTexture : public Texture
    {
    public:
        KMP_API explicit OpenGLTexture(const Filepath& filepath, bool flipVertically = false);
        KMP_API explicit OpenGLTexture(const Image& image);

    private:
        void _LoadFromImage(const Image& image);
    };
    //--------------------------------------------------------------------------
}