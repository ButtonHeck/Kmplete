#include "Kmplete/Core/program_options.h"
#include "Kmplete/Core/types.h"

namespace Kmplete
{
    ProgramOptions::ProgramOptions() noexcept
        : _settingsFilePath(Path())
    {}
    //--------------------------------------------------------------------------

#if defined (KMP_PLATFORM_WINDOWS)
    void ProgramOptions::ProcessCommandLine(char* lpCmdLine)
    {
        auto cmdParser = CreateCmdParser(lpCmdLine);
        ProcessCommandLineArgs(cmdParser);
    }
    //--------------------------------------------------------------------------

    boost::program_options::command_line_parser ProgramOptions::CreateCmdParser(char* lpCmdLine) const
    {
        const auto args = boost::program_options::split_winmain(lpCmdLine);
        return boost::program_options::command_line_parser(args);
    }
    //--------------------------------------------------------------------------
#endif

    void ProgramOptions::ProcessCommandLine(int argc, char** argv)
    {
        auto cmdParser = CreateCmdParser(argc, argv);
        ProcessCommandLineArgs(cmdParser);
    }
    //--------------------------------------------------------------------------

    boost::program_options::command_line_parser ProgramOptions::CreateCmdParser(int argc, char** argv) const
    {
        return boost::program_options::command_line_parser(argc, argv);
    }
    //--------------------------------------------------------------------------

    const Path& ProgramOptions::GetSettingsFilePath() const
    {
        return _settingsFilePath;
    }
    //--------------------------------------------------------------------------

    void ProgramOptions::ProcessCommandLineArgs(boost::program_options::command_line_parser& cmdParser)
    {
        boost::program_options::options_description optDescription("Kmplete options");
        optDescription.add_options()
            ("settings,S", boost::program_options::value<String>(), "Path to settings file")
            ;

        boost::program_options::variables_map vm;
        boost::program_options::store(cmdParser.options(optDescription).run(), vm);
        boost::program_options::notify(vm);

        _settingsFilePath = Path(vm.count("settings") ? vm["settings"].as<String>() : "");
    }
    //--------------------------------------------------------------------------
}