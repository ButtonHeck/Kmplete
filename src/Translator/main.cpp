#include "translator.h"
#include "Kmplete/Log/log.h"
#include "Kmplete/Filesystem/filesystem.h"

#include <boost/program_options.hpp>

#include <iostream>

namespace bpo = boost::program_options;

void PrintUsage(const bpo::options_description& description);
bool WorkModeIsValid(const Kmplete::String& workMode);
bpo::options_description CreateDescription();
int ParseParameters(const bpo::options_description& optionsDescription, bpo::variables_map& vm, Kmplete::Translator::TranslatorParameters& translatorParameters);
//--------------------------------------------------------------------------

int main(int argc, char** argv)
{
    using namespace Kmplete::Translator;

    auto optionsDescription = CreateDescription();

    if (argc < 2)
    {
        std::cerr << "Translator: invalid argument count\n";
        PrintUsage(optionsDescription);
        return ReturnCode::InvalidArgumentCount;
    }

    auto cmdParser = bpo::command_line_parser(argc, argv);
    bpo::variables_map vm;
    bpo::store(cmdParser.options(optionsDescription).run(), vm);
    bpo::notify(vm);

    TranslatorParameters translatorParameters;
    const auto parseParametersResult = ParseParameters(optionsDescription, vm, translatorParameters);
    if (parseParametersResult != 0)
    {
        std::cerr << "Translator: failed to parse parameters\n";
        return parseParametersResult;
    }

    TranslatorProcessor processor(std::move(translatorParameters));
    const auto translatorResultCode = processor.Run();
    return translatorResultCode;
}
//--------------------------------------------------------------------------

bpo::options_description CreateDescription()
{
    using namespace Kmplete;
    using namespace Kmplete::Translator;

    const auto loggingArgument =            Utils::Concatenate(ProcessorArgumentLogging, ",", ProcessorArgumentLoggingShort);
    const auto modeArgument =               Utils::Concatenate(ProcessorArgumentWorkMode, ",", ProcessorArgumentWorkModeShort);
    const auto inputDirectoriesArgument =   Utils::Concatenate(ProcessorArgumentInputDirectories, ",", ProcessorArgumentInputDirectoriesShort);
    const auto extensionsArgument =         Utils::Concatenate(ProcessorArgumentExtensions, ",", ProcessorArgumentExtensionsShort);
    const auto recursiveArgument =          Utils::Concatenate(ProcessorArgumentRecursive, ",", ProcessorArgumentRecursiveShort);
    const auto outputDirectoryArgument =    Utils::Concatenate(ProcessorArgumentOutputDirectory, ",", ProcessorArgumentOutputDirectoryShort);
    const auto outputFileNameArgument =     Utils::Concatenate(ProcessorArgumentOutputFileName, ",", ProcessorArgumentOutputFileNameShort);

    bpo::options_description optionsDescription("Translator options");
    optionsDescription.add_options()
        (loggingArgument.c_str(),                                                       "Is logging enabled")
        (modeArgument.c_str(),              bpo::value<String>(),                       "Working mode (Update, Compile)")
        (inputDirectoriesArgument.c_str(),  bpo::value<StringVector>()->multitoken(),   "Directories to search files")
        (extensionsArgument.c_str(),        bpo::value<StringVector>()->multitoken(),   "Files extensions to parse")
        (recursiveArgument.c_str(),                                                     "Is recursive search in directory")
        (outputDirectoryArgument.c_str(),   bpo::value<String>(),                       "Output files directory")
        (outputFileNameArgument.c_str(),    bpo::value<String>(),                       "Output file name");

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
    using namespace Kmplete::Translator;

    return workMode == ProcessorWorkModeUpdate ||
           workMode == ProcessorWorkModeCompile;
}
//--------------------------------------------------------------------------

