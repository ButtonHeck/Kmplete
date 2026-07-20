#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/nullability.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Base/type_traits.h"
#include "Kmplete/Graphics/graphics_base.h"
#include "Kmplete/Log/log_class_macro.h"
#include "Kmplete/Math/geometry.h"
#include "Kmplete/Profile/profiler_fwd.h"


namespace Kmplete
{
    namespace Graphics
    {
        //! An image object that merely represents a pixel buffer with some common parameters, such as width,
        //! height, channels count. Backed by stb_image
        class KMP_API Image
        {
            KMP_LOG_CLASSNAME(Image)
            KMP_DISABLE_COPY(Image)
            KMP_PROFILE_CONSTRUCTOR_DECLARE()

        public:
            explicit Image(const Filepath& filepath, bool flipVertically = false);
            Image(const Filepath& filepath, ImageChannels desiredChannels, bool flipVertically = false);
            Image(const UByte* pixelBuffer, int bufferSize, const Math::Size2I& size, ImageChannels channels);
            Image(const UByte* fileBuffer, int bufferSize, ImageChannels desiredChannels, bool flipVertically = false);
            ~Image();
            Image(Image&& rhs) noexcept;
            Image& operator=(Image&& rhs) noexcept;

            KMP_NODISCARD int GetWidth() const noexcept;
            KMP_NODISCARD int GetHeight() const noexcept;
            KMP_NODISCARD int GetChannels() const noexcept;
            KMP_NODISCARD Nullable<UByte*> GetPixels() const noexcept;
            KMP_NODISCARD UInt64 GetDataSize() const noexcept;
            KMP_NODISCARD UInt32 GetMipLevels() const noexcept;

        private:
            void _DeleteData();
            void _FixChannels(ImageChannels desiredChannels, int channelsInFile);

        private:
            bool _loadedFromFile;
            int _width;
            int _height;
            ImageChannels _channels;
            UByte* _pixels;
            UInt64 _dataSize;
        };
        //--------------------------------------------------------------------------

        static_assert(IsMoveConstructible<Image>::value);
        static_assert(IsMoveAssignable<Image>::value);
    }
}
