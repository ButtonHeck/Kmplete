#include "Kmplete/Graphics/OpenGL/opengl_texture.h"
#include "Kmplete/Graphics/image.h"

#include <glad/glad.h>

namespace Kmplete
{
    OpenGLTexture::OpenGLTexture(const Path& filename, bool flipVertically)
    {
        Load(Filesystem::ToGenericString(filename).c_str(), flipVertically);
    }
    //--------------------------------------------------------------------------

    OpenGLTexture::OpenGLTexture(const char* filename, bool flipVertically)
    {
        Load(filename, flipVertically);
    }
    //--------------------------------------------------------------------------

    void OpenGLTexture::Load(const char* filename, bool flipVertically)
    {
        Image image(filename, flipVertically);
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

        glTextureStorage2D(handle, 1, internalFormat, width, height);
        glTextureSubImage2D(handle, 0, 0, 0, width, height, dataFormat, GL_UNSIGNED_BYTE, data);

        _handle = static_cast<uint64_t>(handle);
    }
    //--------------------------------------------------------------------------
}