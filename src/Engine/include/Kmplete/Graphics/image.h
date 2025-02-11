#pragma once

#include "Kmplete/Core/kmplete_api.h"
#include "Kmplete/Core/filesystem.h"
#include "Kmplete/Core/nullability.h"

#include <string>

namespace Kmplete
{
    class Image
    {
    public:
        enum Channels
        {
            Grey = 1,
            GreyAlpha,
            RGB,
            RGBAlpha
        };

    public:
        KMP_DISABLE_COPY(Image)

        KMP_API Image(const Path& filename, Channels desiredChannels, bool flipVertically = false);
        KMP_API Image(const char* filename, Channels desiredChannels, bool flipVertically = false);
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
        Channels _channels;
        unsigned char* _pixels;
    };
    //--------------------------------------------------------------------------
}
