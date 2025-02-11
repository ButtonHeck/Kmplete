#include "Kmplete/Graphics/image.h"
#include "Kmplete/Core/log.h"

#include <stb_image.h>

namespace Kmplete
{
    Image::Image(const Path& filename, Channels desiredChannels, bool flipVertically)
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
            KMP_LOG_CORE_WARN("Image: '{}' channels mismatch (desired: {}, actual: {})", filename, static_cast<int>(_channels), actualChannels);
            _channels = static_cast<Channels>(actualChannels);
        }

        if (!_pixels)
        {
            KMP_LOG_CORE_ERROR("Image: '{}' loading error", filename);
        }
        else
        {
            KMP_LOG_CORE_INFO("Image: created [{}x{}] ({} channels) from '{}'", _width, _height, static_cast<int>(_channels), filename);
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

    Image::Image(Image&& rhs) noexcept
        : _width(rhs._width)
        , _height(rhs._height)
        , _channels(rhs._channels)
        , _pixels(rhs._pixels)
    {
        rhs._pixels = nullptr;
    }
    //--------------------------------------------------------------------------

    Image& Image::operator=(Image&& rhs) noexcept
    {
        if (_pixels)
        {
            stbi_image_free(_pixels);
        }

        _width = rhs._width;
        _height = rhs._height;
        _channels = rhs._channels;
        _pixels = rhs._pixels;

        rhs._pixels = nullptr;

        return *this;
    }
    //--------------------------------------------------------------------------

    int Image::GetWidth() const noexcept
    {
        return _width;
    }
    //--------------------------------------------------------------------------

    int Image::GetHeight() const noexcept
    {
        return _height;
    }
    //--------------------------------------------------------------------------

    int Image::GetChannels() const noexcept
    {
        return _channels;
    }
    //--------------------------------------------------------------------------

    Nullable<unsigned char*> Image::GetPixels() const noexcept
    {
        return _pixels;
    }
    //--------------------------------------------------------------------------
}