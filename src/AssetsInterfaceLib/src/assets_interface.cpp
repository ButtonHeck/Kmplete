#include "Kmplete/Assets/assets_interface.h"

#include <fstream>


namespace Kmplete
{
    namespace Assets
    {
        UByte AssetTypeToByte(AssetType type)
        {
            switch (type)
            {
            case AssetType::Texture:
                return 0;

            case AssetType::Font:
                return 1;

            case AssetType::Sound:
                return 2;
            }

            return ErrorAssetTypeValue;
        }
        //--------------------------------------------------------------------------

        AssetType ByteToAssetType(UByte byte)
        {
            if (byte == 0)
            {
                return AssetType::Texture;
            }
            else if (byte == 1)
            {
                return AssetType::Font;
            }
            else if (byte == 2)
            {
                return AssetType::Sound;
            }
            else
            {
                return AssetType::Error;
            }
        }
        //--------------------------------------------------------------------------

        void AssetDataEntryHeader::WriteToFile(std::ofstream& outputFile) const
        {
            outputFile.write(reinterpret_cast<const char*>(this), AssetDataEntryHeaderStructSize);
        }
        //--------------------------------------------------------------------------
    }
}