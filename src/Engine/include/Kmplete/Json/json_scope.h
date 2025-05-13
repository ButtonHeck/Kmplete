#pragma once

#include "Kmplete/Core/kmplete_api.h"
#include "Kmplete/Core/types.h"

namespace Kmplete
{
    struct JsonScope
    {
    public:
        KMP_API void Push(const String& entry);
        KMP_NODISCARD KMP_API bool Pop();

        StringVector scope;
        String scopeString;
    };
    //--------------------------------------------------------------------------
}