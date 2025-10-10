#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Base/platform.h"
#include "Kmplete/Utils/string_id.h"


namespace Kmplete
{
    namespace Assets
    {
        constexpr static auto AssetsFolder = "Data";
        constexpr static auto ErrorAssetTypeValue = UByte(255);


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


        struct AssetLookupInfo
        {
            Filepath filepath;
            UInt64 bufferSize;
            UInt64 bufferOffset;
        };
        //--------------------------------------------------------------------------
    }
}