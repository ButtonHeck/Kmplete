#include "Kmplete/Core/uuid.h"

#include <random>
#include <cstdint>

namespace Kmplete
{
    static std::random_device randomDevice;
    static std::mt19937_64 engine(randomDevice());
    static std::uniform_int_distribution<uint64_t> uniformDistribution;

    UUID UUID::InvalidUuid(0);

    UUID::UUID()
        : _uuid(uniformDistribution(engine))
    {
        while (_uuid == 0)
        {
            _uuid = uniformDistribution(engine);
        }
    }
    //--------------------------------------------------------------------------

    UUID::UUID(uint64_t uuid)
        : _uuid(uuid)
    {}
    //--------------------------------------------------------------------------

    UUID::operator uint64_t() const KMP_NOEXCEPT
    {
        return _uuid;
    }
    //--------------------------------------------------------------------------
}
