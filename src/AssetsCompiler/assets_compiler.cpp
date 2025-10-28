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
                BinaryBuffer assetsTypes;
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

            ReturnCode AssetsCompiler::WriteHeaderData(JsonDocument& sourceJson, AssetCount assetCount, std::ofstream& outputFile, FilepathVector& assetsFilepaths, BinaryBuffer& assetsTypes) const
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

                    const auto assetType = static_cast<UByte>(sourceJson.GetUInt(JsonConfigurationTypeStr, AssetType::Error));
                    if (assetType == AssetType::Error)
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

                    AssetEntryHeader header{
                        .type = assetType,
                        .sid = assetSid,
                        .bufferSize = 0,
                        .bufferOffset = 0
                    };
                    outputFile.write(reinterpret_cast<const char*>(&header), AssetEntryHeaderStructSize);

                    assetsFilepaths.push_back(Filepath(assetFilename));
                    assetsTypes.push_back(assetType);
                }

                KMP_LOG_INFO("asset headers written: {}", assetCount);

                return ReturnCode::Ok;
            }
            //--------------------------------------------------------------------------

            ReturnCode AssetsCompiler::WriteAssetsData(AssetCount assetCount, std::ofstream& outputFile, FilepathVector& assetsFilepaths, BinaryBuffer& assetsTypes) const
            {
                KMP_LOG_INFO("start writing assets data...");

                const auto headersOffset = sizeof(assetCount);
                WriteBufferState writeState{
                    .assetDataBufferOffset = assetCount * AssetEntryHeaderStructSize + headersOffset,
                    .assetBufferSizeCurrentOffset = headersOffset + AssetEntryHeaderOffsetOfBufferSize,
                    .assetBufferOffsetCurrentOffset = headersOffset + AssetEntryHeaderOffsetOfBufferOffset
                };

                for (UInt32 assetIndex = 0; assetIndex < assetCount; assetIndex++)
                {
                    const auto& assetType = assetsTypes[assetIndex];
                    const auto& assetPath = assetsFilepaths[assetIndex];

                    if (assetType == AssetType::Texture)
                    {
                        const auto writeResult = WriteBinaryBuffer(outputFile, assetPath, writeState, "Texture");
                        if (writeResult != ReturnCode::Ok)
                        {
                            return writeResult;
                        }
                    }
                    else if (assetType == AssetType::FontTTF)
                    {
                        const auto writeResult = WriteBinaryBuffer(outputFile, assetPath, writeState, "FontTTF");
                        if (writeResult != ReturnCode::Ok)
                        {
                            return writeResult;
                        }
                    }
                }

                return ReturnCode::Ok;
            }
            //--------------------------------------------------------------------------

            ReturnCode AssetsCompiler::WriteBinaryBuffer(std::ofstream& outputFile, const Filepath& filepath, WriteBufferState& writeState, KMP_MB_UNUSED const String& assetTypeName) const
            {
                const auto binaryBuffer = Filesystem::ReadFileAsBinary(filepath);
                if (binaryBuffer.empty())
                {
                    KMP_LOG_ERROR("failed to read '{}' data from '{}'", assetTypeName, filepath);
                    return ReturnCode::InputFileProcessingError;
                }

                const auto binaryBufferSize = static_cast<UInt64>(binaryBuffer.size());

                outputFile.write(reinterpret_cast<const char*>(binaryBuffer.data()), binaryBufferSize);
                const auto fileEndPosition = outputFile.tellp();

                outputFile.seekp(writeState.assetBufferSizeCurrentOffset);
                outputFile.write(reinterpret_cast<const char*>(&binaryBufferSize), sizeof(binaryBufferSize));

                outputFile.seekp(writeState.assetBufferOffsetCurrentOffset);
                outputFile.write(reinterpret_cast<char*>(&writeState.assetDataBufferOffset), sizeof(writeState.assetDataBufferOffset));

                KMP_LOG_INFO("write '{}' {}\tbytes at offset {}\t from '{}'", assetTypeName, binaryBufferSize, writeState.assetDataBufferOffset, filepath);

                writeState.assetBufferSizeCurrentOffset += AssetEntryHeaderStructSize;
                writeState.assetBufferOffsetCurrentOffset += AssetEntryHeaderStructSize;
                writeState.assetDataBufferOffset += binaryBufferSize;

                outputFile.seekp(fileEndPosition);

                return ReturnCode::Ok;
            }
            //--------------------------------------------------------------------------
        }
    }
}