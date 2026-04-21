#pragma once

#if defined (KMP_ENGINE_PRIVATE)
    #error "main.h should be included only in client code"
#endif

#include "Kmplete/Application/application.h"
#include "Kmplete/Base/platform.h"
#include "Kmplete/Base/pointers.h"
#include "Kmplete/Base/exception.h"
#include "Kmplete/Core/memory_checker.h"
#include "Kmplete/Utils/function_utils.h"
#include "Kmplete/Profile/profiler.h"
#include "Kmplete/Log/log.h"


//! Wrapper function that tries to flush all the profiler data
//! during program termination and rethrows an exception (if any)
void TerminationHandler()
{
    try
    {
#if defined(KMP_PROFILE)
        Kmplete::Profiler::Get().EndSession();
#endif

        const auto exception = std::current_exception();
        if (exception)
        {
            std::rethrow_exception(exception);
        }
    }
    catch (KMP_MB_UNUSED const Kmplete::Exception& e)
    {
        KMP_LOG_CRITICAL_FN("Uncaught exception: '{}'", e.what());
    }
    catch (...)
    {
        KMP_LOG_CRITICAL_FN("Uncaught exception");
    }

    std::exit(2);
}
//--------------------------------------------------------------------------


int Main(const Kmplete::ProgramOptions& programOptions);

#if defined (KMP_PLATFORM_WINDOWS) && defined (KMP_WINMAIN) 
#include <Windows.h>
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR lpCmdLine, int)
{
    std::set_terminate(TerminationHandler);

    Kmplete::ProgramOptions programOptions;
    programOptions.ProcessCommandLine(lpCmdLine);

    return Main(programOptions);
}
//--------------------------------------------------------------------------

#else
int main(int argc, char** argv)
{
    std::set_terminate(TerminationHandler);

    Kmplete::ProgramOptions programOptions;
    programOptions.ProcessCommandLine(argc, argv);

    return Main(programOptions);
}
#endif
//--------------------------------------------------------------------------

namespace Kmplete
{
    const char* ApplicationProfileSessionPrefix();
}

int Main(const Kmplete::ProgramOptions& programOptions)
{
    Kmplete::MemoryChecker::Prepare();

#if defined(KMP_PROFILE)
    Kmplete::Profiler::Get().SetActive(!programOptions.IsProfilingOnDemand());
    Kmplete::Profiler::Get().SetLevel(programOptions.GetProfilingLevel());
#endif

    KMP_MB_UNUSED const auto startupSessionCapacity = 400;
    KMP_PROFILE_BEGIN_SESSION("Startup", Kmplete::Utils::Concatenate(Kmplete::ApplicationProfileSessionPrefix(), "-Profile-Startup.json"), startupSessionCapacity);
    auto app = Kmplete::CreateApplication(programOptions);
    KMP_PROFILE_END_SESSION();

    if (!app)
    {
        return 1;
    }

    KMP_MB_UNUSED const auto runtimeSessionCapacity = 10'000;
    KMP_PROFILE_BEGIN_SESSION("Runtime", Kmplete::Utils::Concatenate(Kmplete::ApplicationProfileSessionPrefix(), "-Profile-Runtime.json"), runtimeSessionCapacity);
    app->Run();
    KMP_PROFILE_END_SESSION();

    KMP_MB_UNUSED const auto shutdownSessionCapacity = 200;
    KMP_PROFILE_BEGIN_SESSION("Shutdown", Kmplete::Utils::Concatenate(Kmplete::ApplicationProfileSessionPrefix(), "-Profile-Shutdown.json"), shutdownSessionCapacity);
    app.reset();
    KMP_PROFILE_END_SESSION();

    return 0;
}
//--------------------------------------------------------------------------
