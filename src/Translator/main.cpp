#include "translator.h"

#include <boost/program_options.hpp>

#include <iostream>

namespace bpo = boost::program_options;

void PrintUsage(const bpo::options_description& description);
bool WorkModeIsValid(const Kmplete::String& workMode);
bpo::options_description CreateDescription();
int ParseParameters(const bpo::options_description& optionsDescription, bpo::variables_map& vm, Kmplete::TranslatorParameters& translatorParameters);
//--------------------------------------------------------------------------

int main(int argc, char** argv)
{
    using namespace Kmplete;

    auto optionsDescription = CreateDescription();

    if (argc < 2)
    {
        std::cerr << "Translator: invalid argument count\n";
        PrintUsage(optionsDescription);
        return -1;
    }

    auto cmdParser = bpo::command_line_parser(argc, argv);
    bpo::variables_map vm;
    bpo::store(cmdParser.options(optionsDescription).run(), vm);
    bpo::notify(vm);

    TranslatorParameters translatorParameters;
    const auto parseParametersResult = ParseParameters(optionsDescription, vm, translatorParameters);
    if (parseParametersResult != 0)
    {
        std::cerr << "Failed to parse parameters\n";
        return parseParametersResult;
    }

    Translator translator(std::move(translatorParameters));
    const auto translatorResultCode = translator.Run();
    return translatorResultCode;
}
//--------------------------------------------------------------------------

bpo::options_description CreateDescription()
{
    using namespace Kmplete;

    bpo::options_description optionsDescription("Translator options");
    optionsDescription.add_options()
        ("mode,M", bpo::value<String>(), "Working mode (Update, Compile)")
        ("input_directories,D", bpo::value<StringVector>()->multitoken(), "Directories to search files")
        ("extensions,E", bpo::value<StringVector>()->multitoken(), "Files extensions to parse")
        ("recursive,R", "Is recursive search in directory")
        ("output_directory,O", bpo::value<String>(), "Output files directory")
        ("output_file_name,F", bpo::value<String>(), "Output file name");

    return optionsDescription;
}
//--------------------------------------------------------------------------

void PrintUsage(const bpo::options_description& description)
{
    std::cout << description << std::endl;
}
//--------------------------------------------------------------------------

bool WorkModeIsValid(const Kmplete::String& workMode)
{
    return workMode == Kmplete::WorkingModeUpdate ||
           workMode == Kmplete::WorkingModeCompile;
}
//--------------------------------------------------------------------------

int ParseParameters(const bpo::options_description& optionsDescription, bpo::variables_map& vm, Kmplete::TranslatorParameters& translatorParameters)
{
    using namespace Kmplete;

    // Work mode parsing
    const auto workMode = vm.count("mode") ? vm["mode"].as<String>() : String();
    if (workMode.empty())
    {
        std::cerr << "Translator's work mode is not set\n";
        PrintUsage(optionsDescription);
        return -2;
    }
    if (!WorkModeIsValid(workMode))
    {
        std::cerr << "Translator's work mode is not valid\n";
        PrintUsage(optionsDescription);
        return -21;
    }

    translatorParameters.workMode = workMode;

    if (workMode == WorkingModeUpdate)
    {
        // Source directory parsing
        const auto sourceDirectoryStrings = vm.count("input_directories") ? vm["input_directories"].as<StringVector>() : StringVector();
        if (sourceDirectoryStrings.empty())
        {
            std::cerr << "Translator directories are not set\n";
            PrintUsage(optionsDescription);
            return -3;
        }
        std::vector<std::filesystem::path> sourceDirectories;
        sourceDirectories.reserve(sourceDirectoryStrings.size());
        for (const auto& sourceDirectoryStr : sourceDirectoryStrings)
        {
            const auto sourceDirectory = std::filesystem::path(sourceDirectoryStr);
            if (!std::filesystem::exists(sourceDirectory) || !std::filesystem::is_directory(sourceDirectory))
            {
                std::cerr << "One of translator directories is not exists or is not of a directory type\n";
                PrintUsage(optionsDescription);
                return -31;
            }

            sourceDirectories.push_back(sourceDirectory);
        }

        translatorParameters.sourceDirectories = sourceDirectories;

        // files extensions parsing
        const auto filesExtensions = vm.count("extensions") ? vm["extensions"].as<StringVector>() : StringVector();
        if (filesExtensions.empty())
        {
            std::cerr << "Files extensions are not set\n";
            PrintUsage(optionsDescription);
            return -4;
        }

        translatorParameters.filesExtensions = filesExtensions;

        // Is recursive parsing
        const bool isRecursive = vm.count("recursive");
        translatorParameters.isRecursive = isRecursive;
    }

    // output directory parsing
    const auto outputDirectoryStr = vm.count("output_directory") ? vm["output_directory"].as<String>() : String();
    if (outputDirectoryStr.empty())
    {
        std::cerr << "Output directory is not set\n";
        PrintUsage(optionsDescription);
        return -5;
    }
    const auto outputDirectory = std::filesystem::path(outputDirectoryStr);
    if (!std::filesystem::exists(outputDirectory))
    {
        const auto success = std::filesystem::create_directories(outputDirectory);
        if (!success)
        {
            std::cerr << "Translator cannot create output directory\n";
            PrintUsage(optionsDescription);
            return -51;
        }
    }

    // output filename parsing
    const auto outputFileNameStr = vm.count("output_file_name") ? vm["output_file_name"].as<String>() : String();
    if (outputFileNameStr.empty())
    {
        std::cerr << "Output file name is not set\n";
        PrintUsage(optionsDescription);
        return -6;
    }

    translatorParameters.outputDirectory = outputDirectory;
    translatorParameters.outputFileName = outputFileNameStr;

    return 0;
}
//--------------------------------------------------------------------------