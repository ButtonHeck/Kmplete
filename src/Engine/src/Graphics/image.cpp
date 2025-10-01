#include "Kmplete/Graphics/image.h"
#include "Kmplete/Log/log.h"
#include "Kmplete/Filesystem/filesystem.h"

#include <stb_image.h>
#include <cstring>


namespace Kmplete
{
    Image::Image(const Filepath& filepath, bool flipVertically /*= false*/)
        : Image(filepath, ImageChannels::RGB, flipVertically)
    {}
    //--------------------------------------------------------------------------

    Image::Image(const Filepath& filepath, ImageChannels desiredChannels, bool flipVertically /*= false*/)
        : _loadedFromFile(true)
        , _width(0)
        , _height(0)
        , _channels(desiredChannels)
        , _pixels(nullptr)
    {
        stbi_set_flip_vertically_on_load(flipVertically);

        auto channelsInFile = 0;
        _pixels = stbi_load(Filesystem::ToGenericString(filepath).c_str(), &_width, &_height, &channelsInFile, desiredChannels);

        if (channelsInFile != _channels)
        {
            KMP_LOG_WARN("'{}' channels mismatch (desired: {}, actual: {})", filepath, static_cast<int>(_channels), channelsInFile);
            _channels = static_cast<ImageChannels>(channelsInFile);
        }

        if (!_pixels)
        {
            KMP_LOG_ERROR("'{}' loading error", filepath);
        }
        else
        {
            KMP_LOG_INFO("created [{}x{}] ({} channels) from '{}'", _width, _height, static_cast<int>(_channels), filepath);
        }
    }
    //--------------------------------------------------------------------------

    Image::Image(const UByte* pixelBuffer, int bufferSize, const Math::Size2I& size, ImageChannels channels)
        : _loadedFromFile(false)
        , _width(size.x)
        , _height(size.y)
        , _channels(channels)
        , _pixels(nullptr)
    {
        if (pixelBuffer == nullptr)
        {
            KMP_LOG_ERROR("given buffer is nullptr");
            return;
        }

        if (size.x <= 0 || size.y <= 0)
        {
            KMP_LOG_ERROR("size dimensions should not be negative, {}x{}", size.x, size.y);
            return;
        }

        if (bufferSize != size.x * size.y * channels)
        {
            KMP_LOG_ERROR("buffer size ({}) mismatch with other parameters, {}x{} ({} channels)", bufferSize, size.x, size.y, static_cast<int>(_channels));
            return;
        }

        _pixels = new UByte[bufferSize];
        std::memcpy(_pixels, pixelBuffer, bufferSize);

        KMP_LOG_INFO("created [{}x{}] ({} channels) from pixel buffer", _width, _height, static_cast<int>(_channels));
    }
    //--------------------------------------------------------------------------

    Image::~Image()
    {
        DeleteData();
    }
    //--------------------------------------------------------------------------

    Image::Image(Image&& rhs) noexcept
        : _loadedFromFile(rhs._loadedFromFile)
        , _width(rhs._width)
        , _height(rhs._height)
        , _channels(rhs._channels)
        , _pixels(rhs._pixels)
    {
        rhs._pixels = nullptr;
    }
    //--------------------------------------------------------------------------

    Image& Image::operator=(Image&& rhs) noexcept
    {
        DeleteData();

        _loadedFromFile = rhs._loadedFromFile;
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

    Nullable<UByte*> Image::GetPixels() const noexcept
    {
        return _pixels;
    }
    //--------------------------------------------------------------------------

    void Image::DeleteData()
    {
        if (_pixels)
        {
            if (_loadedFromFile)
            {
                stbi_image_free(_pixels);
            }
            else
            {
                delete[] _pixels;
            }
        }
    }
    //--------------------------------------------------------------------------
}