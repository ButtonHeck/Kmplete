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

        KMP_NODISCARD UByte AssetTypeToByte(AssetType type);
        KMP_NODISCARD AssetType ByteToAssetType(UByte byte);
        //--------------------------------------------------------------------------

        struct AssetDataEntryHeader
        {
            UByte type;
            UByte tags;
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
    }
}