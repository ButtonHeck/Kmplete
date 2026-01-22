#pragma once

#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Base/platform.h"
#include "Kmplete/Base/macro.h"
#include "Kmplete/Base/string_id.h"


namespace Kmplete
{
    namespace Assets
    {
        static constexpr auto AssetsFolder = "Data";


        enum class AssetType : UByte
        {
            Texture,
            Font,
            Sound,
            Error = 255
        };
        //--------------------------------------------------------------------------


        //! Exact representation of an asset metadata (or "header") stored in .kmpdata file
        //! These metadata stored contiguously and located in the beginning of the
        //! .kmpdata file right after the asset count field
        KMP_BEGIN_PACKED_STRUCT(AssetEntryHeader)
        {
            UByte type;
            StringID sid;
            UInt64 bufferSize;
            UInt64 bufferOffset;
        };
        KMP_END_PACKED_STRUCT

        using AssetCount = UInt32;

        static constexpr auto AssetEntryHeaderStructSize = sizeof(AssetEntryHeader);
        static constexpr auto AssetEntryHeaderOffsetOfBufferSize = offsetof(AssetEntryHeader, bufferSize);
        static constexpr auto AssetEntryHeaderOffsetOfBufferOffset = offsetof(AssetEntryHeader, bufferOffset);
        //--------------------------------------------------------------------------


        //! Helper struct to keep mapping between which asset is stored in which file.
        //! During assets loading multiple assets might be spread between
        //! multiple files - sorting them by filepath gives an opportunity to check
        //! every asset file only once as opposed to reopening same files back and forth
        //! @see Assets::AssetsManager
        struct AssetLookupInfo
        {
            Filepath filepath;
            AssetEntryHeader header;
        };
        //--------------------------------------------------------------------------
    }
}