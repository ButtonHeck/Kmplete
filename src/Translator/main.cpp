#include <boost/process.hpp>
#include <boost/program_options.hpp>

#include <iostream>
#include <filesystem>
#include <string>
#include <algorithm>
#include <array>
#include <fstream>
#include <streambuf>
#include <numeric>
#include <sstream>

constexpr const auto WorkingModeUpdate = "Update";
constexpr const auto WorkingModeCompile = "Compile";

constexpr const auto MessagesFacetName = "LC_MESSAGES";
constexpr const auto PotExtension = ".pot";
constexpr const auto PoExtension = ".po";
constexpr const auto MoExtension = ".mo";
//--------------------------------------------------------------------------

struct LocaleInfo
{
    const std::string localeName;
    const std::string languageShortName;
    const std::string encoding = "UTF-8";
    const int numPlurals;
    const std::string pluralExpression;
};

const LocaleInfo EnglishLocaleInfo = LocaleInfo{
    .localeName = "en_EN",
    .languageShortName = "en",
    .numPlurals = 2,
    .pluralExpression = "n != 1"
};

const LocaleInfo RussianLocaleInfo = LocaleInfo{
    .localeName = "ru_RU",
    .languageShortName = "ru",
    .numPlurals = 3,
    .pluralExpression = "n%10==1 && n%100!=11 ? 0 : n%10>=2 && n%10<=4 && (n%100<10 || n%100>=20) ? 1 : 2"
};

const std::array<LocaleInfo, 2> LocaleInfos = { EnglishLocaleInfo, RussianLocaleInfo };
//--------------------------------------------------------------------------

struct Context
{
    std::string workMode;
    std::vector<std::filesystem::path> sourceDirectories;
    std::vector<std::string> filesExtensions;
    bool isRecursive = false;
    std::filesystem::path outputDirectory;
    std::string outputFileName;
};
//--------------------------------------------------------------------------

void PrintUsage(const boost::program_options::options_description& description);
bool WorkModeIsValid(const std::string& workMode);
bool DoUpdate(const Context& context);
bool DoCompile(const Context& context);
boost::program_options::options_description CreateDescription();
void ProcessDirectoryEntry(const std::filesystem::directory_entry& directoryEntry, const std::vector<std::string>& filesExtensions, std::vector<std::filesystem::path>& filesToProcess);
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
    const auto workMode = vm.count("mode") ? vm["mode"].as<std::string>() : "";
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
        const auto sourceDirectoryStrings = vm.count("input_directories") ? vm["input_directories"].as<std::vector<std::string>>() : std::vector<std::string>();
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
        const auto filesExtensions = vm.count("extensions") ? vm["extensions"].as<std::vector<std::string>>() : std::vector<std::string>();
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
    const auto outputDirectoryStr = vm.count("output_directory") ? vm["output_directory"].as<std::string>() : "";
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
    const auto outputFileNameStr = vm.count("output_file_name") ? vm["output_file_name"].as<std::string>() : "";
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
        ("mode,M", boost::program_options::value<std::string>(), "Working mode (Update, Compile)")
        ("input_directories,D", boost::program_options::value<std::vector<std::string>>()->multitoken(), "Directories to search files")
        ("extensions,E", boost::program_options::value<std::vector<std::string>>()->multitoken(), "Files extensions to parse")
        ("recursive,R", "Is recursive search in directory")
        ("output_directory,O", boost::program_options::value<std::string>(), "Output files directory")
        ("output_file_name,F", boost::program_options::value<std::string>(), "Output file name");

    return optDescription;
}
//--------------------------------------------------------------------------

void PrintUsage(const boost::program_options::options_description& description)
{
    std::cout << description << std::endl;
}
//--------------------------------------------------------------------------

bool WorkModeIsValid(const std::string& workMode)
{
    return workMode == WorkingModeUpdate ||
           workMode == WorkingModeCompile;
}
//--------------------------------------------------------------------------

