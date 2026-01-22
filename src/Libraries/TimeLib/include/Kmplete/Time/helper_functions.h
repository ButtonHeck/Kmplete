#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"


namespace Kmplete
{
    //! Utility functions related with the time
    namespace Time
    {
        KMP_NODISCARD KMP_API String GetCurrentTimeString(const char* format = "%F %T");
    }
}