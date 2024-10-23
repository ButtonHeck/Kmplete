#include "Kmplete/Graphics/image.h"
#include "Kmplete/Core/log.h"
#include "Kmplete/Core/filesystem.h"

#include <stb_image.h>

namespace Kmplete
{
    Image::Image(const std::filesystem::path& filename, Channels desiredChannels, bool flipVertically)
        : Image(Filesystem::ToGenericString(filename).c_str(), desiredChannels, flipVertically)
    {}
    //--------------------------------------------------------------------------

    Image::Image(const char* filename, Channels desiredChannels, bool flipVertically)
        : _width(0)
        , _height(0)
        , _channels(desiredChannels)
        , _pixels(nullptr)
    {
        stbi_set_flip_vertically_on_load(flipVertically);

        auto actualChannels = 0;
        _pixels = stbi_load(filename, &_width, &_height, &actualChannels, _channels);

        if (actualChannels != _channels)
        {
            Log::CoreWarn("Image: '{}' channels mismatch (desired: {}, actual: {})", filename, static_cast<int>(_channels), actualChannels);
            _channels = static_cast<Channels>(actualChannels);
        }

        if (!_pixels)
        {
            Log::CoreError("Image: '{}' loading error", filename);
        }
        else
        {
            Log::CoreDebug("Image: created [{}x{}] ({} channels) from '{}'", _width, _height, static_cast<int>(_channels), filename);
        }
    }
    //--------------------------------------------------------------------------

    Image::~Image()
    {
        if (_pixels)
        {
            stbi_image_free(_pixels);
        }
    }
    //--------------------------------------------------------------------------

    int Image::GetWidth() const KMP_NOEXCEPT
    {
        return _width;
    }
    //--------------------------------------------------------------------------

    int Image::GetHeight() const KMP_NOEXCEPT
    {
        return _height;
    }
    //--------------------------------------------------------------------------

    int Image::GetChannels() const KMP_NOEXCEPT
    {
        return _channels;
    }
    //--------------------------------------------------------------------------

    unsigned char* Image::GetPixels() const KMP_NOEXCEPT
    {
        return _pixels;
    }
    //--------------------------------------------------------------------------
}