void ProcessDirectoryEntry(const std::filesystem::directory_entry& directoryEntry, const std::vector<std::string>& filesExtensions, std::vector<std::filesystem::path>& filesToProcess)
{
    if (directoryEntry.is_directory() || !directoryEntry.is_regular_file() || !directoryEntry.path().has_extension())
    {
        return;
    }

    const auto& entryPath = directoryEntry.path();
    const std::string extension = entryPath.extension().string();
    if (std::find_if(filesExtensions.cbegin(), filesExtensions.cend(),
        [extension](const std::string& supportedExtension) { return supportedExtension == extension; })
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

std::string CreatePoTemplateFile(const Context& context, const LocaleInfo& localeInfo)
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
            return std::string();
        }
    }

    poTemplateFilePath /= context.outputFileName;
    const std::string poTemplateFileName = poTemplateFilePath.string().append(PotExtension);

    if (!std::filesystem::exists(poTemplateFileName))
    {
        std::ofstream(poTemplateFileName).flush();
    }

    if (!std::filesystem::exists(poTemplateFileName))
    {
        std::cerr << "Failed to create .pot file (" << poTemplateFileName << ")\n";
        return std::string();
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

    const std::string filesList = std::accumulate(filesToProcess.begin(), filesToProcess.end(), std::string(), 
        [](const std::filesystem::path& a, const std::filesystem::path& b) {
            return a.string() + (a.empty() ? "" : " ") + b.string();
        }
    );

    for (const auto& localeInfo : LocaleInfos)
    {
        // 1. Create .po template file
        const std::string poTemplateFile = CreatePoTemplateFile(context, localeInfo);
        if (poTemplateFile.empty())
        {
            std::cerr << "Cannot create .pot file\n";
            return false;
        }

        // 2. invoke xgettext for .pot
        std::string xgettextCommand = std::string(XGETTEXT_EXECUTABLE_PATH);
        xgettextCommand.append(" -j -C -n -kTranslate:2,2t -kTranslate:2,3,4t -kTranslateCtx:2,3c,3t -kTranslateCtx:2,3,5c,5t -kTranslateDefer:2,2t -kTranslateDefer:2,3,4t -kTranslateCtxDefer:2,3c,3t -kTranslateCtxDefer:2,3,5c,5t -o ");
        xgettextCommand.append(poTemplateFile);
        xgettextCommand.append(" ");
        xgettextCommand.append(filesList);
        auto xgettextProgram = boost::process::child(xgettextCommand);
        xgettextProgram.wait();

        // 3. fix charset for generated .pot file
        std::ifstream potFileStream(poTemplateFile);
        if (!potFileStream.is_open() || !potFileStream.good())
        {
            std::cerr << "Cannot open newly created .pot file (" << poTemplateFile << ")\n";
            return false;
        }

        std::string potFileContent((std::istreambuf_iterator<char>(potFileStream)), std::istreambuf_iterator<char>());
        potFileStream.close();
        const auto charsetStringToReplace = std::string("CHARSET");
        const auto charsetStringIndex = potFileContent.find(charsetStringToReplace);
        if (charsetStringIndex != std::string::npos)
        {
            potFileContent.replace(charsetStringIndex, charsetStringToReplace.size(), "UTF-8");
        }

        std::ofstream potFileOutStream(poTemplateFile, std::ios::trunc);
        potFileOutStream << potFileContent;
        potFileOutStream.close();
        
        // 4. invoke msginit if necessary for creation of .po file
        std::string poFile = poTemplateFile;
        poFile.pop_back();

        if (!std::filesystem::exists(poFile))
        {
            std::string msginitCommand = std::string(MSGINIT_EXECUTABLE_PATH);
            msginitCommand.append(" -i ");
            msginitCommand.append(poTemplateFile);
            msginitCommand.append(" -o ");
            msginitCommand.append(poFile);
            msginitCommand.append(" -l ");
            msginitCommand.append(localeInfo.localeName).append(".").append(localeInfo.encoding);
            auto msginitProgram = boost::process::child(msginitCommand);
            msginitProgram.wait();
        }

        // 5. invoke msgattrib with --set-obsolete flag
        std::string msgattribCommand = std::string(MSGATTRIB_EXECUTABLE_PATH);
        msgattribCommand.append(" --set-obsolete --ignore-file=");
        msgattribCommand.append(poTemplateFile);
        msgattribCommand.append(" -o ");
        msgattribCommand.append(poFile);
        msgattribCommand.append(" ");
        msgattribCommand.append(poFile);
        auto msgattribProgramStep1 = boost::process::child(msgattribCommand);
        msgattribProgramStep1.wait();

        // 6. invoke msgattrib with --no-obsolete flag
        msgattribCommand = std::string(MSGATTRIB_EXECUTABLE_PATH);
        msgattribCommand.append(" --no-obsolete -o ");
        msgattribCommand.append(poFile);
        msgattribCommand.append(" ");
        msgattribCommand.append(poFile);
        auto msgattribProgramStep2 = boost::process::child(msgattribCommand);
        msgattribProgramStep2.wait();

        // 7. invoke msgmerge
        std::string msgmergeCommand = std::string(MSGMERGE_EXECUTABLE_PATH);
        msgmergeCommand.append(" -q -o ");
        msgmergeCommand.append(poFile);
        msgmergeCommand.append(" ");
        msgmergeCommand.append(poFile);
        msgmergeCommand.append(" ");
        msgmergeCommand.append(poTemplateFile);
        auto msgmergeProgram = boost::process::child(msgmergeCommand);
        msgmergeProgram.wait();

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

        std::string msgfmtCommand = std::string(MSGFMT_EXECUTABLE_PATH);
        msgfmtCommand.append(" -o ");
        msgfmtCommand.append(moFile.string());
        msgfmtCommand.append(" ");
        msgfmtCommand.append(poTemplateFile.string());
        auto msgfmtProgram = boost::process::child(msgfmtCommand);
        msgfmtProgram.wait();
    }

    return true;
}
//--------------------------------------------------------------------------