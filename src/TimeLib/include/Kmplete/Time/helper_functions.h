#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"


namespace Kmplete
{
    namespace Time
    {
        KMP_NODISCARD KMP_API String GetCurrentTimeString(const char* format = "%F %T");
    }
}