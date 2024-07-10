#pragma once

#include "Kmplete/Core/kmplete_api.h"

#include <boost/program_options.hpp>

namespace Kmplete
{
    class KMP_API ProgramOptions
    {
    public:
        ProgramOptions();

        // WinMain entryPoint
        void ProcessCommandLine(char* lpCmdLine);
        // main entryPoint
        void ProcessCommandLine(int argc, char** argv);

        const std::string& GetSettingsFilePath() const;

    private:
        // WinMain entryPoint
        boost::program_options::command_line_parser CreateCmdParser(char* lpCmdLine) const;
        // main entryPoint
        boost::program_options::command_line_parser CreateCmdParser(int argc, char** argv) const;

        void ProcessCommandLineArgs(boost::program_options::command_line_parser& cmdParser);

    private:
        std::string _settingsFilePath;
    };
    //--------------------------------------------------------------------------
}