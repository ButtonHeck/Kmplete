#pragma once

#include "Kmplete/Core/kmplete_api.h"

#include <cstdint>

namespace Kmplete
{
    class KMP_API UUID
    {
    public:
        static UUID InvalidUuid;

        UUID();
        explicit UUID(uint64_t uuid);

        KMP_NODISCARD operator uint64_t() const;

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
