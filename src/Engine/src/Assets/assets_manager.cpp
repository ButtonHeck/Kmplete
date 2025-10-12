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
            , _textureManager(nullptr)
        {
            Initialize(type);

            KMP_PROFILE_CONSTRUCTOR_END();
        }
        //--------------------------------------------------------------------------

        AssetsManager::~AssetsManager()
        {
            KMP_PROFILE_FUNCTION();

            Finalize();
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
                KMP_LOG_ERROR("cannot load asset file from '{}' - file not found", filepath);
                return false;
            }

            const auto fileBuffer = Filesystem::ReadFileAsBinary(fullPath);
            const auto assetCount = *reinterpret_cast<const AssetCount*>(fileBuffer.data());

            KMP_LOG_INFO("start loading {} assets headers from '{}'", assetCount, filepath);
            LoadAssetFileHeaders(fileBuffer, assetCount, filepath);

            if (loadBinaries)
            {
                KMP_LOG_INFO("start loading {} assets binaries from '{}'", assetCount, filepath);
                LoadAssetFileBinaries(fileBuffer, assetCount);
            }

            return true;
        }
        //--------------------------------------------------------------------------

        void AssetsManager::LoadAssets(const Vector<Utils::StringID>& assetsSids)
        {
            KMP_PROFILE_FUNCTION();

            const auto lookupVector = GetSortedByFileAssetsInfos(assetsSids);
            if (lookupVector.empty())
            {
                KMP_LOG_ERROR("failed to load assets - nothing to load");
                return;
            }

            LoadAssetsEntriesBinaries(lookupVector);
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
                    KMP_LOG_WARN("cannot unload asset with sid '{}' - not found", sid);
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

        void AssetsManager::Initialize(GraphicsBackendType type)
        {
            KMP_PROFILE_FUNCTION();

            if (!Filesystem::FilepathExists(_dataPath))
            {
                KMP_LOG_CRITICAL("cannot create due to data path '{}' does not exist", _dataPath);
                throw std::runtime_error("AssetsManager: cannot create due to data path does not exist");
            }

            _textureManager.reset(new TextureManager(type));
        }
        //--------------------------------------------------------------------------

        void AssetsManager::Finalize()
        {
            _textureManager.reset();
        }
        //--------------------------------------------------------------------------

        void AssetsManager::LoadAssetFileHeaders(const BinaryBuffer& fileBuffer, AssetCount assetCount, const Filepath& filepath)
        {
            KMP_PROFILE_FUNCTION();

            for (AssetCount i = 0; i < assetCount; i++)
            {
                const auto headerStructBufferOffset = sizeof(assetCount) + i * AssetEntryHeaderStructSize;
                const auto assetHeader = *reinterpret_cast<const AssetEntryHeader*>(fileBuffer.data() + headerStructBufferOffset);

                _lookupMap.emplace(assetHeader.sid, AssetLookupInfo{ 
                    .filepath = filepath, 
                    .header = assetHeader
                });
            }
        }
        //--------------------------------------------------------------------------

        void AssetsManager::LoadAssetFileBinaries(const BinaryBuffer& fileBuffer, AssetCount assetCount)
        {
            KMP_PROFILE_FUNCTION();

            for (AssetCount i = 0; i < assetCount; i++)
            {
                const auto headerStructBufferOffset = sizeof(assetCount) + i * AssetEntryHeaderStructSize;
                const auto assetHeader = *reinterpret_cast<const AssetEntryHeader*>(fileBuffer.data() + headerStructBufferOffset);

                LoadAssetEntryBinary(fileBuffer, assetHeader);
            }
        }
        //--------------------------------------------------------------------------

        Vector<AssetLookupInfo> AssetsManager::GetSortedByFileAssetsInfos(const Vector<Utils::StringID>& assetsSids) const
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

                lookupVector.push_back(_lookupMap.at(sid));
            }

            std::sort(lookupVector.begin(), lookupVector.end(), [](const AssetLookupInfo& info1, const AssetLookupInfo& info2) { return info1.filepath > info2.filepath; });

            return lookupVector;
        }
        //--------------------------------------------------------------------------

        void AssetsManager::LoadAssetsEntriesBinaries(const Vector<AssetLookupInfo>& lookupVector)
        {
            auto currentFilepath = Filepath();
            auto fileBuffer = BinaryBuffer();

            for (size_t i = 0; i < lookupVector.size(); i++)
            {
                const auto& info = lookupVector[i];

                if (currentFilepath != _dataPath / info.filepath)
                {
                    currentFilepath = _dataPath / info.filepath;
                    if (!Filesystem::FilepathExists(currentFilepath))
                    {
                        KMP_LOG_ERROR("cannot load assets from '{}' - file not found", info.filepath);
                        return;
                    }

                    fileBuffer = Filesystem::ReadFileAsBinary(currentFilepath);
                    if (fileBuffer.empty())
                    {
                        KMP_LOG_ERROR("failed to load assets from '{}' - buffer is empty", info.filepath);
                        return;
                    }
                }

                LoadAssetEntryBinary(fileBuffer, info.header);
            }
        }
        //--------------------------------------------------------------------------

        void AssetsManager::LoadAssetEntryBinary(const BinaryBuffer& fileBuffer, const AssetEntryHeader& assetHeader)
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