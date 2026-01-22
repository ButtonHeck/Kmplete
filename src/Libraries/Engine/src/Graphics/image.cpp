#include "Kmplete/Graphics/image.h"
#include "Kmplete/Log/log.h"
#include "Kmplete/Filesystem/filesystem.h"
#include "Kmplete/Profile/profiler.h"

#include <stb_image.h>

#include <cstring>
#include <stdexcept>


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
        KMP_PROFILE_FUNCTION(ProfileLevelAlways);

        stbi_set_flip_vertically_on_load(flipVertically);

        auto channelsInFile = 0;
        _pixels = stbi_load(Filesystem::ToGenericString(filepath).c_str(), &_width, &_height, &channelsInFile, desiredChannels);

        if (!_pixels)
        {
            KMP_LOG_ERROR("'{}' loading error", filepath);
            throw std::runtime_error("Image: failed to load from file");
        }

        _FixChannels(desiredChannels, channelsInFile);

        KMP_LOG_INFO("created [{}x{}] ({} channels) from '{}'", _width, _height, static_cast<int>(_channels), filepath);
    }
    //--------------------------------------------------------------------------

    Image::Image(const UByte* pixelBuffer, int bufferSize, const Math::Size2I& size, ImageChannels channels)
        : _loadedFromFile(false)
        , _width(size.x)
        , _height(size.y)
        , _channels(channels)
        , _pixels(nullptr)
    {
        KMP_PROFILE_FUNCTION(ProfileLevelAlways);

        if (pixelBuffer == nullptr)
        {
            KMP_LOG_ERROR("given pixel buffer is nullptr");
            throw std::runtime_error("Image: given pixel buffer is nullptr");
        }

        if (size.x <= 0 || size.y <= 0 || bufferSize <= 0)
        {
            KMP_LOG_ERROR("image size dimensions or buffer size should not be negative, {}x{}", size.x, size.y);
            throw std::runtime_error("Image: size dimensions or buffer size should not be negative");
        }

        if (bufferSize != size.x * size.y * channels)
        {
            KMP_LOG_ERROR("buffer size {} mismatch with other parameters, {}x{} ({} channels)", bufferSize, size.x, size.y, static_cast<int>(_channels));
            throw std::runtime_error("Image: buffer size mismatch with other parameters");
        }

        _pixels = new UByte[bufferSize];
        std::memcpy(_pixels, pixelBuffer, bufferSize);

        KMP_LOG_INFO("created [{}x{}] ({} channels) from pixel buffer", _width, _height, static_cast<int>(_channels));
    }
    //--------------------------------------------------------------------------

    Image::Image(const UByte* fileBuffer, int bufferSize, ImageChannels desiredChannels, bool flipVertically /*= false*/)
        : _loadedFromFile(true)
        , _width(0)
        , _height(0)
        , _channels(desiredChannels)
        , _pixels(nullptr)
    {
        KMP_PROFILE_FUNCTION(ProfileLevelAlways);

        if (fileBuffer == nullptr)
        {
            KMP_LOG_ERROR("given file buffer is nullptr");
            throw std::runtime_error("Image: given file buffer is nullptr");
        }

        if (bufferSize <= 0)
        {
            KMP_LOG_ERROR("file buffer size should not be negative");
            throw std::runtime_error("Image: file buffer size should not be negative");
        }

        stbi_set_flip_vertically_on_load(flipVertically);

        auto channelsInFile = 0;
        _pixels = stbi_load_from_memory(fileBuffer, bufferSize, &_width, &_height, &channelsInFile, desiredChannels);

        if (!_pixels)
        {
            KMP_LOG_ERROR("file buffer loading error");
            throw std::runtime_error("Image: failed to load from file buffer");
        }

        _FixChannels(desiredChannels, channelsInFile);

        KMP_LOG_INFO("created [{}x{}] ({} channels) from file buffer", _width, _height, static_cast<int>(_channels));
    }
    //--------------------------------------------------------------------------

    Image::~Image()
    {
        KMP_PROFILE_FUNCTION(ProfileLevelAlways);

        _DeleteData();
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
        KMP_PROFILE_FUNCTION(ProfileLevelAlways);

        if (this == &rhs)
        {
            return *this;
        }

        _DeleteData();

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

    void Image::_DeleteData()
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

    void Image::_FixChannels(ImageChannels desiredChannels, int channelsInFile)
    {
        if (channelsInFile != _channels && desiredChannels != ImageChannels::Unknown)
        {
            KMP_LOG_WARN("file buffer channels mismatch (desired: {}, actual: {})", static_cast<int>(_channels), channelsInFile);
        }

        _channels = static_cast<ImageChannels>(channelsInFile);
    }
    //--------------------------------------------------------------------------
}