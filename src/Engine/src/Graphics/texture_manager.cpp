#include "Kmplete/Graphics/texture_manager.h"
#include "Kmplete/Graphics/error_texture_data.h"
#include "Kmplete/Graphics/OpenGL/opengl_texture.h"
#include "Kmplete/Log/log.h"
#include "Kmplete/Filesystem/filesystem.h"


namespace Kmplete
{
    constexpr static Utils::StringID ErrorTextureSID = 0;

    TextureManager::TextureManager(GraphicsBackendType backendType)
        : _backendType(backendType)
    {}
    //--------------------------------------------------------------------------

    bool TextureManager::CreateTexture(Utils::StringID textureSid, const Filepath& filepath, bool flipVertically /*= false*/)
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
            _textures[ErrorTextureSID] = CreateUPtr<OpenGLTexture>(errorTextureImage);
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
}