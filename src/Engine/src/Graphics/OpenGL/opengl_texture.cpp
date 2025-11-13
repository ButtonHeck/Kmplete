#include "Kmplete/Graphics/OpenGL/opengl_texture.h"
#include "Kmplete/Graphics/image.h"
#include "Kmplete/Profile/profiler.h"
#include "Kmplete/Math/math.h"
#include "Kmplete/Core/assertion.h"

#include <glad/glad.h>


namespace Kmplete
{
    OpenGLTexture::OpenGLTexture(Utils::StringID sid, const Filepath& filepath, bool flipVertically /*= false*/)
        : Texture(sid)
    {
        KMP_PROFILE_FUNCTION(ProfileLevelAlways);

        try
        {
            Image image(filepath, flipVertically);
            LoadFromImage(image);
        }
        catch (const std::exception&)
        {
            throw;
        }
    }
    //--------------------------------------------------------------------------

    OpenGLTexture::OpenGLTexture(Utils::StringID sid, const Image& image)
        : Texture(sid)
    {
        KMP_PROFILE_FUNCTION(ProfileLevelAlways);
        KMP_ASSERT(image.GetPixels());

        LoadFromImage(image);
    }
    //--------------------------------------------------------------------------

    void OpenGLTexture::LoadFromImage(const Image& image)
    {
        KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);
        KMP_ASSERT(image.GetPixels());

        GLenum internalFormat = GL_RGBA8;
        GLenum dataFormat = GL_RGBA;
        const auto imageChannels = image.GetChannels();
        const auto width = image.GetWidth();
        const auto height = image.GetHeight();
        const auto data = image.GetPixels();

        if (imageChannels == ImageChannels::RGBAlpha)
        {
            internalFormat = GL_RGBA8;
            dataFormat = GL_RGBA;
        }
        else if (imageChannels == ImageChannels::RGB)
        {
            internalFormat = GL_RGB8;
            dataFormat = GL_RGB;
        }
        else if (imageChannels == ImageChannels::Grey)
        {
            internalFormat = GL_R8;
            dataFormat = GL_RED;
        }

        const auto mipLevels = static_cast<GLsizei>(Math::Log2(Math::Max(width, height)) + 1);

        GLuint handle;
        glCreateTextures(GL_TEXTURE_2D, 1, &handle); // TODO texture type abstraction
        glTextureParameteri(handle, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // TODO abstraction
        glTextureParameteri(handle, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // TODO abstraction
        glTextureStorage2D(handle, mipLevels, internalFormat, width, height);
        glTextureSubImage2D(handle, 0, 0, 0, width, height, dataFormat, GL_UNSIGNED_BYTE, data);
        glGenerateTextureMipmap(handle);

        _handle = static_cast<UInt64>(handle);
    }
    //--------------------------------------------------------------------------
}