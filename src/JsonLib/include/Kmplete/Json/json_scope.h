#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"

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