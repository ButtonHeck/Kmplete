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
            , _fontManager(nullptr)
        {
            Initialize(type);

            KMP_PROFILE_CONSTRUCTOR_END();
        }
        //--------------------------------------------------------------------------

        AssetsManager::~AssetsManager()
        {
            KMP_PROFILE_FUNCTION(ProfileLevelAlways);

            Finalize();
        }
        //--------------------------------------------------------------------------

        TextureManager& AssetsManager::GetTextureManager() noexcept
        {
            KMP_ASSERT(_textureManager);
            return *_textureManager;
        }
        //--------------------------------------------------------------------------

        FontManager& AssetsManager::GetFontManager() noexcept
        {
            KMP_ASSERT(_fontManager);
            return *_fontManager;
        }
        //--------------------------------------------------------------------------

        bool AssetsManager::LoadAssetFile(const Filepath& filepath, bool loadBinaries /*= true*/)
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

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
                return LoadAssetFileBinaries(fileBuffer, assetCount);
            }

            return true;
        }
        //--------------------------------------------------------------------------

        bool AssetsManager::LoadAssets(const Vector<Utils::StringID>& assetsSids)
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

            const auto lookupVector = GetSortedByFileAssetsInfos(assetsSids);
            if (lookupVector.empty())
            {
                KMP_LOG_ERROR("failed to load assets - nothing to load");
                return false;
            }

            return LoadAssetsEntriesBinaries(lookupVector);
        }
        //--------------------------------------------------------------------------

        bool AssetsManager::UnloadAssets(const Vector<Utils::StringID>& assetsSids)
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

            Vector<Utils::StringID> textureSidsToRemove;
            Vector<Utils::StringID> fontsSidsToRemove;

            for (const auto& sid : assetsSids)
            {
                if (!_lookupMap.contains(sid))
                {
                    KMP_LOG_WARN("cannot unload asset with sid '{}' - not found", sid);
                    continue;
                }

                const auto assetType = _lookupMap[sid].header.type;
                if (assetType == AssetType::Texture)
                {
                    textureSidsToRemove.push_back(sid);
                }
                else if (assetType == AssetType::FontTTF)
                {
                    fontsSidsToRemove.push_back(sid);
                }
            }

            if (!textureSidsToRemove.empty())
            {
                _textureManager->RemoveTextures(textureSidsToRemove);
            }

            if (!fontsSidsToRemove.empty())
            {
                _fontManager->RemoveFonts(fontsSidsToRemove);
            }

            return true;
        }
        //--------------------------------------------------------------------------

        void AssetsManager::Initialize(GraphicsBackendType type)
        {
            KMP_PROFILE_FUNCTION(ProfileLevelAlways);

            if (!Filesystem::FilepathExists(_dataPath))
            {
                KMP_LOG_CRITICAL("cannot create due to data path '{}' does not exist", _dataPath);
                throw std::runtime_error("AssetsManager: cannot create due to data path does not exist");
            }

            _textureManager.reset(new TextureManager(type));
            _fontManager.reset(new FontManager());
        }
        //--------------------------------------------------------------------------

        void AssetsManager::Finalize()
        {
            KMP_PROFILE_FUNCTION(ProfileLevelAlways);

            _fontManager.reset();
            _textureManager.reset();
        }
        //--------------------------------------------------------------------------

        void AssetsManager::LoadAssetFileHeaders(const BinaryBuffer& fileBuffer, AssetCount assetCount, const Filepath& filepath)
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

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

        bool AssetsManager::LoadAssetFileBinaries(const BinaryBuffer& fileBuffer, AssetCount assetCount)
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

            auto loadedOk = true;
            for (AssetCount i = 0; i < assetCount; i++)
            {
                const auto headerStructBufferOffset = sizeof(assetCount) + i * AssetEntryHeaderStructSize;
                const auto assetHeader = *reinterpret_cast<const AssetEntryHeader*>(fileBuffer.data() + headerStructBufferOffset);

                loadedOk &= LoadAssetEntryBinary(fileBuffer, assetHeader);
            }

            return loadedOk;
        }
        //--------------------------------------------------------------------------

        Vector<AssetLookupInfo> AssetsManager::GetSortedByFileAssetsInfos(const Vector<Utils::StringID>& assetsSids) const
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctionsVerbose);

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

        bool AssetsManager::LoadAssetsEntriesBinaries(const Vector<AssetLookupInfo>& sortedLookupVector)
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

            auto currentFilepath = Filepath();
            auto fileBuffer = BinaryBuffer();
            auto loadedOk = true;

            for (const auto& info : sortedLookupVector)
            {
                if (currentFilepath != _dataPath / info.filepath)
                {
                    currentFilepath = _dataPath / info.filepath;

                    fileBuffer = Filesystem::ReadFileAsBinary(currentFilepath);
                    if (fileBuffer.empty())
                    {
                        KMP_LOG_ERROR("failed to load assets from '{}' - buffer is empty or file not found", info.filepath);
                        return false;
                    }
                }

                loadedOk &= LoadAssetEntryBinary(fileBuffer, info.header);
            }

            return loadedOk;
        }
        //--------------------------------------------------------------------------

        bool AssetsManager::LoadAssetEntryBinary(const BinaryBuffer& fileBuffer, const AssetEntryHeader& assetHeader)
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctionsVerbose);

            if (assetHeader.type == AssetType::Texture)
            {
                const auto assetImage = Image(fileBuffer.data() + assetHeader.bufferOffset, static_cast<int>(assetHeader.bufferSize), ImageChannels::Unknown);
                _textureManager->CreateTexture(assetHeader.sid, assetImage);
            }
            else if (assetHeader.type == AssetType::FontTTF)
            {
                const auto fontDataBuffer = BinaryBuffer(fileBuffer.data() + assetHeader.bufferOffset, fileBuffer.data() + assetHeader.bufferOffset + assetHeader.bufferSize);
                _fontManager->CreateFontTTF(assetHeader.sid, fontDataBuffer);
            }

            return true;
        }
        //--------------------------------------------------------------------------
    }
}