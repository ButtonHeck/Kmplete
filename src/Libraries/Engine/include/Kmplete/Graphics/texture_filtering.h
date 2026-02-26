#pragma once

#include "Kmplete/Base/kmplete_api.h"


namespace Kmplete
{
    namespace Graphics
    {
        //! Texture filtering modes that abstract graphics API specific enums
        enum class TextureFilter
        {
            Nearest,
            Linear,
            NearestMipmapNearest,
            LinearMipmapNearest,
            NearestMipmapLinear,
            LinearMipmapLinear
        };
        //--------------------------------------------------------------------------


        //! Class that encapsulates texture filtering parameters and provides
        //! conversion to graphics API specific enums
        class TextureFiltering
        {
        public:
            KMP_API TextureFiltering() noexcept;
            KMP_API TextureFiltering(TextureFilter minFilter, TextureFilter magFilter) noexcept;

            KMP_NODISCARD KMP_API TextureFilter GetMinFilter() const noexcept;
            KMP_NODISCARD KMP_API TextureFilter GetMagFilter() const noexcept;

            KMP_API void SetMinFilter(TextureFilter filter) noexcept;
            KMP_API void SetMagFilter(TextureFilter filter) noexcept;

            KMP_NODISCARD KMP_API bool IsMagFilterValid() const noexcept;

        private:
            TextureFilter _minFilter;
            TextureFilter _magFilter;
        };
        //--------------------------------------------------------------------------
    }
}