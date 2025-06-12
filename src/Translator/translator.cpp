#include "translator.h"

#include <boost/process.hpp>

#include <iostream>
#include <algorithm>
#include <array>
#include <fstream>
#include <streambuf>
#include <numeric>

namespace Kmplete
{
    namespace Translator
    {
        static constexpr std::array<const char*, 2> Locales = {
                "en_EN",
                "ru_RU"
        };


        TranslatorProcessor::TranslatorProcessor(TranslatorParameters&& parameters) noexcept
            : _parameters(parameters)
        {}
        //--------------------------------------------------------------------------

        int TranslatorProcessor::Run() const
        {
            if (_parameters.workMode == ProcessorWorkModeUpdate)
            {
                return Update();
            }
            else if (_parameters.workMode == ProcessorWorkModeCompile)
            {
                return Compile();
            }

            return -1;
        }
        //--------------------------------------------------------------------------

        int TranslatorProcessor::Update() const
        {
            std::vector<std::filesystem::path> filesToProcess;
            GatherFilesToUpdate(filesToProcess, _parameters);

            if (filesToProcess.empty())
            {
                std::cerr << "Translator::Update: Appropriate files not found\n";
                return -1;
            }

            const String filesList = std::accumulate(filesToProcess.begin(), filesToProcess.end(), String(),
                [](const std::filesystem::path& a, const std::filesystem::path& b) {
                    return a.string() + (a.empty() ? "" : " ") + b.string();
                }
            );

            for (const auto& locale : Locales)
            {
                // 1. Create .po template file
                const String poTemplateFile = CreatePoTemplateFile(_parameters, locale);
                if (poTemplateFile.empty())
                {
                    std::cerr << "Translator::Update: cannot create .pot file (" << poTemplateFile << ")\n";
                    return -1;
                }

                // 2. invoke xgettext for .pot
                const auto xgettextCommand = Utils::Concatenate(
                    XGETTEXT_EXECUTABLE_PATH,
                    " -j -C -n ",
                    "-kTranslate:2,2t -kTranslate:2,3,4t ",
                    "-kTranslateCtx:2,3c,3t -kTranslateCtx:2,3,5c,5t ",
                    "-kTranslateDefer:2,2t -kTranslateDefer:2,3,4t ",
                    "-kTranslateCtxDefer:2,3c,3t -kTranslateCtxDefer:2,3,5c,5t -o ",
                    poTemplateFile,
                    " ",
                    filesList);
                auto xgettextProcess = boost::process::child(xgettextCommand);
                xgettextProcess.wait();

                // 3. fix charset for generated .pot file
                std::ifstream potFileStream(poTemplateFile);
                if (!potFileStream.is_open() || !potFileStream.good())
                {
                    std::cerr << "Update: cannot open newly created .pot file (" << poTemplateFile << ")\n";
                    return -1;
                }

                String potFileContent((std::istreambuf_iterator<char>(potFileStream)), std::istreambuf_iterator<char>());
                potFileStream.close();
                const auto charsetStringToReplace = String("CHARSET");
                const auto charsetStringIndex = potFileContent.find(charsetStringToReplace);
                if (charsetStringIndex != String::npos)
                {
                    potFileContent.replace(charsetStringIndex, charsetStringToReplace.size(), "UTF-8");
                }

                std::ofstream potFileOutStream(poTemplateFile, std::ios::trunc);
                potFileOutStream << potFileContent;
                potFileOutStream.close();

                // 4. invoke msginit if necessary for creation of .po file
                String poFile = poTemplateFile;
                poFile.pop_back();

                if (!std::filesystem::exists(poFile))
                {
                    const auto msginitCommand = Utils::Concatenate(
                        MSGINIT_EXECUTABLE_PATH,
                        " -i ",
                        poTemplateFile,
                        " -o ",
                        poFile,
                        " -l ",
                        locale,
                        ".UTF-8"
                    );
                    auto msginitProcess = boost::process::child(msginitCommand);
                    msginitProcess.wait();
                }

                // 5. invoke msgattrib with --set-obsolete flag
                auto msgattribCommand = Utils::Concatenate(
                    MSGATTRIB_EXECUTABLE_PATH,
                    " --set-obsolete --ignore-file=",
                    poTemplateFile,
                    " -o ",
                    poFile,
                    " ",
                    poFile
                );
                auto msgattribProcessStep1 = boost::process::child(msgattribCommand);
                msgattribProcessStep1.wait();

                // 6. invoke msgattrib with --no-obsolete flag
                msgattribCommand = Utils::Concatenate(
                    MSGATTRIB_EXECUTABLE_PATH,
                    " --no-obsolete -o ",
                    poFile,
                    " ",
                    poFile
                );
                auto msgattribProcessStep2 = boost::process::child(msgattribCommand);
                msgattribProcessStep2.wait();

                // 7. invoke msgmerge
                const auto msgmergeCommand = Utils::Concatenate(
                    MSGMERGE_EXECUTABLE_PATH,
                    " -q -o ",
                    poFile,
                    " ",
                    poFile,
                    " ",
                    poTemplateFile
                );
                auto msgmergeProcess = boost::process::child(msgmergeCommand);
                msgmergeProcess.wait();

                // 8. remove .pot file
                std::filesystem::remove(poTemplateFile);
            }

            return 0;
        }
        //--------------------------------------------------------------------------

