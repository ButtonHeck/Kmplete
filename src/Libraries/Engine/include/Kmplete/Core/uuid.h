#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"


namespace Kmplete
{
    //! 64-bit unique identifier based on rng.
    //! An UUID with value 0 treated as invalid.
    //! @see rng.h
    class KMP_API UUID
    {
    public:
        static UUID InvalidUuid;

        UUID();
        explicit UUID(UInt64 uuid) noexcept;
        ~UUID() = default;

        KMP_NODISCARD operator UInt64() const noexcept;

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
