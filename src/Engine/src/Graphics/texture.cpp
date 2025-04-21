#include "Kmplete/Graphics/texture.h"

namespace Kmplete
{
    void* Texture::GetHandle() const
    {
        return reinterpret_cast<void*>(_handle);
    }
    //--------------------------------------------------------------------------
}