int ParseParameters(const bpo::options_description& optionsDescription, bpo::variables_map& vm, Kmplete::Translator::TranslatorParameters& translatorParameters)
{
    using namespace Kmplete;
    using namespace Kmplete::Translator;

#if !defined (KMP_CONFIG_TYPE_PRODUCTION)
    // logging parsing
    const bool loggingEnabled = vm.count(ProcessorArgumentLogging);
    translatorParameters.logging = loggingEnabled;
    if (loggingEnabled)
    {
        Log::LogSettings logSettings;
        logSettings.outputFile = false;
        logSettings.outputConsole = true;
        logSettings.outputStringBuffer = false;
        logSettings.level = 0;
        logSettings.levelFlush = 0;
        Log::Initialize(logSettings, "Kmplete Translator");
    }
#endif

    // Work mode parsing
    const auto workMode = vm.count(ProcessorArgumentWorkMode) ? vm[ProcessorArgumentWorkMode].as<String>() : String();
    if (workMode.empty())
    {
        KMP_LOG_ERROR("Translator: work mode is not set");
        PrintUsage(optionsDescription);
        return ReturnCode::WorkModeIsNotSet;
    }
    if (!WorkModeIsValid(workMode))
    {
        KMP_LOG_ERROR("Translator: work mode is not valid ({})", workMode);
        PrintUsage(optionsDescription);
        return ReturnCode::WorkModeIsNotValid;
    }

    translatorParameters.workMode = workMode;

    if (workMode == ProcessorWorkModeUpdate)
    {
        // Source directories parsing
        const auto sourceDirectoryStrings = vm.count(ProcessorArgumentInputDirectories) ? vm[ProcessorArgumentInputDirectories].as<StringVector>() : StringVector();
        if (sourceDirectoryStrings.empty())
        {
            KMP_LOG_ERROR("Translator: source directories are not set");
            PrintUsage(optionsDescription);
            return ReturnCode::SourceDirectoriesAreNotSet;
        }
        PathVector sourceDirectories;
        sourceDirectories.reserve(sourceDirectoryStrings.size());
        for (const auto& sourceDirectoryStr : sourceDirectoryStrings)
        {
            const auto sourceDirectory = Path(sourceDirectoryStr);
            if (!Filesystem::PathExists(sourceDirectory) || !Filesystem::IsDirectory(sourceDirectory))
            {
                KMP_LOG_ERROR("Translator: one of source directories does not exist or is not of a directory type ({})", sourceDirectory);
                PrintUsage(optionsDescription);
                return ReturnCode::SourceDirectoriesAreNotValid;
            }

            sourceDirectories.push_back(sourceDirectory);
        }

        translatorParameters.sourceDirectories = sourceDirectories;

        // files extensions parsing
        const auto filesExtensions = vm.count(ProcessorArgumentExtensions) ? vm[ProcessorArgumentExtensions].as<StringVector>() : StringVector();
        if (filesExtensions.empty())
        {
            KMP_LOG_ERROR("Translator: files extensions are not set");
            PrintUsage(optionsDescription);
            return ReturnCode::FilesExtensionsAreNotSet;
        }

        translatorParameters.filesExtensions = filesExtensions;

        // Is recursive parsing
        const bool isRecursive = vm.count(ProcessorArgumentRecursive);
        translatorParameters.isRecursive = isRecursive;
    }

    // output directory parsing
    const auto outputDirectoryStr = vm.count(ProcessorArgumentOutputDirectory) ? vm[ProcessorArgumentOutputDirectory].as<String>() : String();
    if (outputDirectoryStr.empty())
    {
        KMP_LOG_ERROR("Translator: output directory is not set");
        PrintUsage(optionsDescription);
        return ReturnCode::OutputDirectoryIsNotSet;
    }
    const auto outputDirectory = Path(outputDirectoryStr);

    if (!Filesystem::CreateDirectories(outputDirectory))
    {
        KMP_LOG_ERROR("Translator: cannot create output directory ({})", outputDirectory);
        PrintUsage(optionsDescription);
        return ReturnCode::OutputDirectoryIsNotValid;
    }

    // output filename parsing
    const auto outputFileNameStr = vm.count(ProcessorArgumentOutputFileName) ? vm[ProcessorArgumentOutputFileName].as<String>() : String();
    if (outputFileNameStr.empty())
    {
        KMP_LOG_ERROR("Translator: output file name is not set");
        PrintUsage(optionsDescription);
        return ReturnCode::OutputFileNameIsNotSet;
    }

    translatorParameters.outputDirectory = outputDirectory;
    translatorParameters.outputFileName = outputFileNameStr;

    return ReturnCode::Ok;
}
//--------------------------------------------------------------------------