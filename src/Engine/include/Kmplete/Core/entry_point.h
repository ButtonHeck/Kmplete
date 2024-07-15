#pragma once

#include "Kmplete/Core/application.h"
#include "Kmplete/Core/platform.h"
#include "Kmplete/Core/program_options.h"
#include "Kmplete/Core/pointers.h"

int Main(const Kmplete::ProgramOptions& programOptions);

#if defined KMP_WINMAIN
#include <Windows.h>
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
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
    auto app = Kmplete::CreateApplication();
    if (!app)
    {
        return 1;
    }

    if (!app->Initialize(programOptions.GetSettingsFilePath()))
    {
        app.reset();
        return 2;
    }

    app->Run();

    if (!app->Finalize())
    {
        app.reset();
        return 3;
    }    

    app.reset();

    return 0;
}
//--------------------------------------------------------------------------
