#include "Kmplete/Assets/assets_manager.h"
#include "Kmplete/Assets/assets_interface.h"
#include "Kmplete/Graphics/image.h"
#include "Kmplete/Core/assertion.h"
#include "Kmplete/Filesystem/filesystem.h"
#include "Kmplete/Log/log.h"


namespace Kmplete
{
    namespace Assets
    {
        AssetsManager::AssetsManager(const Filepath& applicationPath, GraphicsBackendType type)
            : KMP_PROFILE_CONSTRUCTOR_START_BASE_CLASS("AssetsManager::AssetsManager(const Filepath&, GraphicsBackendType)")
              _dataPath(applicationPath / AssetsFolder)
            , _textureManager(CreateUPtr<TextureManager>(type))
        {
            KMP_PROFILE_CONSTRUCTOR_END();
        }
        //--------------------------------------------------------------------------

        TextureManager& AssetsManager::GetTextureManager()
        {
            KMP_ASSERT(_textureManager);
            return *_textureManager;
        }
        //--------------------------------------------------------------------------

        bool AssetsManager::LoadAssetFile(const Filepath& filepath, bool loadData /*= true*/)
        {
            const auto fullPath = _dataPath / filepath;
            if (!Filesystem::FilepathExists(fullPath))
            {
                KMP_LOG_ERROR("cannot load assets from '{}' - file not found", filepath);
                return false;
            }

            const auto fileBuffer = Filesystem::ReadFileAsBinary(fullPath);
            const auto assetCount = *reinterpret_cast<const AssetCount*>(fileBuffer.data());

            KMP_LOG_INFO("start loading {} assets headers from '{}'", assetCount, filepath);
            Vector<AssetDataEntryHeader> headers;
            headers.reserve(assetCount);
            for (AssetCount i = 0; i < assetCount; i++)
            {
                const auto bufferOffset = sizeof(assetCount) + i * sizeof(AssetDataEntryHeader);
                const auto assetHeader = *reinterpret_cast<const AssetDataEntryHeader*>(fileBuffer.data() + bufferOffset);

                const auto assetSid = assetHeader.sid;
                _lookupTable.emplace(assetSid, AssetLookupInfo{.filepath = filepath, .bufferSize = assetHeader.bufferSize, .bufferOffset = assetHeader.bufferOffset});
            }

            if (loadData)
            {
                KMP_LOG_INFO("start loading assets data from '{}'", filepath);
                for (AssetCount i = 0; i < assetCount; i++)
                {
                    const auto bufferOffset = sizeof(assetCount) + i * sizeof(AssetDataEntryHeader);
                    const auto assetHeader = *reinterpret_cast<const AssetDataEntryHeader*>(fileBuffer.data() + bufferOffset);
                    const auto assetType = assetHeader.type;

                    if (ByteToAssetType(assetType) == AssetType::Texture)
                    {
                        auto assetImage = Image(fileBuffer.data() + assetHeader.bufferOffset, static_cast<int>(assetHeader.bufferSize), ImageChannels::Unknown);
                        _textureManager->CreateTexture(assetHeader.sid, std::move(assetImage));
                    }
                }
            }

            return true;
        }
        //--------------------------------------------------------------------------
    }
}