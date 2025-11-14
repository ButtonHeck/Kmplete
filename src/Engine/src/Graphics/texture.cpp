#include "Kmplete/Graphics/texture.h"


namespace Kmplete
{
    Texture::Texture() noexcept
        : _handle(0ULL)
    {}
    //--------------------------------------------------------------------------

    Nullable<void*> Texture::GetHandle() const
    {
        return reinterpret_cast<void*>(_handle);
    }
    //--------------------------------------------------------------------------
}
