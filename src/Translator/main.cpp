#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types.h"
#include "Kmplete/Utils/string_utils.h"

#include <boost/process.hpp>
#include <boost/program_options.hpp>

#include <iostream>
#include <filesystem>
#include <algorithm>
#include <array>
#include <fstream>
#include <streambuf>
#include <numeric>

constexpr const auto WorkingModeUpdate = "Update";
constexpr const auto WorkingModeCompile = "Compile";

constexpr const auto MessagesFacetName = "LC_MESSAGES";
constexpr const auto PotExtension = ".pot";
constexpr const auto PoExtension = ".po";
constexpr const auto MoExtension = ".mo";
//--------------------------------------------------------------------------

using namespace Kmplete;

struct LocaleInfo
{
    const String localeName;
    const String languageShortName;
    const String encoding = "UTF-8";
    const int numPlurals;
};

const LocaleInfo EnglishLocaleInfo = LocaleInfo{
    .localeName = "en_EN",
    .languageShortName = "en",
    .numPlurals = 2
};

const LocaleInfo RussianLocaleInfo = LocaleInfo{
    .localeName = "ru_RU",
    .languageShortName = "ru",
    .numPlurals = 3
};

const std::array<LocaleInfo, 2> LocaleInfos = { EnglishLocaleInfo, RussianLocaleInfo };
//--------------------------------------------------------------------------

struct Context
{
    String workMode;
    std::vector<std::filesystem::path> sourceDirectories;
    StringVector filesExtensions;
    bool isRecursive = false;
    std::filesystem::path outputDirectory;
    String outputFileName;
};
//--------------------------------------------------------------------------

void PrintUsage(const boost::program_options::options_description& description);
bool WorkModeIsValid(const String& workMode);
bool DoUpdate(const Context& context);
bool DoCompile(const Context& context);
boost::program_options::options_description CreateDescription();
void ProcessDirectoryEntry(const std::filesystem::directory_entry& directoryEntry, const StringVector& filesExtensions, std::vector<std::filesystem::path>& filesToProcess);
//--------------------------------------------------------------------------

int main(int argc, char** argv)
{
    boost::program_options::options_description optDescription = CreateDescription();

    if (argc < 2)
    {
        std::cerr << "Translator: invalid argument count\n";
        PrintUsage(optDescription);
        return -1;
    }

    auto cmdParser = boost::program_options::command_line_parser(argc, argv);
    boost::program_options::variables_map vm;
    boost::program_options::store(cmdParser.options(optDescription).run(), vm);
    boost::program_options::notify(vm);

    Context context;

    // Work mode parsing
    const auto workMode = vm.count("mode") ? vm["mode"].as<String>() : String();
    if (workMode.empty())
    {
        std::cerr << "Translator's work mode is not set\n";
        PrintUsage(optDescription);
        return -2;
    }
    if (!WorkModeIsValid(workMode))
    {
        std::cerr << "Translator's work mode is not valid\n";
        PrintUsage(optDescription);
        return -21;
    }

    context.workMode = workMode;

    if (workMode == WorkingModeUpdate)
    {
        // Source directory parsing
        const auto sourceDirectoryStrings = vm.count("input_directories") ? vm["input_directories"].as<StringVector>() : StringVector();
        if (sourceDirectoryStrings.empty())
        {
            std::cerr << "Translator directories are not set\n";
            PrintUsage(optDescription);
            return -3;
        }
        std::vector<std::filesystem::path> sourceDirectories;
        sourceDirectories.reserve(sourceDirectoryStrings.size());
        for (const auto& sourceDirectoryStr : sourceDirectoryStrings)
        {
            const auto sourceDirectory = std::filesystem::path(sourceDirectoryStr);
            if (!std::filesystem::exists(sourceDirectory) || !std::filesystem::is_directory(sourceDirectory))
            {
                std::cerr << "One of translator directories is not exists or is not of a directory type\n";
                PrintUsage(optDescription);
                return -31;
            }

            sourceDirectories.push_back(sourceDirectory);
        }

        context.sourceDirectories = sourceDirectories;

        // files extensions parsing
        const auto filesExtensions = vm.count("extensions") ? vm["extensions"].as<StringVector>() : StringVector();
        if (filesExtensions.empty())
        {
            std::cerr << "Files extensions are not set\n";
            PrintUsage(optDescription);
            return -4;
        }

        context.filesExtensions = filesExtensions;

        // Is recursive parsing
        const bool isRecursive = vm.count("recursive");
        context.isRecursive = isRecursive;
    }

    // output directory parsing
    const auto outputDirectoryStr = vm.count("output_directory") ? vm["output_directory"].as<String>() : String();
    if (outputDirectoryStr.empty())
    {
        std::cerr << "Output directory is not set\n";
        PrintUsage(optDescription);
        return -5;
    }
    const auto outputDirectory = std::filesystem::path(outputDirectoryStr);
    if (!std::filesystem::exists(outputDirectory))
    {
        const auto success = std::filesystem::create_directories(outputDirectory);
        if (!success)
        {
            std::cerr << "Translator cannot create output directory\n";
            PrintUsage(optDescription);
            return -51;
        }
    }

    // output filename parsing
    const auto outputFileNameStr = vm.count("output_file_name") ? vm["output_file_name"].as<String>() : String();
    if (outputFileNameStr.empty())
    {
        std::cerr << "Output file name is not set\n";
        PrintUsage(optDescription);
        return -6;
    }

    context.outputDirectory = outputDirectory;
    context.outputFileName = outputFileNameStr;

    if (workMode == WorkingModeUpdate)
    {
        const auto success = DoUpdate(context);
        if (!success)
        {
            std::cerr << "Translation Update error\n";
            return -100;
        }
    }
    else if (workMode == WorkingModeCompile)
    {
        const auto success = DoCompile(context);
        if (!success)
        {
            std::cerr << "Translation Compile error\n";
            return -101;
        }
    }

    return 0;
}
//--------------------------------------------------------------------------


