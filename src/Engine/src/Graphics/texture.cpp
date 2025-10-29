#include "Kmplete/Graphics/texture.h"


namespace Kmplete
{
    Texture::Texture(Utils::StringID sid) noexcept
        : Assets::Asset(Assets::AssetType::Texture, sid)
        , _handle(0ULL)
    {}
    //--------------------------------------------------------------------------

    Nullable<void*> Texture::GetHandle() const
    {
        return reinterpret_cast<void*>(_handle);
    }
    //--------------------------------------------------------------------------
}
