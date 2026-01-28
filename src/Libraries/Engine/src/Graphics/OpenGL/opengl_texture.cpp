#include "Kmplete/Graphics/OpenGL/opengl_texture.h"
#include "Kmplete/Graphics/image.h"
#include "Kmplete/Profile/profiler.h"
#include "Kmplete/Math/math.h"
#include "Kmplete/Core/assertion.h"
#include "Kmplete/Log/log.h"

#include <glad/glad.h>


namespace Kmplete
{
    namespace Graphics
    {
        OpenGLTexture::OpenGLTexture(const Filepath& filepath, bool flipVertically /*= false*/)
            : Texture()
        {
            KMP_PROFILE_FUNCTION(ProfileLevelAlways);

            try
            {
                Image image(filepath, flipVertically);
                _LoadFromImage(image);
            }
            catch (const std::exception& e)
            {
                KMP_LOG_ERROR("failed to load texture from file '{}' - {}", filepath, e.what());
                throw e;
            }
        }
        //--------------------------------------------------------------------------

        OpenGLTexture::OpenGLTexture(const Image& image)
            : Texture()
        {
            KMP_PROFILE_FUNCTION(ProfileLevelAlways);
            KMP_ASSERT(image.GetPixels());

            _LoadFromImage(image);
        }
        //--------------------------------------------------------------------------

        OpenGLTexture::~OpenGLTexture()
        {
            glDeleteTextures(1, reinterpret_cast<GLuint*>(&_handle));
        }
        //--------------------------------------------------------------------------

        void OpenGLTexture::_SetFilteringImpl()
        {
            const auto nativeHandle = static_cast<GLuint>(_handle);
            glTextureParameteri(nativeHandle, GL_TEXTURE_MIN_FILTER, _filtering.ToOpenGL(_filtering.GetMinFilter()));

            if (_filtering.IsMagFilterValid())
            {
                glTextureParameteri(nativeHandle, GL_TEXTURE_MAG_FILTER, _filtering.ToOpenGL(_filtering.GetMagFilter()));
            }
            else
            {
                KMP_LOG_WARN("invalid magnification filter for texture");
            }
        }
        //--------------------------------------------------------------------------

        void OpenGLTexture::_LoadFromImage(const Image& image)
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
            else if (imageChannels == ImageChannels::GreyAlpha)
            {
                internalFormat = GL_RG8;
                dataFormat = GL_RG;
            }
            else if (imageChannels == ImageChannels::Grey)
            {
                internalFormat = GL_R8;
                dataFormat = GL_RED;
            }

            const auto mipLevels = static_cast<GLsizei>(Math::Log2(Math::Max(width, height)).value() + 1);

            GLuint handle;
            glCreateTextures(GL_TEXTURE_2D, 1, &handle); // TODO texture type abstraction
            _handle = static_cast<UInt64>(handle);

            _SetFilteringImpl();
            glTextureStorage2D(handle, mipLevels, internalFormat, width, height);
            glTextureSubImage2D(handle, 0, 0, 0, width, height, dataFormat, GL_UNSIGNED_BYTE, data);
            glGenerateTextureMipmap(handle);
        }
        //--------------------------------------------------------------------------
    }
}