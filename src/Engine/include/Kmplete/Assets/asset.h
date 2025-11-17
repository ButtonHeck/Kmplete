#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Assets/assets_interface.h"
#include "Kmplete/Utils/string_id.h"


namespace Kmplete
{
    namespace Assets
    {
        class Asset
        {
        public:
            KMP_API Asset(AssetType type, Utils::StringID sid) noexcept;

            KMP_NODISCARD KMP_API AssetType GetType() const noexcept;
            KMP_NODISCARD KMP_API Utils::StringID GetStringID() const noexcept;

            KMP_NODISCARD KMP_API bool operator==(const Asset& other) const;

        protected:
            AssetType _type;
            Utils::StringID _sid;

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