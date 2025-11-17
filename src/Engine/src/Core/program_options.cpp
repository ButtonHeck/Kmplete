#include "Kmplete/Core/program_options.h"


namespace Kmplete
{
    ProgramOptions::ProgramOptions() noexcept
        : _settingsFilepath(Filepath())
        , _profilingLevel(0)
        , _profilingOnDemand(false)
    {}
    //--------------------------------------------------------------------------

#if defined (KMP_PLATFORM_WINDOWS)
    void ProgramOptions::ProcessCommandLine(char* lpCmdLine)
    {
        auto cmdParser = _CreateCmdParser(lpCmdLine);
        _ProcessCommandLineArgs(cmdParser);
    }
    //--------------------------------------------------------------------------

    boost::program_options::command_line_parser ProgramOptions::_CreateCmdParser(char* lpCmdLine) const
    {
        const auto args = boost::program_options::split_winmain(lpCmdLine);
        return boost::program_options::command_line_parser(args);
    }
    //--------------------------------------------------------------------------
#endif

    void ProgramOptions::ProcessCommandLine(int argc, char** argv)
    {
        auto cmdParser = _CreateCmdParser(argc, argv);
        _ProcessCommandLineArgs(cmdParser);
    }
    //--------------------------------------------------------------------------

    boost::program_options::command_line_parser ProgramOptions::_CreateCmdParser(int argc, char** argv) const
    {
        return boost::program_options::command_line_parser(argc, argv);
    }
    //--------------------------------------------------------------------------

    const Filepath& ProgramOptions::GetSettingsFilepath() const noexcept
    {
        return _settingsFilepath;
    }
    //--------------------------------------------------------------------------

    int ProgramOptions::GetProfilingLevel() const noexcept
    {
        return _profilingLevel;
    }
    //--------------------------------------------------------------------------

    bool ProgramOptions::IsProfilingOnDemand() const noexcept
    {
        return _profilingOnDemand;
    }
    //--------------------------------------------------------------------------

    void ProgramOptions::_ProcessCommandLineArgs(boost::program_options::command_line_parser& cmdParser)
    {
        boost::program_options::options_description optDescription("Kmplete options");
        optDescription.add_options()
            ("settings,S",          boost::program_options::value<String>(),    "Path to settings file")
            ("profile_level,P",     boost::program_options::value<int>(),       "Profiling level")
            ("profile_on_demand,D", boost::program_options::value<bool>(),      "Profiling on demand");

        boost::program_options::variables_map vm;
        boost::program_options::store(cmdParser.options(optDescription).run(), vm);
        boost::program_options::notify(vm);

        _settingsFilepath = Filepath(vm.count("settings") ? vm["settings"].as<String>() : "");
        _profilingLevel = vm.count("profile_level") ? vm["profile_level"].as<int>() : 0;
        _profilingOnDemand = vm.count("profile_on_demand") ? vm["profile_on_demand"].as<bool>() : false;
    }
    //--------------------------------------------------------------------------
}