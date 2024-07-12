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

        KMP_NODISCARD const std::string& GetSettingsFilePath() const;

    private:
        // WinMain entryPoint
        KMP_NODISCARD boost::program_options::command_line_parser CreateCmdParser(char* lpCmdLine) const;
        // main entryPoint
        KMP_NODISCARD boost::program_options::command_line_parser CreateCmdParser(int argc, char** argv) const;

        void ProcessCommandLineArgs(boost::program_options::command_line_parser& cmdParser);

    private:
        std::string _settingsFilePath;
    };
    //--------------------------------------------------------------------------
}