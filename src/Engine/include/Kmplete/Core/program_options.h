#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/platform.h"
#include "Kmplete/Core/filesystem.h"

#include <boost/program_options.hpp>

namespace Kmplete
{
    class ProgramOptions
    {
    public:
        KMP_API ProgramOptions() noexcept;

#if defined (KMP_PLATFORM_WINDOWS)
        // WinMain entryPoint
        KMP_API void ProcessCommandLine(char* lpCmdLine);
#endif

        // main entryPoint
        KMP_API void ProcessCommandLine(int argc, char** argv);

        KMP_NODISCARD KMP_API const Path& GetSettingsFilePath() const;

    private:
#if defined (KMP_PLATFORM_WINDOWS)
        // WinMain entryPoint
        KMP_NODISCARD boost::program_options::command_line_parser CreateCmdParser(char* lpCmdLine) const;
#endif

        // main entryPoint
        KMP_NODISCARD boost::program_options::command_line_parser CreateCmdParser(int argc, char** argv) const;

        void ProcessCommandLineArgs(boost::program_options::command_line_parser& cmdParser);

    private:
        Path _settingsFilePath;
    };
    //--------------------------------------------------------------------------
}