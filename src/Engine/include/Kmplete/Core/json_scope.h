#pragma once

#include "Kmplete/Core/kmplete_api.h"
#include "Kmplete/Core/types.h"

#include <string>

namespace Kmplete
{
    struct JsonScope
    {
    public:
        KMP_API void Push(const std::string& entry);
        KMP_API KMP_NODISCARD bool Pop();

        StringVector scope;
        std::string scopeString;
    };
    //--------------------------------------------------------------------------
}