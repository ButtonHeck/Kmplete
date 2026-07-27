#include "Kmplete/Core/exception_handler.h"
#include "Kmplete/Core/stacktrace.h"
#include "Kmplete/Profile/profiler.h"
#include "Kmplete/Log/log.h"


#if defined (KMP_PLATFORM_WINDOWS)
    LONG WINAPI UnhandledExceptionHandler(PEXCEPTION_POINTERS)
    {
    #if defined(KMP_PROFILE)
        Kmplete::Profiler::Get().EndSession();
    #endif

        KMP_LOG_ERROR_FN("UnhandledExceptionHandler: uncaught exception has occured");
        Kmplete::DumpStacktrace();

        return EXCEPTION_CONTINUE_SEARCH;
    }
    //--------------------------------------------------------------------------

#elif defined (KMP_PLATFORM_LINUX)
    void SignalHandler(int signum, siginfo_t*, void*)
    {
    #if defined(KMP_PROFILE)
        Kmplete::Profiler::Get().EndSession();
    #endif

        KMP_LOG_ERROR_FN("SignalHandler: uncaught exception has occured '{}'", sigdescr_np(signum));
        Kmplete::DumpStacktrace();

        exit(signum);
    }
    //--------------------------------------------------------------------------
#endif

void SetExceptionHandler()
{
#if defined (KMP_PLATFORM_WINDOWS)
    SetUnhandledExceptionFilter(UnhandledExceptionHandler);
#elif defined (KMP_PLATFORM_LINUX)
    struct sigaction sa;
    sa.sa_sigaction = SignalHandler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_SIGINFO;

    sigaction(SIGSEGV, &sa, NULL);
    sigaction(SIGABRT, &sa, NULL);
#endif
}
//--------------------------------------------------------------------------