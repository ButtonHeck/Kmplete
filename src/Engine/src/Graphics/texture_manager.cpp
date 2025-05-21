#include "Kmplete/Graphics/texture_manager.h"
#include "Kmplete/Graphics/OpenGL/opengl_texture.h"
#include "Kmplete/Core/log.h"

namespace Kmplete
{
    TextureManager::TextureManager(GraphicsBackendType backendType)
        : _backendType(backendType)
        , _errorTexture(nullptr)
    {}
    //--------------------------------------------------------------------------

    bool TextureManager::CreateTexture(Utils::StringID textureSid, const Path& filename, bool flipVertically /*= false*/)
    {
        if (textureSid == 0)
        {
            return CreateErrorTexture(filename, flipVertically);
        }

        if (_textures.contains(textureSid))
        {
            KMP_LOG_CORE_WARN("TextureManager: already contains a texture with SID '{}'", textureSid);
            return false;
        }

        UPtr<Texture> texture = nullptr;
        switch (_backendType)
        {
        case GraphicsBackendType::OpenGL:
            texture.reset(new OpenGLTexture(filename, flipVertically));
            break;

        default:
            break;
        }

        if (!texture)
        {
            KMP_LOG_CORE_ERROR("TextureManager: failed to create texture '{}'", Filesystem::ToGenericU8String(filename));
            return false;
        }

        _textures[textureSid] = std::move(texture);
        return true;
    }
    //--------------------------------------------------------------------------

    bool TextureManager::CreateErrorTexture(const Path& filename, bool flipVertically)
    {
        if (_errorTexture)
        {
            KMP_LOG_CORE_WARN("TextureManager: error texture already created");
            return false;
        }

        switch (_backendType)
        {
        case GraphicsBackendType::OpenGL:
            _errorTexture.reset(new OpenGLTexture(filename, flipVertically));
            break;

        default:
            break;
        }

        if (!_errorTexture)
        {
            KMP_LOG_CORE_ERROR("TextureManager: error texture failed to load");
            return false;
        }

        return true;
    }
    //--------------------------------------------------------------------------

    Texture& TextureManager::GetTexture(Utils::StringID textureSid)
    {
        if (!_textures.contains(textureSid))
        {
            KMP_LOG_CORE_WARN("TextureManager: texture '{}' not found", textureSid);
            return *_errorTexture;
        }

        return *_textures[textureSid];
    }
    //--------------------------------------------------------------------------
}