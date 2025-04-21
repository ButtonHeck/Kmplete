#pragma once

#include "Kmplete/Graphics/texture.h"
#include "Kmplete/Core/filesystem.h"

namespace Kmplete
{
    class OpenGLTexture : public Texture
    {
    public:
        KMP_API explicit OpenGLTexture(const Path& filename, bool flipVertically = false);
        KMP_API explicit OpenGLTexture(const char* filename, bool flipVertically = false);

    private:
        void Load(const char* filename, bool flipVertically);
    };
    //--------------------------------------------------------------------------
}