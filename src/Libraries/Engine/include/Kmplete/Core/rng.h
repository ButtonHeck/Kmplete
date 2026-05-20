#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Base/type_traits.h"

#include <random>
#include <limits>


namespace Kmplete
{
    enum class RNGBitWidth
    {
        RNGBitWidth32,
        RNGBitWidth64
    };
    //--------------------------------------------------------------------------


    //! Randomizer engine wrapper based on Mersenne Twister algorithm for generating 32-bit numbers
    struct MersenneTwister32
    {
        MersenneTwister32()
            : randomDevice()
            , engine(randomDevice())
        {}

        std::random_device randomDevice;
        std::mt19937 engine;
    };
    //--------------------------------------------------------------------------

    //! Randomizer engine wrapper based on Mersenne Twister algorithm for generating 64-bit numbers
    struct MersenneTwister64
    {
        MersenneTwister64()
            : randomDevice()
            , engine(randomDevice())
        {}

        std::random_device randomDevice;
        std::mt19937_64 engine;
    };
    //--------------------------------------------------------------------------


    //! Base template class for random integer number generators
    //! parameterized with the type of integer and bit width of the backend generator
    template<typename T, RNGBitWidth> requires (IsIntegral<T>::value)
    class RNGIntegral
    {};
    //--------------------------------------------------------------------------

    template<typename T> requires (sizeof(T) <= 4)
    class RNGIntegral<T, RNGBitWidth::RNGBitWidth32>
    {
    public:
        RNGIntegral()
            : _randomizer()
            , _uniformDistribution(std::numeric_limits<T>::min(), std::numeric_limits<T>::max())
        {}

        RNGIntegral(T min, T max)
            : _randomizer()
            , _uniformDistribution(min, max)
        {}

        KMP_NODISCARD T Generate()
        {
            return _uniformDistribution(_randomizer.engine);
        }

    private:
        MersenneTwister32 _randomizer;
        std::uniform_int_distribution<T> _uniformDistribution;
    };
    //--------------------------------------------------------------------------

    template<typename T>
    class RNGIntegral<T, RNGBitWidth::RNGBitWidth64>
    {
    public:
        RNGIntegral()
            : _randomizer()
            , _uniformDistribution(std::numeric_limits<T>::min(), std::numeric_limits<T>::max())
        {}

        RNGIntegral(T min, T max)
            : _randomizer()
            , _uniformDistribution(min, max)
        {}

        KMP_NODISCARD T Generate()
        {
            return _uniformDistribution(_randomizer.engine);
        }

    private:
        MersenneTwister64 _randomizer;
        std::uniform_int_distribution<T> _uniformDistribution;
    };
    //--------------------------------------------------------------------------


    //! Base template class for random real number generators
    //! parameterized with the type of real number and bit width of the backend generator
    template<typename T, RNGBitWidth> requires (IsReal<T>::value)
    class RNGReal
    {};
    //--------------------------------------------------------------------------

    template<typename T> requires (sizeof(T) <= 4)
    class RNGReal<T, RNGBitWidth::RNGBitWidth32>
    {
    public:
        RNGReal()
            : _randomizer()
            , _uniformDistribution(0.0, 1.0)
        {}

        RNGReal(T min, T max)
            : _randomizer()
            , _uniformDistribution(min, max)
        {}

        KMP_NODISCARD T Generate()
        {
            return _uniformDistribution(_randomizer.engine);
        }

    private:
        MersenneTwister32 _randomizer;
        std::uniform_real_distribution<T> _uniformDistribution;
    };
    //--------------------------------------------------------------------------

    template<typename T>
    class RNGReal<T, RNGBitWidth::RNGBitWidth64>
    {
    public:
        RNGReal()
            : _randomizer()
            , _uniformDistribution(0.0, 1.0)
        {}

        RNGReal(T min, T max)
            : _randomizer()
            , _uniformDistribution(min, max)
        {}

        KMP_NODISCARD T Generate()
        {
            return _uniformDistribution(_randomizer.engine);
        }

    private:
        MersenneTwister64 _randomizer;
        std::uniform_real_distribution<T> _uniformDistribution;
    };
    //--------------------------------------------------------------------------


    using RNGInt32 = RNGIntegral<Int32, RNGBitWidth::RNGBitWidth32>;
    using RNGUInt32 = RNGIntegral<UInt32, RNGBitWidth::RNGBitWidth32>;
    using RNGInt64 = RNGIntegral<Int64, RNGBitWidth::RNGBitWidth64>;
    using RNGUInt64 = RNGIntegral<UInt64, RNGBitWidth::RNGBitWidth64>;
    using RNGFloat = RNGReal<float, RNGBitWidth::RNGBitWidth32>;
    using RNGDouble = RNGReal<double, RNGBitWidth::RNGBitWidth64>;
}