#pragma once

#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Base/platform.h"
#include "Kmplete/Base/macro.h"
#include "Kmplete/Utils/string_id.h"


namespace Kmplete
{
    namespace Assets
    {
        static constexpr auto AssetsFolder = "Data";


        enum AssetType : UByte
        {
            Texture,
            Font,
            Sound,
            Error = 255
        };
        //--------------------------------------------------------------------------


        KMP_BEGIN_PACKED_STRUCT(AssetEntryHeader)
        {
            UByte type;
            Utils::StringID sid;
            UInt64 bufferSize;
            UInt64 bufferOffset;
        };
        KMP_END_PACKED_STRUCT

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