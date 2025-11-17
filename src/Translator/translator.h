#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Translator/translator_interface.h"
#include "Kmplete/Log/log_class_macro.h"


namespace Kmplete
{
    namespace Translator
    {
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