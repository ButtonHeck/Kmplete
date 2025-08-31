#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Log/log_class_macro.h"

namespace Kmplete
{
    struct JsonScope
    {
        KMP_LOG_CLASSNAME(JsonScope)

    public:
        KMP_API void Push(const String& entry);
        KMP_NODISCARD KMP_API bool Pop();

        StringVector scope;
        String scopeString;
    };
    //--------------------------------------------------------------------------
}