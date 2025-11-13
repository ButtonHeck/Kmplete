#include "Kmplete/Graphics/texture_manager.h"
#include "Kmplete/Graphics/image.h"
#include "Kmplete/Graphics/OpenGL/opengl_texture.h"
#include "Kmplete/Internal/error_texture_data.h"
#include "Kmplete/Log/log.h"
#include "Kmplete/Filesystem/filesystem.h"
#include "Kmplete/Profile/profiler.h"
#include "Kmplete/Core/assertion.h"

#include <stdexcept>


namespace Kmplete
{
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
        KMP_PROFILE_FUNCTION(ProfileLevelAlways);

        if (!TextureSidIsValid(textureSid))
        {
            return false;
        }

        UPtr<Texture> texture = nullptr;

        try
        {
            switch (_backendType)
            {
            case GraphicsBackendType::OpenGL:
                texture.reset(new OpenGLTexture(textureSid, filepath, flipVertically));
                break;

            default:
                break;
            }
        }
        catch (const std::exception&)
        {
            KMP_LOG_ERROR("failed to create texture '{}'", filepath);
            return false;
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

    bool TextureManager::CreateTexture(Utils::StringID textureSid, const Image& image)
    {
        KMP_PROFILE_FUNCTION(ProfileLevelAlways);
        KMP_ASSERT(image.GetPixels());

        if (!TextureSidIsValid(textureSid))
        {
            return false;
        }

        UPtr<Texture> texture = nullptr;
        switch (_backendType)
        {
        case GraphicsBackendType::OpenGL:
            texture.reset(new OpenGLTexture(textureSid, image));
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

    const Texture& TextureManager::GetTexture(Utils::StringID textureSid) const
    {
        KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

        if (!_textures.contains(textureSid))
        {
            KMP_LOG_WARN("texture '{}' not found", textureSid);
            return *_textures.at(ErrorTextureSID);
        }

        return *_textures.at(textureSid);
    }
    //--------------------------------------------------------------------------

    Texture& TextureManager::GetTexture(Utils::StringID textureSid)
    {
        KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

        if (!_textures.contains(textureSid))
        {
            KMP_LOG_WARN("texture '{}' not found", textureSid);
            return *_textures.at(ErrorTextureSID);
        }

        return *_textures.at(textureSid);
    }
    //--------------------------------------------------------------------------

    void TextureManager::RemoveTextures(const Vector<Utils::StringID>& sids)
    {
        KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

        auto ok = true;
        for (const auto& sid : sids)
        {
            ok &= RemoveTexture(sid);
        }

        if (!ok)
        {
            KMP_LOG_WARN("some textures were not removed");
        }
    }
    //--------------------------------------------------------------------------

    bool TextureManager::RemoveTexture(Utils::StringID sid)
    {
        KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctionsVerbose);

        if (sid == ErrorTextureSID)
        {
            KMP_LOG_WARN("cannot remove texture with reserved sid 0");
            return false;
        }

        if (_textures.erase(sid) == 0)
        {
            KMP_LOG_WARN("not found or failed to remove texture with sid '{}'", sid);
            return false;
        }

        return true;
    }
    //--------------------------------------------------------------------------

    UInt64 TextureManager::TexturesCount() const noexcept
    {
        return _textures.size();
    }
    //--------------------------------------------------------------------------

    bool TextureManager::CreateErrorTexture()
    {
        KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

        if (_textures.contains(ErrorTextureSID))
        {
            KMP_LOG_WARN("error texture already created");
            return false;
        }

        try
        {
            switch (_backendType)
            {
            case GraphicsBackendType::OpenGL:
                _textures[ErrorTextureSID] = CreateUPtr<OpenGLTexture>(ErrorTextureSID, Image(&pixelBuffer[0], 32 * 32 * 3, Math::Size2I(32, 32), ImageChannels::RGB));
                break;

            default:
                break;
            }
        }
        catch (const std::exception&)
        {
            KMP_LOG_ERROR("error texture failed to load");
            return false;
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