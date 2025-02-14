#include "Kmplete/Core/memory_checker.h"

#if defined (KMP_PLATFORM_WINDOWS) && defined (KMP_COMPILER_MSVC)
    #define _CRTDBG_MAP_ALLOC
#endif

namespace Kmplete
{
    void MemoryChecker::DumpAllMemoryLeaks()
    {
#if defined (KMP_PLATFORM_WINDOWS) && defined (KMP_COMPILER_MSVC)
        _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE | _CRTDBG_MODE_DEBUG);
        _CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDERR);
        _CrtDumpMemoryLeaks();
#endif
    }
    //--------------------------------------------------------------------------

    MemoryChecker::MemoryChecker()
    {
#if defined (KMP_PLATFORM_WINDOWS) && defined (KMP_COMPILER_MSVC)
        _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE | _CRTDBG_MODE_DEBUG);
        _CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDERR);
        _CrtMemCheckpoint(&_checkpoint);
#endif
    }
    //--------------------------------------------------------------------------

    MemoryChecker::~MemoryChecker()
    {
        Check();
    }
    //--------------------------------------------------------------------------

    void MemoryChecker::Mark()
    {
#if defined (KMP_PLATFORM_WINDOWS) && defined (KMP_COMPILER_MSVC)
        _CrtMemCheckpoint(&_checkpoint);
#endif
    }
    //--------------------------------------------------------------------------

    void MemoryChecker::Check() const
    {
        _CrtMemDumpAllObjectsSince(&_checkpoint);
    }
    //--------------------------------------------------------------------------
}