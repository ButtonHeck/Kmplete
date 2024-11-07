#pragma once

#include "Kmplete/Application/application.h"
#include "Kmplete/Core/platform.h"
#include "Kmplete/Core/program_options.h"
#include "Kmplete/Core/pointers.h"

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
    auto app = Kmplete::CreateApplication(programOptions.GetSettingsFilePath());
    if (!app)
    {
        return 1;
    }

    app->Run();
    app.reset();

    return 0;
}
//--------------------------------------------------------------------------
