#include "Kmplete/Graphics/texture_filtering.h"
#include "Kmplete/Core/assertion.h"


namespace Kmplete
{
    namespace Graphics
    {
        TextureFiltering::TextureFiltering() noexcept
            : _minFilter(TextureFilter::LinearMipmapLinear)
            , _magFilter(TextureFilter::Linear)
        {}
        //--------------------------------------------------------------------------

        TextureFiltering::TextureFiltering(TextureFilter minFilter, TextureFilter magFilter) noexcept
            : _minFilter(minFilter)
            , _magFilter(magFilter)
        {}
        //--------------------------------------------------------------------------

        TextureFilter TextureFiltering::GetMinFilter() const noexcept
        {
            return _minFilter;
        }
        //--------------------------------------------------------------------------

        TextureFilter TextureFiltering::GetMagFilter() const noexcept
        {
            return _magFilter;
        }
        //--------------------------------------------------------------------------

        void TextureFiltering::SetMinFilter(TextureFilter filter) noexcept
        {
            _minFilter = filter;
        }
        //--------------------------------------------------------------------------

        void TextureFiltering::SetMagFilter(TextureFilter filter) noexcept
        {
            _magFilter = filter;
        }
        //--------------------------------------------------------------------------

        bool TextureFiltering::IsMagFilterValid() const noexcept
        {
            return _magFilter == TextureFilter::Nearest || _magFilter == TextureFilter::Linear;
        }
        //--------------------------------------------------------------------------
    }
}