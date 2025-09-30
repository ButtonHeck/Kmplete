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

        KMP_NODISCARD Byte AssetTypeToByte(AssetType type);
        KMP_NODISCARD AssetType ByteToAssetType(Byte byte);
        //--------------------------------------------------------------------------

        struct AssetDataEntryHeader
        {
            Byte type;
            Byte tags;
            Utils::StringID sid;
            uint64_t bufferSize;
            uint64_t bufferOffset;
        };
        //--------------------------------------------------------------------------

        struct AssetDataFileHeader
        {
            uint32_t entryCount;
            Vector<AssetDataEntryHeader> entryHeaders;
        };
        //--------------------------------------------------------------------------
    }
}