#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types.h"
#include "Kmplete/Utils/string_utils.h"
#include "Kmplete/Translator/translator_interface.h"

#include <filesystem>

namespace Kmplete
{
    namespace Translator
    {
        struct TranslatorParameters
        {
            String workMode;
            std::vector<std::filesystem::path> sourceDirectories;
            StringVector filesExtensions;
            bool isRecursive = false;
            std::filesystem::path outputDirectory;
            String outputFileName;
        };
        //--------------------------------------------------------------------------

        class TranslatorProcessor
        {
        public:
            explicit TranslatorProcessor(TranslatorParameters&& parameters) noexcept;

            KMP_NODISCARD int Run() const;

        private:
            KMP_NODISCARD int Update() const;
            KMP_NODISCARD int Compile() const;

            KMP_NODISCARD std::vector<std::filesystem::path> GatherFilesToUpdate(const TranslatorParameters& parameters) const;
            KMP_NODISCARD bool IsDirectoryEntryAcceptable(const std::filesystem::directory_entry& directoryEntry, const StringVector& filesExtensions) const;
            KMP_NODISCARD String CreatePoTemplateFile(const TranslatorParameters& parameters, const char* locale) const;

        private:
            const TranslatorParameters _parameters;
        };
        //--------------------------------------------------------------------------
    }
}