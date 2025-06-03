#include "Kmplete/Base/kmplete_api.h"

#include <random>
#include <concepts>
#include <cstdint>

namespace Kmplete
{
    enum class RNGBitWidth
    {
        RNGBitWidth32,
        RNGBitWidth64
    };
    //--------------------------------------------------------------------------

    template<typename T, RNGBitWidth>
    requires std::integral<T>
    class RNGIntegral
    {};
    //--------------------------------------------------------------------------

    template<typename T>
    requires (sizeof(T) <= 4)
    class RNGIntegral<T, RNGBitWidth::RNGBitWidth32>
    {
    public:
        RNGIntegral()
            : _engine(_randomDevice())
            , _uniformDistribution()
        {}

        RNGIntegral(T min, T max)
            : _engine(_randomDevice())
            , _uniformDistribution(min, max)
        {}

        KMP_NODISCARD T Generate()
        {
            return _uniformDistribution(_engine);
        }

    private:
        std::random_device _randomDevice;
        std::mt19937 _engine;
        std::uniform_int_distribution<T> _uniformDistribution;
    };
    //--------------------------------------------------------------------------

    template<typename T>
    class RNGIntegral<T, RNGBitWidth::RNGBitWidth64>
    {
    public:
        RNGIntegral()
            : _engine(_randomDevice())
            , _uniformDistribution()
        {}

        RNGIntegral(T min, T max)
            : _engine(_randomDevice())
            , _uniformDistribution(min, max)
        {}

        KMP_NODISCARD T Generate()
        {
            return _uniformDistribution(_engine);
        }

    private:
        std::random_device _randomDevice;
        std::mt19937_64 _engine;
        std::uniform_int_distribution<T> _uniformDistribution;
    };
    //--------------------------------------------------------------------------

    template<typename T, RNGBitWidth>
    requires std::floating_point<T>
    class RNGReal
    {};
    //--------------------------------------------------------------------------

    template<typename T>
    requires (sizeof(T) <= 4)
    class RNGReal<T, RNGBitWidth::RNGBitWidth32>
    {
    public:
        RNGReal()
            : _engine(_randomDevice())
            , _uniformDistribution()
        {}

        RNGReal(T min, T max)
            : _engine(_randomDevice())
            , _uniformDistribution(min, max)
        {}

        KMP_NODISCARD T Generate()
        {
            return _uniformDistribution(_engine);
        }

    private:
        std::random_device _randomDevice;
        std::mt19937 _engine;
        std::uniform_real_distribution<T> _uniformDistribution;
    };
    //--------------------------------------------------------------------------

    template<typename T>
    class RNGReal<T, RNGBitWidth::RNGBitWidth64>
    {
    public:
        RNGReal()
            : _engine(_randomDevice())
            , _uniformDistribution()
        {}

        RNGReal(T min, T max)
            : _engine(_randomDevice())
            , _uniformDistribution(min, max)
        {}

        KMP_NODISCARD T Generate()
        {
            return _uniformDistribution(_engine);
        }

    private:
        std::random_device _randomDevice;
        std::mt19937_64 _engine;
        std::uniform_real_distribution<T> _uniformDistribution;
    };
    //--------------------------------------------------------------------------

    using RNGInt32 = RNGIntegral<int32_t, RNGBitWidth::RNGBitWidth32>;
    using RNGUInt32 = RNGIntegral<uint32_t, RNGBitWidth::RNGBitWidth32>;
    using RNGInt64 = RNGIntegral<int64_t, RNGBitWidth::RNGBitWidth64>;
    using RNGUint64 = RNGIntegral<uint64_t, RNGBitWidth::RNGBitWidth64>;
    using RNGFloat = RNGReal<float, RNGBitWidth::RNGBitWidth32>;
    using RNGDouble = RNGReal<double, RNGBitWidth::RNGBitWidth64>;
}