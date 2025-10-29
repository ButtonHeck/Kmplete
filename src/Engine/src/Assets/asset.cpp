#include "Kmplete/Assets/asset.h"


namespace Kmplete
{
    namespace Assets
    {
        Asset::Asset(AssetType type, Utils::StringID sid) noexcept
            : _type(type)
            , _sid(sid)
        {}
        //--------------------------------------------------------------------------

        AssetType Asset::GetType() const noexcept
        {
            return _type;
        }
        //--------------------------------------------------------------------------

        Utils::StringID Asset::GetStringID() const noexcept
        {
            return _sid;
        }
        //--------------------------------------------------------------------------

        bool Asset::operator==(const Asset& other) const
        {
            return _sid == other._sid && _type == other._type;
        }
        //--------------------------------------------------------------------------
    }
}