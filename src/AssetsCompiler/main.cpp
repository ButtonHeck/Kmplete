#include "assets_compiler.h"

#include "Kmplete/Log/log.h"
#include "Kmplete/Filesystem/filesystem.h"
#include "Kmplete/Utils/string_utils.h"

#include <boost/program_options.hpp>

#include <iostream>


namespace bpo = boost::program_options;

namespace Kmplete
{
    namespace Assets
    {
        void PrintUsage(const bpo::options_description& description);
        bpo::options_description CreateOptionsDescription();
        int ParseParameters(const bpo::options_description& optionsDescription, bpo::variables_map& vm, Kmplete::Assets::Compiler::CompilerParameters& compilerParameters);
    }
}
//--------------------------------------------------------------------------


int main(int argc, char** argv)
{
    using namespace Kmplete::Assets;

    auto optionsDescription = CreateOptionsDescription();

    if (argc < 2)
    {
        std::cerr << "AssetsCompiler: invalid argument count\n";
        PrintUsage(optionsDescription);
        return Compiler::ReturnCode::InvalidArgumentCount;
    }

    auto cmdParser = bpo::command_line_parser(argc, argv);
    bpo::variables_map vm;
    bpo::store(cmdParser.options(optionsDescription).run(), vm);
    bpo::notify(vm);

    Compiler::CompilerParameters compilerParameters;
    const auto parseParametersResult = ParseParameters(optionsDescription, vm, compilerParameters);
    if (parseParametersResult != 0)
    {
        std::cerr << "AssetsCompiler: failed to parse parameters\n";

#if !defined (KMP_CONFIG_TYPE_PRODUCTION)
        if (vm.count(Compiler::CompilerArgumentLogging))
        {
            Kmplete::Log::Finalize();
        }
#endif

        return parseParametersResult;
    }

    Compiler::AssetsCompiler compiler(std::move(compilerParameters));
    const auto compilerReturnCode = compiler.Run();

#if !defined (KMP_CONFIG_TYPE_PRODUCTION)
    if (vm.count(Compiler::CompilerArgumentLogging))
    {
        Kmplete::Log::Finalize();
    }
#endif

    return compilerReturnCode;
}
//--------------------------------------------------------------------------

namespace Kmplete
{
    namespace Assets
    {
        bpo::options_description CreateOptionsDescription()
        {
            using namespace Compiler;

            const auto loggingArgument =            Utils::Concatenate(CompilerArgumentLogging, ",", CompilerArgumentLoggingShort);
            const auto inputFileNameArgument =      Utils::Concatenate(CompilerArgumentInputFileName, ",", CompilerArgumentInputFileNameShort);
            const auto outputFileNameArgument =     Utils::Concatenate(CompilerArgumentOutputFileName, ",", CompilerArgumentOutputFileNameShort);

            bpo::options_description optionsDescription("Assets compiler options");
            optionsDescription.add_options()
                (loggingArgument.c_str(),                                   "Is logging enabled")
                (inputFileNameArgument.c_str(),     bpo::value<String>(),   "Input file name to read data from")
                (outputFileNameArgument.c_str(),    bpo::value<String>(),   "Output file name to write data to");

            return optionsDescription;
        }
        //--------------------------------------------------------------------------

        void PrintUsage(const bpo::options_description& description)
        {
            std::cout << description << std::endl;
        }
        //--------------------------------------------------------------------------

        int ParseParameters(const bpo::options_description& optionsDescription, bpo::variables_map& vm, Kmplete::Assets::Compiler::CompilerParameters& compilerParameters)
        {
            using namespace Compiler;

#if !defined (KMP_CONFIG_TYPE_PRODUCTION)
            // logging parsing
            const bool loggingEnabled = vm.count(CompilerArgumentLogging);
            compilerParameters.logging = loggingEnabled;
            if (loggingEnabled)
            {
                Log::LogSettings logSettings;
                logSettings.outputFile = false;
                logSettings.outputConsole = true;
                logSettings.outputStringBuffer = false;
                logSettings.level = 0;
                logSettings.levelFlush = 0;
                Log::SetSettings(logSettings);
                Log::Initialize("Kmplete Assets compiler");
            }
#endif

            // input filename parsing
            const auto inputFileNameStr = vm.count(CompilerArgumentInputFileName) ? vm[CompilerArgumentInputFileName].as<String>() : String();
            if (inputFileNameStr.empty())
            {
                KMP_LOG_ERROR_FN("AssetsCompiler: input file name is not set");
                PrintUsage(optionsDescription);
                return ReturnCode::InputFileIsNotSet;
            }

            const auto inputFileNamePath = Filepath(inputFileNameStr);
            if (!Filesystem::FilepathExists(inputFileNamePath) || !Filesystem::IsFile(inputFileNamePath))
            {
                KMP_LOG_ERROR_FN("AssetsCompiler: input file '{}' does not exist or is not of a file type", inputFileNamePath);
                PrintUsage(optionsDescription);
                return ReturnCode::InputFileIsNotValid;
            }

            compilerParameters.sourceFile = inputFileNamePath;

            // output filename parsing
            const auto outputFileNameStr = vm.count(CompilerArgumentOutputFileName) ? vm[CompilerArgumentOutputFileName].as<String>() : String();
            if (outputFileNameStr.empty())
            {
                KMP_LOG_ERROR_FN("AssetsCompiler: output file name is not set");
                PrintUsage(optionsDescription);
                return ReturnCode::OutputFileIsNotSet;
            }

            const auto outputFileNamePath = Filepath(outputFileNameStr);
            if (!Filesystem::CreateFile(outputFileNamePath))
            {
                KMP_LOG_ERROR_FN("AssetsCompiler: cannot create output file '{}'", outputFileNamePath);
                PrintUsage(optionsDescription);
                return ReturnCode::OutputFileCreationFailed;
            }

            compilerParameters.outputFile = outputFileNamePath;

            return ReturnCode::Ok;
        }
        //--------------------------------------------------------------------------
    }
}