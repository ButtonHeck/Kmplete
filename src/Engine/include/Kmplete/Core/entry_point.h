#pragma once

#include "Kmplete/Core/application.h"
#include "Kmplete/Core/platform.h"
#include "Kmplete/Core/program_options.h"

int Main(const Kmplete::ProgramOptions& programOptions);

#if defined KMPLETE_WINMAIN
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
    Kmplete::Application* app = Kmplete::CreateApplication();
    if (!app)
    {
        return 1;
    }

    if (!app->Initialize(programOptions.GetSettingsFilePath()))
    {
        delete app;
        return 2;
    }

    app->Run();

    delete app;

    return 0;
}
//--------------------------------------------------------------------------
