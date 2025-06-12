#pragma once

namespace Kmplete
{
    namespace Translator
    {
        static constexpr auto ProcessorWorkModeUpdate = "Update";
        static constexpr auto ProcessorWorkModeCompile = "Compile";

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
    }
}