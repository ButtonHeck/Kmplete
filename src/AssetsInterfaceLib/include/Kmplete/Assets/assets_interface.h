#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"
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

        KMP_NODISCARD KMP_API UByte AssetTypeToByte(AssetType type);
        KMP_NODISCARD KMP_API AssetType ByteToAssetType(UByte byte);
        //--------------------------------------------------------------------------

        struct AssetDataEntryHeader
        {
            UByte type;
            Utils::StringID sid;
            UInt64 bufferSize;
            UInt64 bufferOffset;
        };
        //--------------------------------------------------------------------------

        struct AssetDataFileHeader
        {
            UInt32 entryCount;
            Vector<AssetDataEntryHeader> entryHeaders;
        };
        //--------------------------------------------------------------------------


        namespace Compiler
        {
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

                OutputFileIsNotSet = -30,
                OutputFileCreationFailed = -31
            };
        }
    }
}