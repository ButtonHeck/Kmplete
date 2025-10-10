#pragma once

#include "Kmplete/Graphics/texture.h"
#include "Kmplete/Graphics/image.h"
#include "Kmplete/Base/types_aliases.h"


namespace Kmplete
{
    class OpenGLTexture : public Texture
    {
    public:
        KMP_API explicit OpenGLTexture(const Filepath& filepath, bool flipVertically = false);
        KMP_API explicit OpenGLTexture(const char* filepath, bool flipVertically = false);
        KMP_API explicit OpenGLTexture(Image&& image);

    private:
        void Load();

    private:
        Image _image;
    };
    //--------------------------------------------------------------------------
}