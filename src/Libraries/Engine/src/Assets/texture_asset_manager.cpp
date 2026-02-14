#include "Kmplete/Assets/texture_asset_manager.h"
#include "Kmplete/Graphics/texture_factory.h"
#include "Kmplete/Graphics/image.h"
#include "Kmplete/Internal/error_texture_data.h"
#include "Kmplete/Log/log.h"
#include "Kmplete/Filesystem/filesystem.h"
#include "Kmplete/Profile/profiler.h"
#include "Kmplete/Core/assertion.h"

#include <stdexcept>


namespace Kmplete
{
    namespace Assets
    {
        TextureAssetManager::TextureAssetManager(Graphics::GraphicsBackend& graphicsBackend)
            : _graphicsBackend(graphicsBackend)
        {
            if (!_CreateErrorTextureAsset())
            {
                //TODO: uncomment after finishing Vulkan implementation
                //KMP_LOG_CRITICAL("error texture loading failed");
                //throw std::runtime_error("TextureAssetManager: error texture loading failed");
            }
        }
        //--------------------------------------------------------------------------

        bool TextureAssetManager::CreateAsset(StringID textureSid, const Filepath& filepath, bool flipVertically /*= false*/)
        {
            KMP_PROFILE_FUNCTION(ProfileLevelAlways);

            if (!_TextureSidIsValid(textureSid))
            {
                return false;
            }

            const auto backendType = _graphicsBackend.GetType();
            auto* texture = Graphics::TextureFactory::CreateTexture(backendType, filepath, flipVertically);
            if (texture == nullptr)
            {
                KMP_LOG_ERROR("failed to create texture '{}'", filepath);
                return false;
            }

            const auto [iterator, hasEmplaced] = _textures.emplace(textureSid, CreateUPtr<Assets::TextureAsset>(textureSid, texture));
            return hasEmplaced;
        }
        //--------------------------------------------------------------------------

        bool TextureAssetManager::CreateAsset(StringID textureSid, const Graphics::Image& image)
        {
            KMP_PROFILE_FUNCTION(ProfileLevelAlways);
            KMP_ASSERT(image.GetPixels());

            if (!_TextureSidIsValid(textureSid))
            {
                return false;
            }

            const auto backendType = _graphicsBackend.GetType();
            auto* texture = Graphics::TextureFactory::CreateTexture(backendType, image);
            if (texture == nullptr)
            {
                KMP_LOG_ERROR("failed to create texture from image");
                return false;
            }

            const auto [iterator, hasEmplaced] = _textures.emplace(textureSid, CreateUPtr<Assets::TextureAsset>(textureSid, texture));
            return hasEmplaced;
        }
        //--------------------------------------------------------------------------

        const Assets::TextureAsset& TextureAssetManager::GetAsset(StringID textureSid) const
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

        Assets::TextureAsset& TextureAssetManager::GetAsset(StringID textureSid)
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

        void TextureAssetManager::RemoveAssets(const Vector<StringID>& sids)
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

            auto ok = true;
            for (const auto& sid : sids)
            {
                ok &= RemoveAsset(sid);
            }

            if (!ok)
            {
                KMP_LOG_WARN("some textures were not removed");
            }
        }
        //--------------------------------------------------------------------------

        bool TextureAssetManager::RemoveAsset(StringID sid)
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

        UInt64 TextureAssetManager::GetAssetsCount() const noexcept
        {
            return _textures.size();
        }
        //--------------------------------------------------------------------------

        bool TextureAssetManager::_CreateErrorTextureAsset()
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

            if (_textures.contains(ErrorTextureSID))
            {
                KMP_LOG_WARN("error texture already created");
                return false;
            }

            const auto backendType = _graphicsBackend.GetType();
            auto* texture = Graphics::TextureFactory::CreateTexture(backendType, Graphics::Image(&pixelBuffer[0], 32 * 32 * 3, Math::Size2I(32, 32), Graphics::ImageChannels::RGB));
            if (texture == nullptr)
            {
                KMP_LOG_ERROR("error texture failed to load");
                return false;
            }

            const auto [iterator, hasEmplaced] = _textures.emplace(ErrorTextureSID, CreateUPtr<Assets::TextureAsset>(ErrorTextureSID, texture));
            return hasEmplaced;
        }
        //--------------------------------------------------------------------------

        bool TextureAssetManager::_TextureSidIsValid(StringID textureSid)
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
}