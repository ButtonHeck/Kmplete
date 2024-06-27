#pragma once

#include "kmplete_export.h"

#include <cstdint>

namespace Kmplete
{
    class KMPLETE_API UUID
    {
    public:
        static UUID InvalidUuid;

        UUID();
        explicit UUID(uint64_t uuid);

        operator uint64_t() const;

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
