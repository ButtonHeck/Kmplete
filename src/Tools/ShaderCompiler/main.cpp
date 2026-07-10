#include "shader_compiler_tool.h"

#include "Kmplete/Log/log.h"
#include "Kmplete/Filesystem/filesystem.h"
#include "Kmplete/Utils/string_utils.h"
#include "Kmplete/ShaderCompiler/compile.h"

#include <boost/program_options.hpp>

#include <iostream>


namespace bpo = boost::program_options;

namespace Kmplete
{
    namespace ShaderCompiler
    {
        void PrintUsage(const bpo::options_description& description);
        bool WorkModeIsValid(const Kmplete::String& workMode);
        bpo::options_description CreateOptionsDescription();
        int ParseParameters(const bpo::options_description& optionsDescription, bpo::variables_map& vm, ShaderCompilerTool::CompilerParameters& compilerParameters);
    }
}
//--------------------------------------------------------------------------


//! The entry point of ShaderCompilerTool program:
//! Stage 1 - fetch and process command line arguments
//! Stage 2 - parse those arguments to compiler-specific parameters
//! Stage 3 - create and run the compiler with parameters from stage 2
int main(int argc, char** argv)
{
    using namespace Kmplete::ShaderCompiler;

    // Stage 1

    auto optionsDescription = CreateOptionsDescription();

    if (argc < 2)
    {
        std::cerr << "ShaderCompilerTool: invalid argument count\n";
        PrintUsage(optionsDescription);
        return ShaderCompilerTool::InvalidArgumentCount;
    }

    auto cmdParser = bpo::command_line_parser(argc, argv);
    bpo::variables_map vm;
    bpo::store(cmdParser.options(optionsDescription).run(), vm);
    bpo::notify(vm);


    // Stage 2

    ShaderCompilerTool::CompilerParameters compilerParameters;
    const auto parseParametersResult = ParseParameters(optionsDescription, vm, compilerParameters);
    if (parseParametersResult != ShaderCompilerTool::ReturnCode::Ok)
    {
        std::cerr << "ShaderCompilerTool: failed to parse parameters\n";

#if !defined (KMP_CONFIG_TYPE_PRODUCTION)
        if (vm.count(ShaderCompilerTool::CompilerArgumentLogging))
        {
            Kmplete::Log::Finalize();
        }
#endif

        return parseParametersResult;
    }


    // Stage 3

    ShaderCompilerTool::ShaderCompilerProcessor processor(std::move(compilerParameters));
    const auto processorResultCode = processor.Run();

#if !defined (KMP_CONFIG_TYPE_PRODUCTION)
    if (vm.count(ShaderCompilerTool::CompilerArgumentLogging))
    {
        Kmplete::Log::Finalize();
    }
#endif

    return processorResultCode;
}
//--------------------------------------------------------------------------


namespace Kmplete
{
    namespace ShaderCompiler
    {
        bpo::options_description CreateOptionsDescription()
        {
            using namespace ShaderCompilerTool;

            const auto loggingArgument =            Utils::Concatenate(CompilerArgumentLogging, ",", CompilerArgumentLoggingShort);
            const auto modeArgument =               Utils::Concatenate(CompilerArgumentWorkMode, ",", CompilerArgumentWorkModeShort);
            const auto sourceFilesArgument =        Utils::Concatenate(CompilerArgumentSourceFiles, ",", CompilerArgumentSourceFilesShort);
            const auto destinationFilesArgument =   Utils::Concatenate(CompilerArgumentDestinationFiles, ",", CompilerArgumentDestinationFilesShort);
            const auto shaderTypesArgument =        Utils::Concatenate(CompilerArgumentShaderTypes, ",", CompilerArgumentShaderTypesShort);

            bpo::options_description optionsDescription("ShaderCompiler options");
            optionsDescription.add_options()
                (loggingArgument.c_str(),                                                       "Is logging enabled")
                (modeArgument.c_str(),              bpo::value<String>(),                       "Working mode (Direct, Json)")
                (sourceFilesArgument.c_str(),       bpo::value<StringVector>()->multitoken(),   "Shader source file(s)")
                (destinationFilesArgument.c_str(),  bpo::value<StringVector>()->multitoken(),   "Shader binary destination file(s)")
                (shaderTypesArgument.c_str(),       bpo::value<StringVector>()->multitoken(),   "Shader type(s)");

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
            return workMode == ShaderCompilerTool::CompilerWorkModeDirect ||
                   workMode == ShaderCompilerTool::CompilerWorkModeJson;
        }
        //--------------------------------------------------------------------------

        int ParseParameters(const bpo::options_description& optionsDescription, bpo::variables_map& vm, ShaderCompilerTool::CompilerParameters& compilerParameters)
        {
            using namespace ShaderCompilerTool;

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
                Log::Initialize("Kmplete Shader compiler", Filesystem::GetCurrentFilepath() / "Logs");
            }
#endif

            // Work mode parsing
            const auto workMode = vm.count(CompilerArgumentWorkMode) ? vm[CompilerArgumentWorkMode].as<String>() : String();
            if (workMode.empty())
            {
                KMP_LOG_ERROR_FN("ShaderCompilerTool: work mode is not set");
                PrintUsage(optionsDescription);
                return ReturnCode::WorkModeIsNotSet;
            }
            if (!WorkModeIsValid(workMode))
            {
                KMP_LOG_ERROR_FN("ShaderCompilerTool: work mode '{}' is not valid", workMode);
                PrintUsage(optionsDescription);
                return ReturnCode::WorkModeIsNotValid;
            }

