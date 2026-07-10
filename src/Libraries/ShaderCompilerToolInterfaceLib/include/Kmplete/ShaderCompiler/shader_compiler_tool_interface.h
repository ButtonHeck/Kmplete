#pragma once


namespace Kmplete
{
    namespace ShaderCompiler
    {
        //! Interface data for interaction with the ShaderCompilerTool program
        namespace ShaderCompilerTool
        {
            static constexpr auto CompilerWorkModeDirect = "Direct";
            static constexpr auto CompilerWorkModeJson = "Json";

            static constexpr auto JsonConfigurationTopLevelName = "Shaders";
            static constexpr auto JsonConfigurationSourceFileStr = "Source";
            static constexpr auto JsonConfigurationDestinationFileStr = "Destination";
            static constexpr auto JsonConfigurationShaderTypeStr = "ShaderType";

            static constexpr auto CompilerArgumentLogging = "logging";
            static constexpr auto CompilerArgumentLoggingShort = "L";
            static constexpr auto CompilerArgumentWorkMode = "mode";
            static constexpr auto CompilerArgumentWorkModeShort = "M";
            static constexpr auto CompilerArgumentSourceFiles = "source_files";
            static constexpr auto CompilerArgumentSourceFilesShort = "S";
            static constexpr auto CompilerArgumentDestinationFiles = "destination_files";
            static constexpr auto CompilerArgumentDestinationFilesShort = "D";
            static constexpr auto CompilerArgumentShaderTypes = "shader_types";
            static constexpr auto CompilerArgumentShaderTypesShort = "T";


            enum ReturnCode
            {
                Ok = 0,
                InvalidArgumentCount = -1,
                InvalidArgumentVariablesCount = -11,
                WorkModeIsNotSet = -2,
                WorkModeIsNotValid = -21,
                SourceJsonFilesAreNotSet = -3,
                SourceJsonFilesAreNotValid = -31,
                ShaderSourceFilesAreNotSet = -4,
                ShaderSourceFilesAreNotValid = -41,
                ShaderBinaryDestinationFilesAreNotSet = -5,
                ShaderTypesAreNotSet = -6,
                ShaderTypesAreNotValid = -61,

                ShaderSourceIsEmpty = -70,
                ShaderBinaryBufferIsEmpty = -71,
                ShaderBinaryWriteFileFailed = -72,
                ShaderJsonFileParsingError = -73,
                ShaderJsonFileFormatError = -74,
                ShaderJsonFileDataError = -75
            };
        }
        //--------------------------------------------------------------------------
    }
}