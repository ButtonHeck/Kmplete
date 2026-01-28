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

        const TextureFiltering& Texture::GetFiltering() const
        {
            return _filtering;
        }
        //--------------------------------------------------------------------------

        void Texture::SetFiltering(const TextureFiltering& filtering)
        {
            _filtering = filtering;
            _SetFilteringImpl();
        }
        //--------------------------------------------------------------------------
    }
}
