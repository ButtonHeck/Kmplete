#pragma once

namespace Kmplete
{
    namespace Translator
    {
        static constexpr auto ProcessorWorkModeUpdate = "Update";
        static constexpr auto ProcessorWorkModeCompile = "Compile";

        static constexpr auto ProcessorArgumentLogging = "logging";
        static constexpr auto ProcessorArgumentLoggingShort = "L";
        static constexpr auto ProcessorArgumentWorkMode = "mode";
        static constexpr auto ProcessorArgumentWorkModeShort = "M";
        static constexpr auto ProcessorArgumentInputDirectories = "input_directories";
        static constexpr auto ProcessorArgumentInputDirectoriesShort = "D";
        static constexpr auto ProcessorArgumentExtensions = "extensions";
        static constexpr auto ProcessorArgumentExtensionsShort = "E";
        static constexpr auto ProcessorArgumentRecursive = "recursive";
        static constexpr auto ProcessorArgumentRecursiveShort = "R";
        static constexpr auto ProcessorArgumentOutputDirectory = "output_directory";
        static constexpr auto ProcessorArgumentOutputDirectoryShort = "O";
        static constexpr auto ProcessorArgumentOutputFileName = "output_file_name";
        static constexpr auto ProcessorArgumentOutputFileNameShort = "F";

        enum ReturnCode
        {
            Ok = 0,
            InvalidArgumentCount = -1,
            WorkModeIsNotSet = -2,
            WorkModeIsNotValid = -21,
            SourceDirectoriesAreNotSet = -3,
            SourceDirectoriesAreNotValid = -31,
            FilesExtensionsAreNotSet = -4,
            OutputDirectoryIsNotSet = -5,
            OutputDirectoryIsNotValid = -51,
            OutputFileNameIsNotSet = -6,

            ProcessorFilesNotFound = -10,
            ProcessorCreatePotFailed = -11,
            ProcessorOpenPotFailed = -12,

            ProcessorXgettextError = -100,
            ProcessorMsginitError = -101,
            ProcessorMsgattribError = -102,
            ProcessorMsgmergeError = -103,
            ProcessorMsgfmtError = -104,
        };
    }
}