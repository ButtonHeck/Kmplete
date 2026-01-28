#include "Kmplete/Graphics/texture_filtering.h"
#include "Kmplete/Core/assertion.h"

#include <glad/glad.h>


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

        int TextureFiltering::ToOpenGL(TextureFilter filter) const
        {
            switch (filter)
            {
                case TextureFilter::Nearest:
                    return GL_NEAREST;
                case TextureFilter::Linear:
                    return GL_LINEAR;
                case TextureFilter::NearestMipmapNearest:
                    return GL_NEAREST_MIPMAP_NEAREST;
                case TextureFilter::LinearMipmapNearest:
                    return GL_LINEAR_MIPMAP_NEAREST;
                case TextureFilter::NearestMipmapLinear:
                    return GL_NEAREST_MIPMAP_LINEAR;
                case TextureFilter::LinearMipmapLinear:
                    return GL_LINEAR_MIPMAP_LINEAR;
                default:
                    KMP_ASSERT(false && "Invalid texture filter");
                    return GL_LINEAR;
            }
        }
        //--------------------------------------------------------------------------

        bool TextureFiltering::IsMagFilterValid() const noexcept
        {
            return _magFilter == TextureFilter::Nearest || _magFilter == TextureFilter::Linear;
        }
        //--------------------------------------------------------------------------
    }
}