            compilerParameters.workMode = workMode;

            if (workMode == CompilerWorkModeJson)
            {
                // Source json files parsing
                const auto sourceJsonFileStrings = vm.count(CompilerArgumentSourceFiles) ? vm[CompilerArgumentSourceFiles].as<StringVector>() : StringVector();
                if (sourceJsonFileStrings.empty())
                {
                    KMP_LOG_ERROR_FN("ShaderCompilerTool: source json files are not set");
                    PrintUsage(optionsDescription);
                    return ReturnCode::SourceJsonFilesAreNotSet;
                }
                FilepathVector sourceJsonFilePaths;
                sourceJsonFilePaths.reserve(sourceJsonFileStrings.size());
                for (const auto& sourceJsonFileStr : sourceJsonFileStrings)
                {
                    const auto sourceJsonFilePath = Filepath(sourceJsonFileStr);
                    if (!Filesystem::FilepathExists(sourceJsonFilePath) || !Filesystem::IsFile(sourceJsonFilePath))
                    {
                        KMP_LOG_ERROR_FN("ShaderCompilerTool: one of source files '{}' does not exist or is not of a file type", sourceJsonFilePath);
                        PrintUsage(optionsDescription);
                        return ReturnCode::SourceJsonFilesAreNotValid;
                    }

                    sourceJsonFilePaths.push_back(sourceJsonFilePath);
                }

                compilerParameters.sourceJsonFiles = sourceJsonFilePaths;
            }
            else
            {
                // Shader source files parsing
                const auto shaderSourceFilesStrings = vm.count(CompilerArgumentSourceFiles) ? vm[CompilerArgumentSourceFiles].as<StringVector>() : StringVector();
                if (shaderSourceFilesStrings.empty())
                {
                    KMP_LOG_ERROR_FN("ShaderCompilerTool: shader source files are not set");
                    PrintUsage(optionsDescription);
                    return ReturnCode::ShaderSourceFilesAreNotSet;
                }
                FilepathVector shaderSourceFilePaths;
                shaderSourceFilePaths.reserve(shaderSourceFilesStrings.size());
                for (const auto& shaderSourceFileStr : shaderSourceFilesStrings)
                {
                    const auto shaderSourceFilePath = Filepath(shaderSourceFileStr);
                    if (!Filesystem::FilepathExists(shaderSourceFilePath) || !Filesystem::IsFile(shaderSourceFilePath))
                    {
                        KMP_LOG_ERROR_FN("ShaderCompilerTool: one of shader source files '{}', does not exist of is not of a file type", shaderSourceFilePath);
                        PrintUsage(optionsDescription);
                        return ReturnCode::ShaderSourceFilesAreNotValid;
                    }

                    shaderSourceFilePaths.push_back(shaderSourceFilePath);
                }

                compilerParameters.shaderSourceFiles = shaderSourceFilePaths;


                // Shader binary destination files parsing
                const auto shaderDestinationFilesStrings = vm.count(CompilerArgumentDestinationFiles) ? vm[CompilerArgumentDestinationFiles].as<StringVector>() : StringVector();
                if (shaderDestinationFilesStrings.empty())
                {
                    KMP_LOG_ERROR_FN("ShaderCompilerTool: shader binary destination files are not set");
                    PrintUsage(optionsDescription);
                    return ReturnCode::ShaderBinaryDestinationFilesAreNotSet;
                }
                FilepathVector shaderBinaryDestinationFilePaths;
                shaderBinaryDestinationFilePaths.reserve(shaderDestinationFilesStrings.size());
                for (const auto& shaderBinaryDestinationFileStr : shaderDestinationFilesStrings)
                {
                    shaderBinaryDestinationFilePaths.push_back(Filepath(shaderBinaryDestinationFileStr));
                }

                compilerParameters.shaderBinaryDestinationFiles = shaderBinaryDestinationFilePaths;


                // Shader types parsing
                const auto shaderTypeStrings = vm.count(CompilerArgumentShaderTypes) ? vm[CompilerArgumentShaderTypes].as<StringVector>() : StringVector();
                if (shaderTypeStrings.empty())
                {
                    KMP_LOG_ERROR_FN("ShaderCompilerTool: shader types are not set");
                    PrintUsage(optionsDescription);
                    return ReturnCode::ShaderTypesAreNotSet;
                }
                Vector<ShaderType> shaderTypes;
                shaderTypes.reserve(shaderTypeStrings.size());
                for (const auto& shaderTypeStr : shaderTypeStrings)
                {
                    const auto shaderType = StringToShaderType(shaderTypeStr);
                    if (!shaderType.has_value())
                    {
                        KMP_LOG_ERROR_FN("ShaderCompilerTool: shader type conversion failed");
                        PrintUsage(optionsDescription);
                        return ReturnCode::ShaderTypesAreNotValid;
                    }

                    shaderTypes.push_back(shaderType.value());
                }

                compilerParameters.shaderTypes = shaderTypes;


                // Check parameters size match
                if (compilerParameters.shaderSourceFiles.size() != compilerParameters.shaderBinaryDestinationFiles.size() ||
                    compilerParameters.shaderSourceFiles.size() != compilerParameters.shaderTypes.size())
                {
                    KMP_LOG_ERROR_FN("ShaderCompilerTool: lengths of all three parameters lists should match");
                    PrintUsage(optionsDescription);
                    return ReturnCode::InvalidArgumentVariablesCount;
                }
            }

            return ReturnCode::Ok;
        }
        //--------------------------------------------------------------------------
    }
}