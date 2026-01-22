#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Translator/translator_interface.h"
#include "Kmplete/Log/log_class_macro.h"


namespace Kmplete
{
    namespace Translator
    {
        //! Representation of the Translator program arguments
        //! --mode
        //! --input_directories
        //! --extensions
        //! --recursive
        //! --output_directory
        //! --output_file_name
        //! --logging
        struct TranslatorParameters
        {
            String workMode;
            FilepathVector sourceDirectories;
            StringVector filesExtensions;
            bool isRecursive = false;
            Filepath outputDirectory;
            String outputFileName;
            bool logging = false;
        };
        //--------------------------------------------------------------------------


        //! The proccessor that takes directories containing files with sources needed to translate
        //! and updates/compiles translations to a single .mo file. Uses gettext family of programs
        //! as backend (ran as sub-processses). This processor is capable of running in one of two modes:
        //! 1) updating translations - recursively gathering all files (with suitable extensions) from source 
        //! directories and creating .po template files and 2) compiling translations - using .po files to create .mo files 
        //! with translations. Between those steps translations must be somehow added in text form.
        //! Overall translation process looks like this:
        //! 1) invoke Translator with --mode=Update --other_params=...
        //! 2) add translations in files generated from previous step (via Notepad or any other editor)
        //! 3) invoke Translator with --mode=Compile --other_params=...
        //! 4*) Copy/move generated translation .mo files to an appropriate application folder (* This step should be
        //! either done manually or automated by other means)
        class TranslatorProcessor
        {
            KMP_LOG_CLASSNAME(TranslatorProcessor)

        public:
            explicit TranslatorProcessor(TranslatorParameters&& parameters) noexcept;

            KMP_NODISCARD ReturnCode Run() const;

        private:
            KMP_NODISCARD ReturnCode _Update() const;
            KMP_NODISCARD ReturnCode _Compile() const;

            KMP_NODISCARD FilepathVector _GatherFilesToUpdate(const TranslatorParameters& parameters) const;
            KMP_NODISCARD bool _IsDirectoryEntryAcceptable(const std::filesystem::directory_entry& directoryEntry, const StringVector& filesExtensions) const;
            KMP_NODISCARD String _CreatePoTemplateFile(const TranslatorParameters& parameters, const char* locale) const;

        private:
            const TranslatorParameters _parameters;
        };
        //--------------------------------------------------------------------------
    }
}