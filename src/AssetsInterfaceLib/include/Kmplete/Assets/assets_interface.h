#pragma once

#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Base/platform.h"
#include "Kmplete/Utils/string_id.h"


namespace Kmplete
{
    namespace Assets
    {
        constexpr static auto AssetsFolder = "Data";


        enum AssetType : UByte
        {
            Texture,
            Font,
            Sound,
            Error = 255
        };
        //--------------------------------------------------------------------------


#if defined (KMP_COMPILER_MSVC)
        #pragma pack(push, 1)
        struct AssetEntryHeader
#elif defined (KMP_COMPILER_MINGW) || defined (KMP_COMPILER_GCC) || defined (KMP_COMPILER_CLANG)
        struct __attribute__((packed)) AssetEntryHeader
#endif
        {
            UByte type;
            Utils::StringID sid;
            UInt64 bufferSize;
            UInt64 bufferOffset;
        };
#if defined (KMP_COMPILER_MSVC)
        #pragma pack(pop)
#endif

        using AssetCount = UInt32;

        static constexpr auto AssetEntryHeaderStructSize = sizeof(AssetEntryHeader);
        static constexpr auto AssetEntryHeaderOffsetOfBufferSize = offsetof(AssetEntryHeader, bufferSize);
        static constexpr auto AssetEntryHeaderOffsetOfBufferOffset = offsetof(AssetEntryHeader, bufferOffset);
        //--------------------------------------------------------------------------


        struct AssetLookupInfo
        {
            Filepath filepath;
            AssetEntryHeader header;
        };
        //--------------------------------------------------------------------------
    }
}