#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Log/log_class_macro.h"


namespace Kmplete
{
    //! Utility struct to keep track which node is currently used for
    //! reading or writing
    struct KMP_API JsonScope
    {
        KMP_LOG_CLASSNAME(JsonScope)

    public:
        void Push(const String& entry);
        KMP_NODISCARD bool Pop();

        StringVector scope;
        String scopeString;
    };
    //--------------------------------------------------------------------------
}