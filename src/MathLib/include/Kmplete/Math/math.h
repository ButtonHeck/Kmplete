#pragma once

#include "Kmplete/Base/kmplete_api.h"

#include <glm/glm.hpp>
#include <glm/gtc/integer.hpp>
#include <type_traits>


namespace Kmplete
{
    namespace Math
    {
        template<typename ValueType> requires(std::is_arithmetic_v<ValueType> == true)
        KMP_NODISCARD ValueType Clamp(const ValueType& value, const ValueType& min, const ValueType& max)
        {
            return glm::clamp(value, min, max);
        }
        //--------------------------------------------------------------------------

        template<typename ValueType> requires(std::is_arithmetic_v<ValueType> == true)
        KMP_NODISCARD ValueType Min(ValueType value1, ValueType value2)
        {
            return glm::min(value1, value2);
        }
        //--------------------------------------------------------------------------

        template<typename ValueType> requires(std::is_arithmetic_v<ValueType> == true)
        KMP_NODISCARD ValueType Max(ValueType value1, ValueType value2)
        {
            return glm::max(value1, value2);
        }
        //--------------------------------------------------------------------------

        template<typename ValueType> requires(std::is_arithmetic_v<ValueType> == true)
        KMP_NODISCARD ValueType Log2(ValueType value)
        {
            return glm::log2(value);
        }
        //--------------------------------------------------------------------------
    }
}