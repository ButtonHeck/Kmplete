#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/string_id.h"
#include "Kmplete/Assets/assets_interface.h"


namespace Kmplete
{
    namespace Assets
    {
        //! Base class for all the assets used by application. 
        //! Just as simple as a pair of type enumerator and asset identifier
        class Asset
        {
        public:
            KMP_API Asset(AssetType type, StringID sid) noexcept;

            KMP_NODISCARD KMP_API AssetType GetType() const noexcept;
            KMP_NODISCARD KMP_API StringID GetStringID() const noexcept;

            KMP_NODISCARD KMP_API bool operator==(const Asset& other) const;

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