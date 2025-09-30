#pragma once

#include "Kmplete/Base/kmplete_api.h"
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

        KMP_NODISCARD uint8_t AssetTypeToByte(AssetType type);
        KMP_NODISCARD AssetType ByteToAssetType(uint8_t byte);

        struct AssetFileHeader
        {
            AssetType type;
        };
    }
}