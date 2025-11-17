#pragma once


namespace Kmplete
{
    namespace Assets
    {
        namespace Compiler
        {
            static constexpr auto JsonConfigurationTopLevelName = "Assets";
            static constexpr auto JsonConfigurationFileStr = "File";
            static constexpr auto JsonConfigurationTypeStr = "Type";
            static constexpr auto JsonConfigurationSidStr = "Sid";

            static constexpr auto CompilerArgumentLogging = "logging";
            static constexpr auto CompilerArgumentLoggingShort = "L";
            static constexpr auto CompilerArgumentInputFileName = "input_file_name";
            static constexpr auto CompilerArgumentInputFileNameShort = "I";
            static constexpr auto CompilerArgumentOutputFileName = "output_file_name";
            static constexpr auto CompilerArgumentOutputFileNameShort = "O";


            enum ReturnCode
            {
                Ok = 0,
                InvalidArgumentCount = -1,

                InputFileIsNotSet = -20,
                InputFileIsNotValid = -21,
                InputFileParsingError = -22,
                InputFileFormatError = -23,
                InputFileProcessingError = -24,
                InputFileDuplicationsError = -25,

                OutputFileIsNotSet = -30,
                OutputFileCreationFailed = -31,
                OutputFileOpeningFailed = -32
            };
        }
        //--------------------------------------------------------------------------
    }
}