#include "Kmplete/Internal/error_texture_data.h"
#include "Kmplete/Graphics/texture_manager.h"
#include "Kmplete/Graphics/OpenGL/opengl_texture.h"
#include "Kmplete/Log/log.h"
#include "Kmplete/Filesystem/filesystem.h"

#include <stdexcept>


namespace Kmplete
{
    constexpr static Utils::StringID ErrorTextureSID = 0;

    TextureManager::TextureManager(GraphicsBackendType backendType)
        : _backendType(backendType)
    {
        if (!CreateErrorTexture())
        {
            KMP_LOG_CRITICAL("error texture loading failed");
            throw std::runtime_error("TextureManager: error texture loading failed");
        }
    }
    //--------------------------------------------------------------------------

    bool TextureManager::CreateTexture(Utils::StringID textureSid, const Filepath& filepath, bool flipVertically /*= false*/)
    {
        if (!TextureSidIsValid(textureSid))
        {
            return false;
        }

        UPtr<Texture> texture = nullptr;
        switch (_backendType)
        {
        case GraphicsBackendType::OpenGL:
            texture.reset(new OpenGLTexture(filepath, flipVertically));
            break;

        default:
            break;
        }

        if (!texture)
        {
            KMP_LOG_ERROR("failed to create texture '{}'", filepath);
            return false;
        }

        _textures[textureSid] = std::move(texture);
        return true;
    }
    //--------------------------------------------------------------------------

    bool TextureManager::CreateTexture(Utils::StringID textureSid, Image&& image)
    {
        if (!TextureSidIsValid(textureSid))
        {
            return false;
        }

        UPtr<Texture> texture = nullptr;
        switch (_backendType)
        {
        case GraphicsBackendType::OpenGL:
            texture.reset(new OpenGLTexture(std::move(image)));
            break;

        default:
            break;
        }

        if (!texture)
        {
            KMP_LOG_ERROR("failed to create texture from image");
            return false;
        }

        _textures[textureSid] = std::move(texture);
        return true;
    }
    //--------------------------------------------------------------------------

    Texture& TextureManager::GetTexture(Utils::StringID textureSid)
    {
        if (!_textures.contains(textureSid))
        {
            KMP_LOG_WARN("texture '{}' not found", textureSid);
            return *_textures[ErrorTextureSID];
        }

        return *_textures[textureSid];
    }
    //--------------------------------------------------------------------------

    bool TextureManager::CreateErrorTexture()
    {
        if (_textures.contains(ErrorTextureSID))
        {
            KMP_LOG_WARN("error texture already created");
            return false;
        }

        switch (_backendType)
        {
        case GraphicsBackendType::OpenGL:
            _textures[ErrorTextureSID] = CreateUPtr<OpenGLTexture>(Image(&pixelBuffer[0], 32 * 32 * 3, Math::Size2I(32, 32), ImageChannels::RGB));
            break;

        default:
            break;
        }

        if (!_textures.contains(ErrorTextureSID))
        {
            KMP_LOG_ERROR("error texture failed to load");
            return false;
        }

        return true;
    }
    //--------------------------------------------------------------------------

    bool TextureManager::TextureSidIsValid(Utils::StringID textureSid)
    {
        if (textureSid == ErrorTextureSID)
        {
            KMP_LOG_ERROR("cannot create texture with zero id");
            return false;
        }

        if (_textures.contains(textureSid))
        {
            KMP_LOG_WARN("already contains a texture with SID '{}'", textureSid);
            return false;
        }

        return true;
    }
    //--------------------------------------------------------------------------
}