        int TranslatorProcessor::Compile() const
        {
            for (const auto& locale : Locales)
            {
                auto outputDirectory = _parameters.outputDirectory;
                outputDirectory /= locale;
                outputDirectory /= "LC_MESSAGES";

                if (!std::filesystem::exists(outputDirectory))
                {
                    std::cerr << "Translator::Compile: output directory for locale " << locale << " not found\n";
                    continue;
                }

                auto poTemplateFile = outputDirectory;
                poTemplateFile /= (_parameters.outputFileName + ".po");

                if (!std::filesystem::exists(poTemplateFile))
                {
                    std::cerr << "Translator::Compile: source .pot file not found (" << poTemplateFile << ")\n";
                    continue;
                }

                auto moFile = outputDirectory;
                moFile /= (_parameters.outputFileName + ".mo");

                const auto msgfmtCommand = Utils::Concatenate(
                    MSGFMT_EXECUTABLE_PATH,
                    " -o ",
                    moFile.string(),
                    " ",
                    poTemplateFile.string()
                );
                auto msgfmtProcess = boost::process::child(msgfmtCommand);
                msgfmtProcess.wait();
            }

            return 0;
        }
        //--------------------------------------------------------------------------

        String TranslatorProcessor::CreatePoTemplateFile(const TranslatorParameters& parameters, const char* locale) const
        {
            auto poTemplateFilePath = parameters.outputDirectory;
            poTemplateFilePath /= locale;
            poTemplateFilePath /= "LC_MESSAGES";

            if (!std::filesystem::exists(poTemplateFilePath))
            {
                const auto success = std::filesystem::create_directories(poTemplateFilePath);
                if (!success)
                {
                    std::cerr << "Translator::Update: cannot create .pot file directory hierarchy (" << poTemplateFilePath << ")\n";
                    return String();
                }
            }

            poTemplateFilePath /= parameters.outputFileName;
            const String poTemplateFileName = poTemplateFilePath.string().append(".pot");

            if (!std::filesystem::exists(poTemplateFileName))
            {
                std::ofstream(poTemplateFileName).flush();
            }

            if (!std::filesystem::exists(poTemplateFileName))
            {
                std::cerr << "Translator::Update: failed to create .pot file (" << poTemplateFileName << ")\n";
                return String();
            }

            return poTemplateFileName;
        }
        //--------------------------------------------------------------------------

        void TranslatorProcessor::ProcessDirectoryEntry(const std::filesystem::directory_entry& directoryEntry, const StringVector& filesExtensions, std::vector<std::filesystem::path>& filesToProcess) const
        {
            if (directoryEntry.is_directory() || !directoryEntry.is_regular_file() || !directoryEntry.path().has_extension())
            {
                return;
            }

            const auto& entryPath = directoryEntry.path();
            const auto entryExtension = entryPath.extension().string();
            if (std::find_if(filesExtensions.cbegin(), filesExtensions.cend(),
                [entryExtension](const String& enabledExtension) { return enabledExtension == entryExtension; })
                != filesExtensions.cend())
            {
                filesToProcess.push_back(entryPath);
            }
        }
        //--------------------------------------------------------------------------

        void TranslatorProcessor::GatherFilesToUpdate(std::vector<std::filesystem::path>& filesToProcess, const TranslatorParameters& parameters) const
        {
            for (const auto& sourceDirectory : parameters.sourceDirectories)
            {
                if (parameters.isRecursive)
                {
                    for (const auto& directoryEntry : std::filesystem::recursive_directory_iterator(sourceDirectory))
                    {
                        ProcessDirectoryEntry(directoryEntry, parameters.filesExtensions, filesToProcess);
                    }
                }
                else
                {
                    for (const auto& directoryEntry : std::filesystem::directory_iterator(sourceDirectory))
                    {
                        ProcessDirectoryEntry(directoryEntry, parameters.filesExtensions, filesToProcess);
                    }
                }
            }
        }
        //--------------------------------------------------------------------------
    }
}