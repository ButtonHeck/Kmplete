#pragma once

#include "Kmplete/Core/platform.h"
#include "Kmplete/Core/kmplete_api.h"

#if defined (KMP_PLATFORM_WINDOWS) && defined (KMP_COMPILER_MSVC)
    #include <crtdbg.h>
#endif

namespace Kmplete
{
    class MemoryChecker
    {
    public:
        KMP_API static void DumpAllMemoryLeaks();

    public:
        KMP_API MemoryChecker();
        KMP_API ~MemoryChecker();

        KMP_API void Mark();
        KMP_API void Check() const;

    private:
#if defined (KMP_PLATFORM_WINDOWS) && defined (KMP_COMPILER_MSVC)
        _CrtMemState _checkpoint;
#endif
    };
    //--------------------------------------------------------------------------
}
