#include "Kmplete/Core/memory_checker.h"

#if defined (KMP_PLATFORM_WINDOWS) && defined (KMP_COMPILER_MSVC)
    #include <crtdbg.h>
    #define _CRTDBG_MAP_ALLOC
#endif

namespace Kmplete
{
    void MemoryChecker::Prepare()
    {
#if defined (KMP_PLATFORM_WINDOWS) && defined (KMP_COMPILER_MSVC)
        _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
        _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE | _CRTDBG_MODE_DEBUG);
        _CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDERR);
#endif
    }
    //--------------------------------------------------------------------------
}