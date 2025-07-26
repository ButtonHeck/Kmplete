#pragma once

#include "Kmplete/Graphics/texture.h"
#include "Kmplete/Base/types_aliases.h"

namespace Kmplete
{
    class OpenGLTexture : public Texture
    {
    public:
        KMP_API explicit OpenGLTexture(const Filepath& filename, bool flipVertically = false);
        KMP_API explicit OpenGLTexture(const char* filename, bool flipVertically = false);

    private:
        void Load(const char* filename, bool flipVertically);
    };
    //--------------------------------------------------------------------------
}