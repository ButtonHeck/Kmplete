#pragma once

#if defined (KMP_ENGINE_PRIVATE)
    #error "entry_point.h should be included only in client code"
#endif

#include "Kmplete/Application/application.h"
#include "Kmplete/Core/platform.h"
#include "Kmplete/Core/pointers.h"
#include "Kmplete/Core/memory_checker.h"

int Main(const Kmplete::ProgramOptions& programOptions);

#if defined (KMP_PLATFORM_WINDOWS) && defined (KMP_WINMAIN) 
#include <Windows.h>
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR lpCmdLine, int)
{
    Kmplete::ProgramOptions programOptions;
    programOptions.ProcessCommandLine(lpCmdLine);

    return Main(programOptions);
}
//--------------------------------------------------------------------------

#else
int main(int argc, char** argv)
{
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
