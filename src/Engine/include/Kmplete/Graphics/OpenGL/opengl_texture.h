#pragma once

#include "Kmplete/Graphics/texture.h"

namespace Kmplete
{
    class OpenGLTexture : public Texture
    {
    public:
        KMP_API OpenGLTexture(const Path& filename, ImageChannels desiredChannels, bool flipVertically = false);
        KMP_API OpenGLTexture(const char* filename, ImageChannels desiredChannels, bool flipVertically = false);

    private:
        void Load(const char* filename, ImageChannels desiredChannels, bool flipVertically);
    };
    //--------------------------------------------------------------------------
}