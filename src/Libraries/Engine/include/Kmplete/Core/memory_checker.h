#pragma once

#include "Kmplete/Base/kmplete_api.h"


namespace Kmplete
{
    namespace MemoryChecker
    {
        //! Helper function to initialize memory leaks detection mechanism.
        //! Currently implemented only for Windows
        KMP_API void Prepare();
    };
    //--------------------------------------------------------------------------
}
