#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Base/type_traits.h"

#include <random>


namespace Kmplete
{
    enum class RNGBitWidth
    {
        RNGBitWidth32,
        RNGBitWidth64
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


    using RNGInt32 = RNGIntegral<Int32, RNGBitWidth::RNGBitWidth32>;
    using RNGUInt32 = RNGIntegral<UInt32, RNGBitWidth::RNGBitWidth32>;
    using RNGInt64 = RNGIntegral<Int64, RNGBitWidth::RNGBitWidth64>;
    using RNGUint64 = RNGIntegral<UInt64, RNGBitWidth::RNGBitWidth64>;
    using RNGFloat = RNGReal<float, RNGBitWidth::RNGBitWidth32>;
    using RNGDouble = RNGReal<double, RNGBitWidth::RNGBitWidth64>;
}