boost::program_options::options_description CreateDescription()
{
    boost::program_options::options_description optDescription("Translator options");
    optDescription.add_options()
        ("mode,M", boost::program_options::value<String>(), "Working mode (Update, Compile)")
        ("input_directories,D", boost::program_options::value<StringVector>()->multitoken(), "Directories to search files")
        ("extensions,E", boost::program_options::value<StringVector>()->multitoken(), "Files extensions to parse")
        ("recursive,R", "Is recursive search in directory")
        ("output_directory,O", boost::program_options::value<String>(), "Output files directory")
        ("output_file_name,F", boost::program_options::value<String>(), "Output file name");

    return optDescription;
}
//--------------------------------------------------------------------------

void PrintUsage(const boost::program_options::options_description& description)
{
    std::cout << description << std::endl;
}
//--------------------------------------------------------------------------

bool WorkModeIsValid(const String& workMode)
{
    return workMode == WorkingModeUpdate ||
           workMode == WorkingModeCompile;
}
//--------------------------------------------------------------------------

void ProcessDirectoryEntry(const std::filesystem::directory_entry& directoryEntry, const StringVector& filesExtensions, std::vector<std::filesystem::path>& filesToProcess)
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

void GatherFilesToUpdate(std::vector<std::filesystem::path>& filesToProcess, const Context& context)
{
    for (const auto& sourceDirectory : context.sourceDirectories)
    {
        if (context.isRecursive)
        {
            for (const auto& directoryEntry : std::filesystem::recursive_directory_iterator(sourceDirectory))
            {
                ProcessDirectoryEntry(directoryEntry, context.filesExtensions, filesToProcess);
            }
        }
        else
        {
            for (const auto& directoryEntry : std::filesystem::directory_iterator(sourceDirectory))
            {
                ProcessDirectoryEntry(directoryEntry, context.filesExtensions, filesToProcess);
            }
        }
    }
}
//--------------------------------------------------------------------------

String CreatePoTemplateFile(const Context& context, const LocaleInfo& localeInfo)
{
    auto poTemplateFilePath = context.outputDirectory;
    poTemplateFilePath /= localeInfo.localeName;
    poTemplateFilePath /= MessagesFacetName;

    if (!std::filesystem::exists(poTemplateFilePath))
    {
        const auto success = std::filesystem::create_directories(poTemplateFilePath);
        if (!success)
        {
            std::cerr << "Cannot create .pot file directory hierarchy (" << poTemplateFilePath.string() << ")\n";
            return String();
        }
    }

    poTemplateFilePath /= context.outputFileName;
    const String poTemplateFileName = poTemplateFilePath.string().append(PotExtension);

    if (!std::filesystem::exists(poTemplateFileName))
    {
        std::ofstream(poTemplateFileName).flush();
    }

    if (!std::filesystem::exists(poTemplateFileName))
    {
        std::cerr << "Failed to create .pot file (" << poTemplateFileName << ")\n";
        return String();
    }

    return poTemplateFileName;
}
//--------------------------------------------------------------------------

bool DoUpdate(const Context& context)
{
    std::vector<std::filesystem::path> filesToProcess;
    GatherFilesToUpdate(filesToProcess, context);

    if (filesToProcess.empty())
    {
        std::cerr << "DoUpdate: Appropriate files not found\n";
        return false;
    }

    const String filesList = std::accumulate(filesToProcess.begin(), filesToProcess.end(), String(), 
        [](const std::filesystem::path& a, const std::filesystem::path& b) {
            return a.string() + (a.empty() ? "" : " ") + b.string();
        }
    );

    for (const auto& localeInfo : LocaleInfos)
    {
        // 1. Create .po template file
        const String poTemplateFile = CreatePoTemplateFile(context, localeInfo);
        if (poTemplateFile.empty())
        {
            std::cerr << "Cannot create .pot file\n";
            return false;
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
            std::cerr << "Cannot open newly created .pot file (" << poTemplateFile << ")\n";
            return false;
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
                localeInfo.localeName,
                ".",
                localeInfo.encoding
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

    return true;
}
//--------------------------------------------------------------------------

bool DoCompile(const Context& context)
{
    for (const auto& localeInfo : LocaleInfos)
    {
        auto outputDirectory = context.outputDirectory;
        outputDirectory /= localeInfo.localeName;
        outputDirectory /= MessagesFacetName;

        if (!std::filesystem::exists(outputDirectory))
        {
            std::cerr << "Output directory for locale " << localeInfo.localeName << " not found\n";
            continue;
        }

        auto poTemplateFile = outputDirectory;
        poTemplateFile /= (context.outputFileName + PoExtension);

        if (!std::filesystem::exists(poTemplateFile))
        {
            std::cerr << "Source .pot file not found " << poTemplateFile << "\n";
            continue;
        }

        auto moFile = outputDirectory;
        moFile /= (context.outputFileName + MoExtension);

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

    return true;
}
//--------------------------------------------------------------------------