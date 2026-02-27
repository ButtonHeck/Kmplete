#include "Kmplete/Graphics/texture.h"


namespace Kmplete
{
    namespace Graphics
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
}
