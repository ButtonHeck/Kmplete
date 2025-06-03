#include "Kmplete/Core/uuid.h"
#include "Kmplete/Core/rng.h"

namespace Kmplete
{
    static RNGUint64 rng;

    UUID UUID::InvalidUuid(0);

    UUID::UUID()
        : _uuid(rng.Generate())
    {
        while (_uuid == 0)
        {
            _uuid = rng.Generate();
        }
    }
    //--------------------------------------------------------------------------

    UUID::UUID(uint64_t uuid) noexcept
        : _uuid(uuid)
    {}
    //--------------------------------------------------------------------------

    UUID::operator uint64_t() const noexcept
    {
        return _uuid;
    }
    //--------------------------------------------------------------------------
}
