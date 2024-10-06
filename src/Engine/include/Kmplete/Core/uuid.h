#pragma once

#include "Kmplete/Core/kmplete_api.h"

#include <cstdint>

namespace Kmplete
{
    class UUID
    {
    public:
        KMP_API static UUID InvalidUuid;

        KMP_API UUID();
        KMP_API explicit UUID(uint64_t uuid) KMP_NOEXCEPT;

        KMP_NODISCARD KMP_API operator uint64_t() const KMP_NOEXCEPT;

    private:
        uint64_t _uuid;
    };
    //--------------------------------------------------------------------------
}

namespace std
{
    template <typename T> struct hash;

    template<>
    struct hash<Kmplete::UUID>
    {
        size_t operator()(const Kmplete::UUID& uuid) const
        {
            return uint64_t(uuid);
        }
    };
    //--------------------------------------------------------------------------
}
