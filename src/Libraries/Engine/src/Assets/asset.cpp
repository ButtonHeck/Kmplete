#include "Kmplete/Assets/asset.h"
#include "Kmplete/Profile/profiler.h"


namespace Kmplete
{
    namespace Assets
    {
        Asset::Asset(AssetType type, StringID sid, AssetSubTypeMask subTypeMask) noexcept
            : KMP_PROFILE_CONSTRUCTOR_START_BASE_CLASS()
              _type(type)
            , _subTypeMask(subTypeMask)
            , _sid(sid)
        {
            KMP_PROFILE_CONSTRUCTOR_END()
        }
        //--------------------------------------------------------------------------

        AssetType Asset::GetType() const noexcept
        {
            return _type;
        }
        //--------------------------------------------------------------------------

        AssetSubTypeMask Asset::GetSubTypeMask() const noexcept
        {
            return _subTypeMask;
        }
        //--------------------------------------------------------------------------

        StringID Asset::GetStringID() const noexcept
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