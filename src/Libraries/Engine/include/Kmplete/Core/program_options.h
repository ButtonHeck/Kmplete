#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/platform.h"
#include "Kmplete/Base/types_aliases.h"

#include <boost/program_options.hpp>


namespace Kmplete
{
    //! Wrapper for program's arguments using Boost.program_options as the parser backend, responsible
    //! for parsing incoming arguments both from main(...) and WinMain(...) entry points.
    //! @see main.h for example usage
    class KMP_API ProgramOptions
    {
    public:
        ProgramOptions() noexcept;
        ~ProgramOptions() = default;

#if defined (KMP_PLATFORM_WINDOWS)
        //! WinMain entryPoint
        void ProcessCommandLine(char* lpCmdLine);
#endif

        //! main entryPoint
        void ProcessCommandLine(int argc, char** argv);

        KMP_NODISCARD const Filepath& GetSettingsFilepath() const noexcept;
        KMP_NODISCARD int GetProfilingLevel() const noexcept;
        KMP_NODISCARD bool IsProfilingOnDemand() const noexcept;

    private:
#if defined (KMP_PLATFORM_WINDOWS)
        //! WinMain entryPoint
        KMP_NODISCARD boost::program_options::command_line_parser _CreateCmdParser(char* lpCmdLine) const;
#endif

        //! main entryPoint
        KMP_NODISCARD boost::program_options::command_line_parser _CreateCmdParser(int argc, char** argv) const;

        void _ProcessCommandLineArgs(boost::program_options::command_line_parser& cmdParser);

    private:
        Filepath _settingsFilepath;
        int _profilingLevel;
        bool _profilingOnDemand;
    };
    //--------------------------------------------------------------------------
}