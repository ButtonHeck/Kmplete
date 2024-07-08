#include "Kmplete/Core/program_options.h"

namespace Kmplete
{
    ProgramOptions::ProgramOptions()
        : _settingsFilePath("")
    {}
    //--------------------------------------------------------------------------

    void ProgramOptions::ProcessCommandLine(char* lpCmdLine)
    {
        auto cmdParser = CreateCmdParser(lpCmdLine);
        ProcessCommandLineArgs(cmdParser);
    }
    //--------------------------------------------------------------------------

    void ProgramOptions::ProcessCommandLine(int argc, char** argv)
    {
        auto cmdParser = CreateCmdParser(argc, argv);
        ProcessCommandLineArgs(cmdParser);
    }
    //--------------------------------------------------------------------------

    const std::string& ProgramOptions::GetSettingsFilePath() const
    {
        return _settingsFilePath;
    }
    //--------------------------------------------------------------------------

    boost::program_options::command_line_parser ProgramOptions::CreateCmdParser(char* lpCmdLine) const
    {
        const auto args = boost::program_options::split_winmain(lpCmdLine);
        return boost::program_options::command_line_parser(args);
    }
    //--------------------------------------------------------------------------

    boost::program_options::command_line_parser ProgramOptions::CreateCmdParser(int argc, char** argv) const
    {
        return boost::program_options::command_line_parser(argc, argv);
    }
    //--------------------------------------------------------------------------

    void ProgramOptions::ProcessCommandLineArgs(boost::program_options::command_line_parser& cmdParser)
    {
        boost::program_options::options_description optDescription("Kmplete options");
        optDescription.add_options()
            ("settngs,S", boost::program_options::value<std::string>(), "Path to settings file")
            ;

        boost::program_options::variables_map vm;
        boost::program_options::store(cmdParser.options(optDescription).run(), vm);
        boost::program_options::notify(vm);

        _settingsFilePath = vm.count("settings") ? vm["settings"].as<std::string>() : "";
    }
    //--------------------------------------------------------------------------
}