#include "Kmplete/Graphics/OpenGL/opengl_texture.h"
#include "Kmplete/Graphics/image.h"

#include <glad/glad.h>

namespace Kmplete
{
    OpenGLTexture::OpenGLTexture(const Path& filename, ImageChannels desiredChannels, bool flipVertically)
        : Texture(filename, desiredChannels, flipVertically)
    {
        Load(Filesystem::ToGenericString(filename).c_str(), desiredChannels, flipVertically);
    }
    //--------------------------------------------------------------------------

    OpenGLTexture::OpenGLTexture(const char* filename, ImageChannels desiredChannels, bool flipVertically)
        : Texture(filename, desiredChannels, flipVertically)
    {
        Load(filename, desiredChannels, flipVertically);
    }
    //--------------------------------------------------------------------------

    void OpenGLTexture::Load(const char* filename, ImageChannels desiredChannels, bool flipVertically)
    {
        Image image(filename, desiredChannels, flipVertically);
        GLuint handle;
        glCreateTextures(GL_TEXTURE_2D, 1, &handle); // TODO texture type abstraction
        glTextureParameteri(handle, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // TODO abstraction
        glTextureParameteri(handle, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // TODO abstraction

        GLenum internalFormat = GL_RGBA8;
        GLenum dataFormat = GL_RGBA;
        const auto imageChannels = image.GetChannels();
        const auto width = image.GetWidth();
        const auto height = image.GetHeight();
        const auto data = image.GetPixels();
        if (imageChannels == 4)
        {
            internalFormat = GL_RGBA8;
            dataFormat = GL_RGBA;
        }
        else if (imageChannels == 3)
        {
            internalFormat = GL_RGB8;
            dataFormat = GL_RGB;
        }
        else if (imageChannels == 1)
        {
            internalFormat = GL_R8;
            dataFormat = GL_RED;
        }

        glTextureStorage2D(handle, 1, internalFormat, width, height);
        glTextureSubImage2D(handle, 0, 0, 0, width, height, dataFormat, GL_UNSIGNED_BYTE, data);

        _handle = static_cast<uint64_t>(handle);
    }
    //--------------------------------------------------------------------------
}