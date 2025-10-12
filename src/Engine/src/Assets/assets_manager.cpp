#include "Kmplete/Assets/assets_manager.h"
#include "Kmplete/Assets/assets_interface.h"
#include "Kmplete/Graphics/image.h"
#include "Kmplete/Core/assertion.h"
#include "Kmplete/Filesystem/filesystem.h"
#include "Kmplete/Log/log.h"

#include <stdexcept>


namespace Kmplete
{
    namespace Assets
    {
        AssetsManager::AssetsManager(const Filepath& applicationPath, GraphicsBackendType type)
            : KMP_PROFILE_CONSTRUCTOR_START_BASE_CLASS("AssetsManager::AssetsManager(const Filepath&, GraphicsBackendType)")
              _dataPath(applicationPath / AssetsFolder)
            , _textureManager(CreateUPtr<TextureManager>(type))
        {
            if (!Filesystem::FilepathExists(_dataPath))
            {
                KMP_LOG_CRITICAL("cannot create due to data path '{}' does not exist", _dataPath);
                throw std::runtime_error("AssetsManager: cannot create due to data path does not exist");
            }

            KMP_PROFILE_CONSTRUCTOR_END();
        }
        //--------------------------------------------------------------------------

        TextureManager& AssetsManager::GetTextureManager() noexcept
        {
            KMP_ASSERT(_textureManager);
            return *_textureManager;
        }
        //--------------------------------------------------------------------------

        bool AssetsManager::LoadAssetFile(const Filepath& filepath, bool loadBinaries /*= true*/)
        {
            KMP_PROFILE_FUNCTION();

            const auto fullPath = _dataPath / filepath;
            if (!Filesystem::FilepathExists(fullPath))
            {
                KMP_LOG_ERROR("cannot load assets from '{}' - file not found", filepath);
                return false;
            }

            const auto fileBuffer = Filesystem::ReadFileAsBinary(fullPath);
            const auto assetCount = *reinterpret_cast<const AssetCount*>(fileBuffer.data());

            KMP_LOG_INFO("start loading {} assets headers from '{}'", assetCount, filepath);
            LoadAssetFileHeaderData(fileBuffer, assetCount, filepath);

            if (loadBinaries)
            {
                KMP_LOG_INFO("start loading assets binaries from '{}'", filepath);
                LoadAssetFileBinaryData(fileBuffer, assetCount);
            }

            return true;
        }
        //--------------------------------------------------------------------------

        void AssetsManager::LoadAssets(const Vector<Utils::StringID>& assetsSids)
        {
            KMP_PROFILE_FUNCTION();

            Vector<AssetLookupInfo> lookupVector;
            lookupVector.reserve(assetsSids.size());

            for (const auto& sid : assetsSids)
            {
                if (!_lookupMap.contains(sid))
                {
                    KMP_LOG_WARN("cannot load asset with sid '{}' - not found", sid);
                    continue;
                }

                lookupVector.push_back(_lookupMap[sid]);
            }

            if (lookupVector.empty())
            {
                KMP_LOG_ERROR("nothing to load");
                return;
            }

            std::sort(lookupVector.begin(), lookupVector.end(), [](const AssetLookupInfo& info1, const AssetLookupInfo& info2) { return info1.filepath > info2.filepath; });

            auto currentFilepath = _dataPath / lookupVector[0].filepath;
            auto fileBuffer = Filesystem::ReadFileAsBinary(currentFilepath);
            if (fileBuffer.empty())
            {
                KMP_LOG_ERROR("failed to load data from '{}'", currentFilepath);
                return;
            }

            for (size_t i = 0; i < lookupVector.size(); i++)
            {
                const auto& info = lookupVector[i];

                if (currentFilepath != _dataPath / info.filepath)
                {
                    currentFilepath = _dataPath / info.filepath;
                    fileBuffer = Filesystem::ReadFileAsBinary(currentFilepath);
                    if (fileBuffer.empty())
                    {
                        KMP_LOG_ERROR("failed to load data from '{}'", currentFilepath);
                        return;
                    }
                }

                LoadAssetBinary(fileBuffer, info.header);
            }
        }
        //--------------------------------------------------------------------------

        void AssetsManager::UnloadAssets(const Vector<Utils::StringID>& assetsSids)
        {
            KMP_PROFILE_FUNCTION();

            for (size_t i = 0; i < assetsSids.size(); i++)
            {
                const auto sid = assetsSids[i];
                if (!_lookupMap.contains(sid))
                {
                    KMP_LOG_WARN("sid '{}' not found", sid);
                    continue;
                }

                Vector<Utils::StringID> textureSidsToRemove;

                const auto assetType = _lookupMap[sid].header.type;
                if (assetType == AssetType::Texture)
                {
                    textureSidsToRemove.push_back(sid);
                }

                if (!textureSidsToRemove.empty())
                {
                    _textureManager->RemoveTextures(textureSidsToRemove);
                }
            }
        }
        //--------------------------------------------------------------------------

        void AssetsManager::LoadAssetFileHeaderData(const BinaryBuffer& fileBuffer, AssetCount assetCount, const Filepath& filepath)
        {
            KMP_PROFILE_FUNCTION();

            for (AssetCount i = 0; i < assetCount; i++)
            {
                const auto headerStructBufferOffset = sizeof(assetCount) + i * AssetDataEntryHeaderStructSize;
                const auto assetHeader = *reinterpret_cast<const AssetDataEntryHeader*>(fileBuffer.data() + headerStructBufferOffset);

                _lookupMap.emplace(assetHeader.sid, AssetLookupInfo{ 
                    .filepath = filepath, 
                    .header = assetHeader
                });
            }
        }
        //--------------------------------------------------------------------------

        void AssetsManager::LoadAssetFileBinaryData(const BinaryBuffer& fileBuffer, AssetCount assetCount)
        {
            KMP_PROFILE_FUNCTION();

            for (AssetCount i = 0; i < assetCount; i++)
            {
                const auto headerStructBufferOffset = sizeof(assetCount) + i * AssetDataEntryHeaderStructSize;
                const auto assetHeader = *reinterpret_cast<const AssetDataEntryHeader*>(fileBuffer.data() + headerStructBufferOffset);

                LoadAssetBinary(fileBuffer, assetHeader);
            }
        }
        //--------------------------------------------------------------------------

        void AssetsManager::LoadAssetBinary(const BinaryBuffer& fileBuffer, const AssetDataEntryHeader& assetHeader)
        {
            if (assetHeader.type == AssetType::Texture)
            {
                const auto assetImage = Image(fileBuffer.data() + assetHeader.bufferOffset, static_cast<int>(assetHeader.bufferSize), ImageChannels::Unknown);
                _textureManager->CreateTexture(assetHeader.sid, assetImage);
            }
        }
        //--------------------------------------------------------------------------
    }
}