#pragma once

#include "Kmplete/Core/kmplete_api.h"
#include "Kmplete/Core/filesystem.h"
#include "Kmplete/Core/nullability.h"
#include "Kmplete/Graphics/graphics_base.h"

#include <string>

namespace Kmplete
{
    class Image
    {
    public:
        KMP_DISABLE_COPY(Image)

        KMP_API explicit Image(const Path& filename, bool flipVertically = false);
        KMP_API explicit Image(const char* filename, bool flipVertically = false);
        KMP_API Image(const Path& filename, ImageChannels desiredChannels, bool flipVertically = false);
        KMP_API Image(const char* filename, ImageChannels desiredChannels, bool flipVertically = false);
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
