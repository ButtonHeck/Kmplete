#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/string_id.h"
#include "Kmplete/Assets/assets_interface.h"
#include "Kmplete/Profile/profiler_fwd.h"


namespace Kmplete
{
    namespace Assets
    {
        //! Base class for all the assets used by application. 
        //! Just as simple as a pair of type enumerator and asset identifier
        class KMP_API Asset
        {
            KMP_PROFILE_CONSTRUCTOR_DECLARE()

        public:
            Asset(AssetType type, StringID sid) noexcept;
            ~Asset() = default;

            KMP_NODISCARD AssetType GetType() const noexcept;
            KMP_NODISCARD StringID GetStringID() const noexcept;

            KMP_NODISCARD bool operator==(const Asset& other) const;

        protected:
            AssetType _type;
            StringID _sid;

        private:
            template <typename T> friend struct std::hash;
        };
        //--------------------------------------------------------------------------
    }
}

namespace std
{
    template<>
    struct hash<Kmplete::Assets::Asset>
    {
        size_t operator()(const Kmplete::Assets::Asset& asset) const
        {
            return asset._sid;
        }
    };
    //--------------------------------------------------------------------------
}