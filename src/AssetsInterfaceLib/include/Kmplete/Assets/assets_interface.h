#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Base/platform.h"
#include "Kmplete/Utils/string_id.h"


namespace Kmplete
{
    namespace Assets
    {
        enum class AssetType
        {
            Texture,
            Font,
            Sound,
            Error
        };

        constexpr static UByte ErrorAssetTypeValue = 255;

        KMP_NODISCARD KMP_API UByte AssetTypeToByte(AssetType type);
        KMP_NODISCARD KMP_API AssetType ByteToAssetType(UByte byte);
        //--------------------------------------------------------------------------

#if defined (KMP_COMPILER_MSVC)
        #pragma pack(push, 1)
        struct AssetDataEntryHeader
#elif defined (KMP_COMPILER_MINGW) || defined (KMP_COMPILER_GCC) || defined (KMP_COMPILER_CLANG)
        struct __attribute__((packed)) AssetDataEntryHeader
#endif
        {
            UByte type;
            Utils::StringID sid;
            UInt64 bufferSize;
            UInt64 bufferOffset;

            KMP_API void WriteToFile(std::ofstream& outputFile) const;
        };
#if defined (KMP_COMPILER_MSVC)
        #pragma pack(pop)
#endif

        using AssetCount = UInt32;

        static constexpr auto AssetDataEntryHeaderStructSize = sizeof(AssetDataEntryHeader);
        static constexpr auto AssetDataEntryHeaderOffsetOfBufferSize = offsetof(AssetDataEntryHeader, bufferSize);
        static constexpr auto AssetDataEntryHeaderOffsetOfBufferOffset = offsetof(AssetDataEntryHeader, bufferOffset);
        //--------------------------------------------------------------------------


        namespace Compiler
        {
            static constexpr auto JsonConfigurationTopLevelName = "Assets";
            static constexpr auto JsonConfigurationFileStr = "File";
            static constexpr auto JsonConfigurationTypeStr = "Type";
            static constexpr auto JsonConfigurationSidStr = "Sid";

            static constexpr auto CompilerArgumentLogging = "logging";
            static constexpr auto CompilerArgumentLoggingShort = "L";
            static constexpr auto CompilerArgumentInputFileName = "input_file_name";
            static constexpr auto CompilerArgumentInputFileNameShort = "I";
            static constexpr auto CompilerArgumentOutputFileName = "output_file_name";
            static constexpr auto CompilerArgumentOutputFileNameShort = "O";

            enum ReturnCode
            {
                Ok = 0,
                InvalidArgumentCount = -1,

                InputFileIsNotSet = -20,
                InputFileIsNotValid = -21,
                InputFileParsingError = -22,
                InputFileProcessingError = -23,

                OutputFileIsNotSet = -30,
                OutputFileCreationFailed = -31,
                OutputFileOpeningFailed = -32
            };
        }
    }
}