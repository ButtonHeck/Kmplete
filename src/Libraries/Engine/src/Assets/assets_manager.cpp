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
        AssetsManager::AssetsManager(const Filepath& applicationPath, Graphics::GraphicsBackendType type)
            : KMP_PROFILE_CONSTRUCTOR_START_BASE_CLASS()
              _dataPath(applicationPath / AssetsFolder)
            , _textureAssetManager(nullptr)
            , _fontAssetManager(nullptr)
        {
            _Initialize(type);

            KMP_PROFILE_CONSTRUCTOR_END();
        }
        //--------------------------------------------------------------------------

        AssetsManager::~AssetsManager()
        {
            KMP_PROFILE_FUNCTION(ProfileLevelAlways);

            _Finalize();
        }
        //--------------------------------------------------------------------------

        const TextureAssetManager& AssetsManager::GetTextureAssetManager() const noexcept
        {
            KMP_ASSERT(_textureAssetManager);
            return *_textureAssetManager;
        }
        //--------------------------------------------------------------------------

        TextureAssetManager& AssetsManager::GetTextureAssetManager() noexcept
        {
            KMP_ASSERT(_textureAssetManager);
            return *_textureAssetManager;
        }
        //--------------------------------------------------------------------------

        const FontAssetManager& AssetsManager::GetFontAssetManager() const noexcept
        {
            KMP_ASSERT(_fontAssetManager);
            return *_fontAssetManager;
        }
        //--------------------------------------------------------------------------

        FontAssetManager& AssetsManager::GetFontAssetManager() noexcept
        {
            KMP_ASSERT(_fontAssetManager);
            return *_fontAssetManager;
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
            if (fileBuffer.size() < sizeof(AssetCount))
            {
                KMP_LOG_ERROR("asset file '{}' buffer is too small", filepath);
                return false;
            }

            const auto assetCount = *reinterpret_cast<const AssetCount*>(fileBuffer.data());

            KMP_LOG_INFO("start loading {} assets headers from '{}'", assetCount, filepath);
            _LoadAssetFileHeaders(fileBuffer, assetCount, filepath);

            if (loadBinaries)
            {
                KMP_LOG_INFO("start loading {} assets binaries from '{}'", assetCount, filepath);
                return _LoadAssetFileBinaries(fileBuffer, assetCount);
            }

            return true;
        }
        //--------------------------------------------------------------------------

        bool AssetsManager::LoadAssets(const Vector<StringID>& assetsSids)
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

            const auto lookupVector = _GetSortedByFileAssetsInfos(assetsSids);
            if (lookupVector.empty())
            {
                KMP_LOG_ERROR("failed to load assets - nothing to load");
                return false;
            }

            return _LoadAssetsEntriesBinaries(lookupVector);
        }
        //--------------------------------------------------------------------------

        bool AssetsManager::UnloadAssets(const Vector<StringID>& assetsSids)
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

            Vector<StringID> textureSidsToRemove;
            Vector<StringID> fontsSidsToRemove;

            for (const auto& sid : assetsSids)
            {
                if (!_lookupMap.contains(sid))
                {
                    KMP_LOG_WARN("cannot unload asset with sid '{}' - not found", sid);
                    continue;
                }

                const auto assetType = _lookupMap[sid].header.type;
                if (assetType == static_cast<UByte>(AssetType::Texture))
                {
                    textureSidsToRemove.push_back(sid);
                }
                else if (assetType == static_cast<UByte>(AssetType::Font))
                {
                    fontsSidsToRemove.push_back(sid);
                }
            }

            if (!textureSidsToRemove.empty())
            {
                _textureAssetManager->RemoveAssets(textureSidsToRemove);
            }

            if (!fontsSidsToRemove.empty())
            {
                _fontAssetManager->RemoveAssets(fontsSidsToRemove);
            }

            return true;
        }
        //--------------------------------------------------------------------------

        void AssetsManager::_Initialize(Graphics::GraphicsBackendType type)
        {
            KMP_PROFILE_FUNCTION(ProfileLevelAlways);

            if (!Filesystem::FilepathExists(_dataPath))
            {
                KMP_LOG_CRITICAL("cannot create due to data path '{}' does not exist", _dataPath);
                throw std::runtime_error("AssetsManager: cannot create due to data path does not exist");
            }

            _textureAssetManager.reset(new TextureAssetManager(type));
            _fontAssetManager.reset(new FontAssetManager());
        }
        //--------------------------------------------------------------------------

        void AssetsManager::_Finalize()
        {
            KMP_PROFILE_FUNCTION(ProfileLevelAlways);

            _fontAssetManager.reset();
            _textureAssetManager.reset();
        }
        //--------------------------------------------------------------------------

        void AssetsManager::_LoadAssetFileHeaders(const BinaryBuffer& fileBuffer, AssetCount assetCount, const Filepath& filepath)
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

            for (AssetCount i = 0; i < assetCount; i++)
            {
                const auto headerStructBufferOffset = sizeof(assetCount) + i * AssetEntryHeaderStructSize;
                const auto assetHeader = *reinterpret_cast<const AssetEntryHeader*>(fileBuffer.data() + headerStructBufferOffset);

                const auto [iterator, hasEmplaced] = _lookupMap.emplace(assetHeader.sid, AssetLookupInfo{ 
                    .filepath = filepath, 
                    .header = assetHeader
                });

                if (!hasEmplaced)
                {
                    KMP_LOG_ERROR("Asset SID duplication detected - file '{}' already registered asset with SID '{}' as currently processed file '{}'", _lookupMap[assetHeader.sid].filepath, assetHeader.sid, filepath);
                }
            }
        }
        //--------------------------------------------------------------------------

        bool AssetsManager::_LoadAssetFileBinaries(const BinaryBuffer& fileBuffer, AssetCount assetCount)
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

            auto loadedOk = true;
            for (AssetCount i = 0; i < assetCount; i++)
            {
                const auto headerStructBufferOffset = sizeof(assetCount) + i * AssetEntryHeaderStructSize;
                const auto assetHeader = *reinterpret_cast<const AssetEntryHeader*>(fileBuffer.data() + headerStructBufferOffset);

                loadedOk &= _LoadAssetEntryBinary(fileBuffer, assetHeader);
            }

            return loadedOk;
        }
        //--------------------------------------------------------------------------

        Vector<AssetLookupInfo> AssetsManager::_GetSortedByFileAssetsInfos(const Vector<StringID>& assetsSids) const
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

            std::sort(lookupVector.begin(), lookupVector.end(), [](const AssetLookupInfo& info1, const AssetLookupInfo& info2) { return info1.filepath < info2.filepath; });

            return lookupVector;
        }
        //--------------------------------------------------------------------------

        bool AssetsManager::_LoadAssetsEntriesBinaries(const Vector<AssetLookupInfo>& sortedLookupVector)
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

                loadedOk &= _LoadAssetEntryBinary(fileBuffer, info.header);
            }

            return loadedOk;
        }
        //--------------------------------------------------------------------------

        bool AssetsManager::_LoadAssetEntryBinary(const BinaryBuffer& fileBuffer, const AssetEntryHeader& assetHeader)
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctionsVerbose);

            if (fileBuffer.size() < assetHeader.bufferOffset + assetHeader.bufferSize) {
                KMP_LOG_ERROR("asset buffer overflow - file too small for asset data");
                return false;
            }

            if (assetHeader.type == static_cast<UByte>(AssetType::Texture))
            {
                try
                {
                    const auto assetImage = Graphics::Image(fileBuffer.data() + assetHeader.bufferOffset, static_cast<int>(assetHeader.bufferSize), Graphics::ImageChannels::Unknown);
                    return _textureAssetManager->CreateAsset(assetHeader.sid, assetImage);
                }
                catch (KMP_MB_UNUSED const std::exception& e)
                {
                    KMP_LOG_ERROR("failed to create texture: {}", e.what());
                    return false;
                }
            }
            else if (assetHeader.type == static_cast<UByte>(AssetType::Font))
            {
                return _fontAssetManager->CreateAsset(assetHeader.sid, BinaryBuffer(fileBuffer.data() + assetHeader.bufferOffset, fileBuffer.data() + assetHeader.bufferOffset + assetHeader.bufferSize));
            }

            KMP_LOG_ERROR("unknown asset type '{}'", assetHeader.type);

            return false;
        }
        //--------------------------------------------------------------------------
    }
}