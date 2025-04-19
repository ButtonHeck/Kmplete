#pragma once

#include "Kmplete/Graphics/texture.h"

namespace Kmplete
{
    class OpenGLTexture : public Texture
    {
    public:
        KMP_API OpenGLTexture(const Path& filename, Image::Channels desiredChannels, bool flipVertically = false);
        KMP_API OpenGLTexture(const char* filename, Image::Channels desiredChannels, bool flipVertically = false);

    private:
        void Load(const char* filename, Image::Channels desiredChannels, bool flipVertically);
    };
    //--------------------------------------------------------------------------
}