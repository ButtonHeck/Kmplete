#pragma once

#if defined (KMP_ENGINE_PRIVATE)
    #error "entry_point.h should be included only in client code"
#endif

#include "Kmplete/Application/application.h"
#include "Kmplete/Base/platform.h"
#include "Kmplete/Base/pointers.h"
#include "Kmplete/Core/memory_checker.h"

#include <exception>

void TerminationHandler()
{
    try
    {
        const auto exception = std::current_exception();
        if (exception)
        {
            std::rethrow_exception(exception);
        }
    }
    catch (KMP_MB_UNUSED const std::exception& e)
    {
        KMP_LOG_CRITICAL("Uncaught exception: '{}'", e.what());
    }
    catch (...)
    {
        KMP_LOG_CRITICAL("Uncaught exception");
    }

    abort();
}
//--------------------------------------------------------------------------

int Main(const Kmplete::ProgramOptions& programOptions);

#if defined (KMP_PLATFORM_WINDOWS) && defined (KMP_WINMAIN) 
#include <Windows.h>
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR lpCmdLine, int)
{
    set_terminate(TerminationHandler);

    Kmplete::ProgramOptions programOptions;
    programOptions.ProcessCommandLine(lpCmdLine);

    return Main(programOptions);
}
//--------------------------------------------------------------------------

#else
int main(int argc, char** argv)
{
    set_terminate(TerminationHandler);

    Kmplete::ProgramOptions programOptions;
    programOptions.ProcessCommandLine(argc, argv);

    return Main(programOptions);
}
#endif
//--------------------------------------------------------------------------

int Main(const Kmplete::ProgramOptions& programOptions)
{
    Kmplete::MemoryChecker::Prepare();
    auto app = Kmplete::CreateApplication(programOptions);
    if (!app)
    {
        return 1;
    }

    app->Run();
    app.reset();

    return 0;
}
//--------------------------------------------------------------------------
