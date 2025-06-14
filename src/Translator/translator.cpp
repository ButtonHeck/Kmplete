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
            auto returnCode = 0;
            if (_parameters.workMode == ProcessorWorkModeUpdate)
            {
                returnCode = Update();
            }
            else if (_parameters.workMode == ProcessorWorkModeCompile)
            {
                returnCode = Compile();
            }

            return returnCode;
        }
        //--------------------------------------------------------------------------

        int TranslatorProcessor::Update() const
        {
            const auto filesToProcess = GatherFilesToUpdate(_parameters);
            if (filesToProcess.empty())
            {
                std::cerr << "TranslatorProcessor::Update: Appropriate files not found\n";
                return ReturnCode::ProcessorFilesNotFound;
            }

            const auto filesList = std::accumulate(filesToProcess.begin(), filesToProcess.end(), String(),
                [](const Path& a, const Path& b) {
                    return a.string() + (a.empty() ? "" : " ") + b.string();
                }
            );

            for (const auto& locale : Locales)
            {
                // 1. Create .po template file
                const auto poTemplateFile = CreatePoTemplateFile(_parameters, locale);
                if (poTemplateFile.empty())
                {
                    std::cerr << "TranslatorProcessor::Update: cannot create .pot file (" << poTemplateFile << ")\n";
                    return ReturnCode::ProcessorCreatePotFailed;
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
                const auto xgettextExitCode = xgettextProcess.exit_code();
                if (xgettextExitCode != 0)
                {
                    std::cerr << "TranslatorProcessor::Update: xgettext error occured\n";
                    return ReturnCode::ProcessorXgettextError;
                }

                // 3. fix charset for generated .pot file
                std::ifstream potFileStream(poTemplateFile);
                if (!potFileStream.is_open() || !potFileStream.good())
                {
                    std::cerr << "TranslatorProcessor::Update: cannot open newly created .pot file (" << poTemplateFile << ")\n";
                    return ReturnCode::ProcessorOpenPotFailed;
                }

                String potFileContent((std::istreambuf_iterator<char>(potFileStream)), std::istreambuf_iterator<char>());
                potFileStream.close();
                potFileContent = Utils::RegexReplace(potFileContent, "CHARSET", "UTF-8");

                std::ofstream potFileOutStream(poTemplateFile, std::ios::trunc);
                potFileOutStream << potFileContent;
                potFileOutStream.close();

                // 4. invoke msginit if necessary for creation of .po file
                const auto poFile = Utils::RegexReplace(poTemplateFile, ".pot", ".po");

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
                    const auto msginitExitCode = msginitProcess.exit_code();
                    if (msginitExitCode != 0)
                    {
                        std::cerr << "TranslatorProcessor::Update: msginit error occured\n";
                        return ReturnCode::ProcessorMsginitError;
                    }
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
                const auto msgattribStep1ExitCode = msgattribProcessStep1.exit_code();
                if (msgattribStep1ExitCode != 0)
                {
                    std::cerr << "TranslatorProcessor::Update: msgattrib error occured\n";
                    return ReturnCode::ProcessorMsgattribError;
                }

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
                const auto msgattribStep2ExitCode = msgattribProcessStep2.exit_code();
                if (msgattribStep2ExitCode != 0)
                {
                    std::cerr << "TranslatorProcessor::Update: msgattrib error occured\n";
                    return ReturnCode::ProcessorMsgattribError;
                }

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
                const auto msgmergeExitCode = msgmergeProcess.exit_code();
                if (msgmergeExitCode != 0)
                {
                    std::cerr << "TranslatorProcessor::Update: msgmerge error occured\n";
                    return ReturnCode::ProcessorMsgmergeError;
                }

                // 8. remove .pot file
                std::filesystem::remove(poTemplateFile);
            }

            return ReturnCode::Ok;
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
                    std::cerr << "TranslatorProcessor::Compile: output directory for locale " << locale << " not found\n";
                    continue;
                }

                auto poTemplateFile = outputDirectory;
                poTemplateFile /= (_parameters.outputFileName + ".po");

                if (!std::filesystem::exists(poTemplateFile))
                {
                    std::cerr << "TranslatorProcessor::Compile: source .pot file not found (" << poTemplateFile << ")\n";
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
                const auto msgfmtExitCode = msgfmtProcess.exit_code();
                if (msgfmtExitCode != 0)
                {
                    std::cerr << "TranslatorProcessor::Update: msgfmt error occured\n";
                    return ReturnCode::ProcessorMsgmergeError;
                }
            }

            return ReturnCode::Ok;
        }
        //--------------------------------------------------------------------------

        PathVector TranslatorProcessor::GatherFilesToUpdate(const TranslatorParameters& parameters) const
        {
            PathVector filesToProcess;
            for (const auto& sourceDirectory : parameters.sourceDirectories)
            {
                if (parameters.isRecursive)
                {
                    for (const auto& directoryEntry : std::filesystem::recursive_directory_iterator(sourceDirectory))
                    {
                        if (IsDirectoryEntryAcceptable(directoryEntry, parameters.filesExtensions))
                        {
                            filesToProcess.push_back(directoryEntry.path());
                        }
                    }
                }
                else
                {
                    for (const auto& directoryEntry : std::filesystem::directory_iterator(sourceDirectory))
                    {
                        if (IsDirectoryEntryAcceptable(directoryEntry, parameters.filesExtensions))
                        {
                            filesToProcess.push_back(directoryEntry.path());
                        }
                    }
                }
            }

            return filesToProcess;
        }
        //--------------------------------------------------------------------------

        bool TranslatorProcessor::IsDirectoryEntryAcceptable(const std::filesystem::directory_entry& directoryEntry, const StringVector& filesExtensions) const
        {
            if (directoryEntry.is_directory() || !directoryEntry.is_regular_file() || !directoryEntry.path().has_extension())
            {
                return false;
            }

            const auto& entryPath = directoryEntry.path();
            const auto entryExtension = entryPath.extension().string();
            return std::find_if(filesExtensions.cbegin(), filesExtensions.cend(),
                [entryExtension](const String& enabledExtension) { return enabledExtension == entryExtension; })
                != filesExtensions.cend();
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
                    std::cerr << "TranslatorProcessor::Update: cannot create .pot file directory hierarchy (" << poTemplateFilePath << ")\n";
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
                std::cerr << "TranslatorProcessor::Update: failed to create .pot file (" << poTemplateFileName << ")\n";
                return String();
            }

            return poTemplateFileName;
        }
        //--------------------------------------------------------------------------
    }
}