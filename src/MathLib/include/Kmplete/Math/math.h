#pragma once

#include "Kmplete/Base/kmplete_api.h"

#include <glm/glm.hpp>


namespace Kmplete
{
    namespace Math
    {
        template<typename ValueType>
        KMP_NODISCARD ValueType Clamp(const ValueType& value, const ValueType& min, const ValueType& max)
        {
            return glm::clamp(value, min, max);
        }
        //--------------------------------------------------------------------------
    }
}