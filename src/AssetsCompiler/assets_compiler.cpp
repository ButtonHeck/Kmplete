#include "assets_compiler.h"

#include "Kmplete/Json/json_document.h"
#include "Kmplete/Filesystem/filesystem.h"
#include "Kmplete/Log/log.h"

#include <fstream>


namespace Kmplete
{
    namespace Assets
    {
        namespace Compiler
        {
            AssetsCompiler::AssetsCompiler(CompilerParameters&& parameters) noexcept
                : _parameters(parameters)
            {}
            //--------------------------------------------------------------------------

            ReturnCode AssetsCompiler::Run() const
            {
                KMP_LOG_INFO("start processing...");

                auto sourceJson = JsonDocument(_parameters.sourceFile);
                if (sourceJson.HasError())
                {
                    KMP_LOG_ERROR("assets description parsing failed");
                    return ReturnCode::InputFileParsingError;
                }

                const AssetCount assetCount = sourceJson.StartGetArray(JsonConfigurationTopLevelName);
                if (assetCount == 0)
                {
                    KMP_LOG_ERROR("required array '{}' not found or this array is empty", JsonConfigurationTopLevelName);
                    return ReturnCode::InputFileFormatError;
                }

                std::ofstream outputFile(_parameters.outputFile, std::ios::binary);
                if (!outputFile.is_open())
                {
                    KMP_LOG_ERROR("failed to open output file '{}'", _parameters.outputFile);
                    return ReturnCode::OutputFileOpeningFailed;
                }

                FilepathVector assetsFilepaths;
                Vector<UByte> assetsTypes;
                assetsFilepaths.reserve(assetCount);
                assetsTypes.reserve(assetCount);

                const auto writeHeadersResult = WriteHeaderData(sourceJson, assetCount, outputFile, assetsFilepaths, assetsTypes);
                if (writeHeadersResult != ReturnCode::Ok)
                {
                    KMP_LOG_ERROR("failed to write assets headers data");
                    return writeHeadersResult;
                }

                if (!sourceJson.EndGetArray())
                {
                    KMP_LOG_ERROR("failed to end assets array");
                    return ReturnCode::InputFileFormatError;
                }

                const auto writeDataResult = WriteAssetsData(assetCount, outputFile, assetsFilepaths, assetsTypes);
                if (writeDataResult != ReturnCode::Ok)
                {
                    KMP_LOG_ERROR("failed to write assets buffers data");
                    return writeDataResult;
                }

                return ReturnCode::Ok;
            }
            //--------------------------------------------------------------------------

            ReturnCode AssetsCompiler::WriteHeaderData(JsonDocument& sourceJson, AssetCount assetCount, std::ofstream& outputFile, FilepathVector& assetsFilepaths, Vector<UByte>& assetsTypes) const
            {
                KMP_LOG_INFO("start writing header data...");

                outputFile.write(reinterpret_cast<const char*>(&assetCount), sizeof(assetCount));

                for (UInt32 assetIndex = 0; assetIndex < assetCount; assetIndex++)
                {
                    if (!sourceJson.StartGetObject(assetIndex))
                    {
                        KMP_LOG_ERROR("failed to get asset json object at index {}", assetIndex);
                        return ReturnCode::InputFileFormatError;
                    }

                    const auto assetFilename = sourceJson.GetString(JsonConfigurationFileStr);
                    if (assetFilename.empty())
                    {
                        KMP_LOG_ERROR("failed to get asset's filename at index {}", assetIndex);
                        return ReturnCode::InputFileFormatError;
                    }

                    const auto assetType = static_cast<UByte>(sourceJson.GetUInt(JsonConfigurationTypeStr, ErrorAssetTypeValue));
                    if (assetType == ErrorAssetTypeValue)
                    {
                        KMP_LOG_ERROR("failed to get asset's type at index {}", assetIndex);
                        return ReturnCode::InputFileFormatError;
                    }

                    const auto assetSid = sourceJson.GetUInt64(JsonConfigurationSidStr);
                    if (assetSid == 0)
                    {
                        KMP_LOG_ERROR("failed to get asset's sid at index {}", assetIndex);
                        return ReturnCode::InputFileFormatError;
                    }

                    if (!sourceJson.EndGetObject())
                    {
                        KMP_LOG_ERROR("failed to end asset json object at index {}", assetIndex);
                        return ReturnCode::InputFileFormatError;
                    }

                    AssetDataEntryHeader header{
                        .type = assetType,
                        .sid = assetSid,
                        .bufferSize = 0,
                        .bufferOffset = 0
                    };
                    header.WriteToFile(outputFile);

                    assetsFilepaths.push_back(Filepath(assetFilename));
                    assetsTypes.push_back(assetType);
                }

                KMP_LOG_INFO("asset headers written: {}", assetCount);

                return ReturnCode::Ok;
            }
            //--------------------------------------------------------------------------

            ReturnCode AssetsCompiler::WriteAssetsData(AssetCount assetCount, std::ofstream& outputFile, FilepathVector& assetsFilepaths, Vector<UByte>& assetsTypes) const
            {
                KMP_LOG_INFO("start writing assets data...");

                const auto headersOffset = sizeof(assetCount);
                UInt64 assetDataBufferOffset = assetCount * AssetDataEntryHeaderStructSize + headersOffset;
                UInt64 assetBufferSizeCurrentOffset = headersOffset + AssetDataEntryHeaderOffsetOfBufferSize;
                UInt64 assetBufferOffsetCurrentOffset = headersOffset + AssetDataEntryHeaderOffsetOfBufferOffset;

                for (UInt32 assetIndex = 0; assetIndex < assetCount; assetIndex++)
                {
                    const auto assetType = ByteToAssetType(assetsTypes[assetIndex]);
                    const auto& assetPath = assetsFilepaths[assetIndex];

                    if (assetType == AssetType::Texture)
                    {
                        const auto textureData = Filesystem::ReadFileAsBinary(assetPath);
                        if (textureData.empty())
                        {
                            KMP_LOG_ERROR("failed to read texture data from '{}'", assetPath);
                            return ReturnCode::InputFileProcessingError;
                        }

                        const auto textureDataSize = static_cast<UInt64>(textureData.size());

                        outputFile.write(reinterpret_cast<const char*>(textureData.data()), textureDataSize);
                        const auto fileEndPosition = outputFile.tellp();

                        outputFile.seekp(assetBufferSizeCurrentOffset);
                        outputFile.write(reinterpret_cast<const char*>(&textureDataSize), sizeof(textureDataSize));

                        outputFile.seekp(assetBufferOffsetCurrentOffset);
                        outputFile.write(reinterpret_cast<char*>(&assetDataBufferOffset), sizeof(assetDataBufferOffset));

                        KMP_LOG_INFO("write texture {}\tbytes at offset {}\t({})", textureDataSize, assetDataBufferOffset, assetPath);

                        assetBufferSizeCurrentOffset += AssetDataEntryHeaderStructSize;
                        assetBufferOffsetCurrentOffset += AssetDataEntryHeaderStructSize;
                        assetDataBufferOffset += textureDataSize;

                        outputFile.seekp(fileEndPosition);
                    }
                }

                return ReturnCode::Ok;
            }
            //--------------------------------------------------------------------------
        }
    }
}