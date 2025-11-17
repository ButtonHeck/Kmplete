#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"


namespace Kmplete
{
    class UUID
    {
    public:
        KMP_API static UUID InvalidUuid;

        KMP_API UUID();
        KMP_API explicit UUID(UInt64 uuid) noexcept;

        KMP_NODISCARD KMP_API operator UInt64() const noexcept;

    private:
        UInt64 _uuid;
    };
    //--------------------------------------------------------------------------
}

namespace std
{
    template<>
    struct hash<Kmplete::UUID>
    {
        size_t operator()(const Kmplete::UUID& uuid) const
        {
            return Kmplete::UInt64(uuid);
        }
    };
    //--------------------------------------------------------------------------
}
