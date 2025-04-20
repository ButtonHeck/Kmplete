#include "Kmplete/Graphics/texture_manager.h"
#include "Kmplete/Graphics/OpenGL/opengl_texture.h"
#include "Kmplete/Core/log.h"

namespace Kmplete
{
    TextureManager::TextureManager(GraphicsBackend::BackendType backendType)
        : _backendType(backendType)
    {}
    //--------------------------------------------------------------------------

    bool TextureManager::CreateTexture(Utils::StringID textureSid, const Path& filename, bool flipVertically)
    {
        if (_textures.contains(textureSid))
        {
            KMP_LOG_CORE_WARN("TextureManager: already contains a texture with SID '{}'", textureSid);
            return false;
        }

        UPtr<Texture> texture = nullptr;
        switch (_backendType)
        {
        case GraphicsBackend::BackendType::OpenGL:
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

    Texture& TextureManager::GetTexture(Utils::StringID textureSid)
    {
        //TODO return default error texture if something went wrong
        return *_textures[textureSid];
    }
    //--------------------------------------------------------------------------
}