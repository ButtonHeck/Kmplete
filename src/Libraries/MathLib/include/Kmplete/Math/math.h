#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/type_traits.h"
#include "Kmplete/Base/optional.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/integer.hpp>

#include <bit>


namespace Kmplete
{
    //! Definitions of common math functions backed by the GLM library functions
    namespace Math
    {
        template<typename ValueType> requires (IsArithmetic<ValueType>::value)
        KMP_NODISCARD ValueType Clamp(const ValueType& value, const ValueType& min, const ValueType& max)
        {
            return glm::clamp(value, min, max);
        }
        //--------------------------------------------------------------------------

        template<typename ValueType> requires (IsArithmetic<ValueType>::value)
        KMP_NODISCARD ValueType Min(ValueType value1, ValueType value2)
        {
            return glm::min(value1, value2);
        }
        //--------------------------------------------------------------------------

        template<typename ValueType> requires (IsArithmetic<ValueType>::value)
        KMP_NODISCARD ValueType Max(ValueType value1, ValueType value2)
        {
            return glm::max(value1, value2);
        }
        //--------------------------------------------------------------------------

        template<typename ValueType> requires (IsArithmetic<ValueType>::value)
        KMP_NODISCARD Optional<ValueType> Log2(ValueType value)
        {
            if (value <= ValueType(0))
            {
                return Optional<ValueType>();
            }

            return glm::log2(value);
        }
        //--------------------------------------------------------------------------

        template<typename ValueType> requires (IsArithmetic<ValueType>::value)
        KMP_NODISCARD ValueType Floor(ValueType value)
        {
            return ValueType(glm::floor(value));
        }
        //--------------------------------------------------------------------------

        template<typename ValueType> requires (IsArithmetic<ValueType>::value)
        KMP_NODISCARD ValueType Ceil(ValueType value)
        {
            return ValueType(glm::ceil(value));
        }
        //--------------------------------------------------------------------------

        template<typename ValueType> requires (IsIntegral<ValueType>::value)
        KMP_NODISCARD bool IsPowerOf2(ValueType value)
        {
            return (value == 0) || std::popcount(value) == 1;
        }
        //--------------------------------------------------------------------------

        template<typename ValueType> requires (IsIntegral<ValueType>::value)
        KMP_NODISCARD ValueType NearestPowerOf2(ValueType value)
        {
            if (IsPowerOf2(value))
            {
                return value;
            }

            ValueType lower = std::bit_floor(value);
            ValueType higher = std::bit_ceil(value);

            return (value - lower < higher - value) ? lower : higher;
        }
        //--------------------------------------------------------------------------
    }
}