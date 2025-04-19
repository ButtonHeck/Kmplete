#include "Kmplete/Graphics/texture.h"

namespace Kmplete
{
    Texture::Texture(const Path &, Image::Channels, bool)
        : _handle(0)
    {}
    //--------------------------------------------------------------------------

    Texture::Texture(const char*, Image::Channels, bool)
        : _handle(0)
    {}
    //--------------------------------------------------------------------------

    void* Texture::GetHandle() const
    {
        return reinterpret_cast<void*>(_handle);
    }
    //--------------------------------------------------------------------------
}
