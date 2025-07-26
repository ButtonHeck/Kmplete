#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/nullability.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Graphics/graphics_base.h"

namespace Kmplete
{
    class Image
    {
    public:
        KMP_DISABLE_COPY(Image)

        KMP_API explicit Image(const Filepath& filepath, bool flipVertically = false);
        KMP_API explicit Image(const char* filepath, bool flipVertically = false);
        KMP_API Image(const Filepath& filepath, ImageChannels desiredChannels, bool flipVertically = false);
        KMP_API Image(const char* filepath, ImageChannels desiredChannels, bool flipVertically = false);
        KMP_API ~Image();
        KMP_API Image(Image&& rhs) noexcept;
        KMP_API Image& operator=(Image&& rhs) noexcept;

        KMP_NODISCARD KMP_API int GetWidth() const noexcept;
        KMP_NODISCARD KMP_API int GetHeight() const noexcept;
        KMP_NODISCARD KMP_API int GetChannels() const noexcept;
        KMP_NODISCARD KMP_API Nullable<unsigned char*> GetPixels() const noexcept;

    private:
        int _width;
        int _height;
        ImageChannels _channels;
        unsigned char* _pixels;
    };
    //--------------------------------------------------------------------------
}
