#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/nullability.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Graphics/graphics_base.h"
#include "Kmplete/Log/log_class_macro.h"
#include "Kmplete/Math/geometry.h"


namespace Kmplete
{
    class Image
    {
        KMP_LOG_CLASSNAME(Image)

    public:
        KMP_DISABLE_COPY(Image)

        KMP_API explicit Image(const Filepath& filepath, bool flipVertically = false);
        KMP_API Image(const Filepath& filepath, ImageChannels desiredChannels, bool flipVertically = false);
        KMP_API Image(const UByte* pixelBuffer, int bufferSize, const Math::Size2I& size, ImageChannels channels);
        KMP_API Image(const UByte* fileBuffer, int bufferSize, ImageChannels desiredChannels, bool flipVertically = false);
        KMP_API ~Image();
        KMP_API Image(Image&& rhs) noexcept;
        KMP_API Image& operator=(Image&& rhs) noexcept;

        KMP_NODISCARD KMP_API int GetWidth() const noexcept;
        KMP_NODISCARD KMP_API int GetHeight() const noexcept;
        KMP_NODISCARD KMP_API int GetChannels() const noexcept;
        KMP_NODISCARD KMP_API Nullable<UByte*> GetPixels() const noexcept;

    private:
        void DeleteData();
        void FixChannels(ImageChannels desiredChannels, int channelsInFile);

    private:
        bool _loadedFromFile;
        int _width;
        int _height;
        ImageChannels _channels;
        UByte* _pixels;
    };
    //--------------------------------------------------------------------------
}
