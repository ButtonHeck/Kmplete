#pragma once

#include "Kmplete/Core/kmplete_api.h"

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
        KMP_API Image(const std::string& filename, Channels desiredChannels, bool flipVertically = false);
        KMP_API Image(const char* filename, Channels desiredChannels, bool flipVertically = false);
        KMP_API ~Image();

        KMP_NODISCARD KMP_API int GetWidth() const KMP_NOEXCEPT;
        KMP_NODISCARD KMP_API int GetHeight() const KMP_NOEXCEPT;
        KMP_NODISCARD KMP_API int GetChannels() const KMP_NOEXCEPT;
        KMP_NODISCARD KMP_API unsigned char* GetPixels() const KMP_NOEXCEPT;

    private:
        int _width;
        int _height;
        Channels _channels;
        unsigned char* _pixels;
    };
    //--------------------------------------------------------------------------
}
