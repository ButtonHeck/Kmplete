#include "Kmplete/Graphics/texture.h"

#include <cmath>


namespace Kmplete
{
    unsigned int Texture::GetMipLevelsCount(int width, int height) noexcept
    {
        return static_cast<unsigned int>(std::log2(std::max(width, height)) + 1);
    }
    //--------------------------------------------------------------------------

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
