#include "Kmplete/FileDialogs/file_dialogs.h"
#include "Kmplete/Filesystem/filesystem.h"
#include "Kmplete/Localization/localization_manager.h"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("File dialog open single file - OPEN", "[file_dialogs][open]")
{
    auto localizationManager = Kmplete::LocalizationManager();
    localizationManager.SetLocale("ru_RU.UTF8");

    const auto file = Kmplete::FileDialogs::OpenFile("Open any single file", Kmplete::Filesystem::GetCurrentFilepath());
    REQUIRE(!file.empty());
    REQUIRE(Kmplete::Filesystem::FilepathIsValid(file));
    REQUIRE(Kmplete::Filesystem::FilepathExists(file));
}

TEST_CASE("File dialog open single file - CANCEL", "[file_dialogs][open]")
{
    const auto file = Kmplete::FileDialogs::OpenFile("Press cancel", Kmplete::Filesystem::GetCurrentFilepath());
    REQUIRE(file.empty());
}

TEST_CASE("File dialog open single file - OPEN json", "[file_dialogs][open]")
{
    auto localizationManager = Kmplete::LocalizationManager();
    localizationManager.SetLocale("ru_RU.UTF8");

    const auto file = Kmplete::FileDialogs::OpenFile("Open any single json file", Kmplete::Filesystem::GetCurrentFilepath(), { "JSON Files", "*.json" });
    REQUIRE(!file.empty());
    REQUIRE(Kmplete::Filesystem::ToGenericString(file).ends_with(".json"));
    REQUIRE(Kmplete::Filesystem::FilepathIsValid(file));
    REQUIRE(Kmplete::Filesystem::FilepathExists(file));
}

TEST_CASE("File dialog open single file - OPEN manual name non-existent", "[file_dialogs][open]")
{
    auto localizationManager = Kmplete::LocalizationManager();
    localizationManager.SetLocale("ru_RU.UTF8");

    const auto file = Kmplete::FileDialogs::OpenFile("Open any manually entered non-existing file", Kmplete::Filesystem::GetCurrentFilepath());
    REQUIRE(!file.empty());
    REQUIRE(Kmplete::Filesystem::FilepathIsValid(file));
    REQUIRE_FALSE(Kmplete::Filesystem::FilepathExists(file));
}

TEST_CASE("File dialog open single file - OPEN manual name existing", "[file_dialogs][open]")
{
    auto localizationManager = Kmplete::LocalizationManager();
    localizationManager.SetLocale("ru_RU.UTF8");

    const auto file = Kmplete::FileDialogs::OpenFile("Open any manually entered existing file", Kmplete::Filesystem::GetCurrentFilepath());
    REQUIRE(!file.empty());
    REQUIRE(Kmplete::Filesystem::FilepathIsValid(file));
    REQUIRE(Kmplete::Filesystem::FilepathExists(file));
}

TEST_CASE("File dialog open single file - OPEN manual name existing non-match filter", "[file_dialogs][open]")
{
    auto localizationManager = Kmplete::LocalizationManager();
    localizationManager.SetLocale("ru_RU.UTF8");

    const auto file = Kmplete::FileDialogs::OpenFile("Open any manually entered existing file that do not match filter", Kmplete::Filesystem::GetCurrentFilepath(), { "JSON Files", "*.json" });
    REQUIRE(!file.empty());
    REQUIRE(Kmplete::Filesystem::FilepathIsValid(file));
    REQUIRE(Kmplete::Filesystem::FilepathExists(file));
}
//--------------------------------------------------------------------------

TEST_CASE("File dialog open multiple files - OPEN", "[file_dialogs][open]")
{
    auto localizationManager = Kmplete::LocalizationManager();
    localizationManager.SetLocale("ru_RU.UTF8");

    const auto files = Kmplete::FileDialogs::OpenFiles("Open any multiple files", Kmplete::Filesystem::GetCurrentFilepath());
    REQUIRE(!files.empty());
    bool ok = true;
    for (const auto& file : files)
    {
        ok &= Kmplete::Filesystem::FilepathIsValid(file);
        ok &= Kmplete::Filesystem::FilepathExists(file);

        if (!ok)
        {
            WARN(file);
            break;
        }
    }

    REQUIRE(ok);
}

TEST_CASE("File dialog open multiple files - CANCEL", "[file_dialogs][open]")
{
    const auto files = Kmplete::FileDialogs::OpenFiles("Press cancel", Kmplete::Filesystem::GetCurrentFilepath());
    REQUIRE(files.empty());
}

TEST_CASE("File dialog open multiple files - OPEN json", "[file_dialogs][open]")
{
    auto localizationManager = Kmplete::LocalizationManager();
    localizationManager.SetLocale("ru_RU.UTF8");

    const auto files = Kmplete::FileDialogs::OpenFiles("Open any multiple json files", Kmplete::Filesystem::GetCurrentFilepath(), { "JSON Files", "*.json" });
    REQUIRE(!files.empty());
    bool ok = true;
    for (const auto& file : files)
    {
        ok &= Kmplete::Filesystem::FilepathIsValid(file);
        ok &= Kmplete::Filesystem::FilepathExists(file);
        ok &= Kmplete::Filesystem::ToGenericString(file).ends_with(".json");

        if (!ok)
        {
            WARN(file);
            break;
        }
    }

    REQUIRE(ok);
}

TEST_CASE("File dialog open multiple files - OPEN manual names non-existent", "[file_dialogs][open]")
{
    auto localizationManager = Kmplete::LocalizationManager();
    localizationManager.SetLocale("ru_RU.UTF8");

    const auto files = Kmplete::FileDialogs::OpenFiles("Open any manually entered non-existing files", Kmplete::Filesystem::GetCurrentFilepath());
    REQUIRE(!files.empty());
    bool ok = true;
    for (const auto& file : files)
    {
        ok &= Kmplete::Filesystem::FilepathIsValid(file);
        ok &= !Kmplete::Filesystem::FilepathExists(file);

        if (!ok)
        {
            WARN(file);
            break;
        }
    }

    REQUIRE(ok);
}

TEST_CASE("File dialog open multiple files - OPEN manual names existent", "[file_dialogs][open]")
{
    auto localizationManager = Kmplete::LocalizationManager();
    localizationManager.SetLocale("ru_RU.UTF8");

    const auto files = Kmplete::FileDialogs::OpenFiles("Open any manually entered existing files", Kmplete::Filesystem::GetCurrentFilepath());
    REQUIRE(!files.empty());
    bool ok = true;
    for (const auto& file : files)
    {
        ok &= Kmplete::Filesystem::FilepathIsValid(file);
        ok &= Kmplete::Filesystem::FilepathExists(file);

        if (!ok)
        {
            WARN(file);
            break;
        }
    }

    REQUIRE(ok);
}
//--------------------------------------------------------------------------

TEST_CASE("File dialog save file - SAVE new", "[file_dialogs][save]")
{
    auto localizationManager = Kmplete::LocalizationManager();
    localizationManager.SetLocale("ru_RU.UTF8");

    KMP_MB_UNUSED const auto res = Kmplete::FileDialogs::OpenMessage("File dialog save", "In the next dialog save to any new file", Kmplete::FileDialogs::MessageChoice::Ok);
    const auto file = Kmplete::FileDialogs::SaveFile("Save to any new file", Kmplete::Filesystem::GetCurrentFilepath());
    REQUIRE(!file.empty());
    REQUIRE(Kmplete::Filesystem::FilepathIsValid(file));
    REQUIRE_FALSE(Kmplete::Filesystem::FilepathExists(file));
}

TEST_CASE("File dialog save file - CANCEL", "[file_dialogs][save]")
{
    KMP_MB_UNUSED const auto res = Kmplete::FileDialogs::OpenMessage("File dialog save", "In the next dialog press Cancel", Kmplete::FileDialogs::MessageChoice::Ok);
    const auto file = Kmplete::FileDialogs::SaveFile("Press Cancel", Kmplete::Filesystem::GetCurrentFilepath());
    REQUIRE(file.empty());
}

TEST_CASE("File dialog save file - SAVE overwrite", "[file_dialogs][save]")
{
    auto localizationManager = Kmplete::LocalizationManager();
    localizationManager.SetLocale("ru_RU.UTF8");

    KMP_MB_UNUSED const auto res = Kmplete::FileDialogs::OpenMessage("File dialog save", "In the next dialog save to any existing file", Kmplete::FileDialogs::MessageChoice::Ok);
    const auto file = Kmplete::FileDialogs::SaveFile("Save to any existing file", Kmplete::Filesystem::GetCurrentFilepath(), {"Any files", "*.*"}, true);
    REQUIRE(!file.empty());
    REQUIRE(Kmplete::Filesystem::FilepathExists(file));
    REQUIRE(Kmplete::Filesystem::IsFile(file));
}

TEST_CASE("File dialog save file - SAVE new non-matching filter", "[file_dialogs][save]")
{
    auto localizationManager = Kmplete::LocalizationManager();
    localizationManager.SetLocale("ru_RU.UTF8");

    KMP_MB_UNUSED const auto res = Kmplete::FileDialogs::OpenMessage("File dialog save", "In the next dialog save to any new file that do not match filter", Kmplete::FileDialogs::MessageChoice::Ok);
    const auto file = Kmplete::FileDialogs::SaveFile("Save to any new file that do not match filter", Kmplete::Filesystem::GetCurrentFilepath(), {"JSON files", "*.json"});
    REQUIRE(!file.empty());
    REQUIRE(Kmplete::Filesystem::FilepathIsValid(file));
    REQUIRE_FALSE(Kmplete::Filesystem::FilepathExists(file));
}

TEST_CASE("File dialog save file - Cyrillic characters", "[file_dialogs][open]")
{
    auto localizationManager = Kmplete::LocalizationManager();
    localizationManager.SetLocale("ru_RU.UTF8");

    KMP_MB_UNUSED const auto res = Kmplete::FileDialogs::OpenMessage("File dialog save", "In the next dialog save to new file with cyrillic characters", Kmplete::FileDialogs::MessageChoice::Ok);
    const auto file = Kmplete::FileDialogs::SaveFile("Save to new file with cyrillic characters", Kmplete::Filesystem::GetCurrentFilepath());
    REQUIRE(!file.empty());
    REQUIRE(Kmplete::Filesystem::FilepathIsValid(file));
}
//--------------------------------------------------------------------------

TEST_CASE("File dialog open folder - OPEN", "[file_dialogs][folder]")
{
    auto localizationManager = Kmplete::LocalizationManager();
    localizationManager.SetLocale("ru_RU.UTF8");

    const auto folder = Kmplete::FileDialogs::OpenDirectory("Open any directory", Kmplete::Filesystem::GetCurrentFilepath());
    REQUIRE(!folder.empty());
    REQUIRE(Kmplete::Filesystem::IsDirectory(folder));
}

TEST_CASE("File dialog open folder - CANCEL", "[file_dialogs][folder]")
{
    const auto folder = Kmplete::FileDialogs::OpenDirectory("Press Cancel", Kmplete::Filesystem::GetCurrentFilepath());
    REQUIRE(folder.empty());
}
//--------------------------------------------------------------------------

TEST_CASE("Message dialog OK button", "[file_dialogs][message]")
{
    auto messageResult = Kmplete::FileDialogs::OpenMessage("Message dialog test", "Press OK");
    REQUIRE(messageResult == Kmplete::FileDialogs::MessageButton::Ok);

    messageResult = Kmplete::FileDialogs::OpenMessage("Message dialog test", "Press OK", Kmplete::FileDialogs::MessageChoice::Ok);
    REQUIRE(messageResult == Kmplete::FileDialogs::MessageButton::Ok);
}

TEST_CASE("Message dialog CANCEL button", "[file_dialogs][message]")
{
    auto messageResult = Kmplete::FileDialogs::OpenMessage("Message dialog test", "Press CANCEL");
    REQUIRE(messageResult == Kmplete::FileDialogs::MessageButton::Cancel);

    messageResult = Kmplete::FileDialogs::OpenMessage("Message dialog test", "Press CANCEL", Kmplete::FileDialogs::MessageChoice::YesNoCancel);
    REQUIRE(messageResult == Kmplete::FileDialogs::MessageButton::Cancel);

    messageResult = Kmplete::FileDialogs::OpenMessage("Message dialog test", "Press CANCEL", Kmplete::FileDialogs::MessageChoice::RetryCancel);
    REQUIRE(messageResult == Kmplete::FileDialogs::MessageButton::Cancel);
}

TEST_CASE("Message dialog close button", "[file_dialogs][message]")
{
    auto messageResult = Kmplete::FileDialogs::OpenMessage("Message dialog test", "Close this dialog (Expect Cancel)");
    REQUIRE(messageResult == Kmplete::FileDialogs::MessageButton::Cancel);

    // Windows returns OK, Ubuntu's Unity returns Cancel
    messageResult = Kmplete::FileDialogs::OpenMessage("Message dialog test", "Close this dialog (Expect OK or Cancel)", Kmplete::FileDialogs::MessageChoice::Ok);
    REQUIRE((messageResult == Kmplete::FileDialogs::MessageButton::Ok || messageResult == Kmplete::FileDialogs::MessageButton::Cancel));

    messageResult = Kmplete::FileDialogs::OpenMessage("Message dialog test", "Close this dialog (Expect Cancel)", Kmplete::FileDialogs::MessageChoice::RetryCancel);
    REQUIRE(messageResult == Kmplete::FileDialogs::MessageButton::Cancel);

    messageResult = Kmplete::FileDialogs::OpenMessage("Message dialog test", "Close this dialog (Expect Cancel)", Kmplete::FileDialogs::MessageChoice::YesNoCancel);
    REQUIRE(messageResult == Kmplete::FileDialogs::MessageButton::Cancel);
}

TEST_CASE("Message dialog YES button", "[file_dialogs][message]")
{
    auto messageResult = Kmplete::FileDialogs::OpenMessage("Message dialog test", "Press YES", Kmplete::FileDialogs::MessageChoice::YesNo);
    REQUIRE(messageResult == Kmplete::FileDialogs::MessageButton::Yes);

    messageResult = Kmplete::FileDialogs::OpenMessage("Message dialog test", "Press YES", Kmplete::FileDialogs::MessageChoice::YesNoCancel);
    REQUIRE(messageResult == Kmplete::FileDialogs::MessageButton::Yes);
}

TEST_CASE("Message dialog NO button", "[file_dialogs][message]")
{
    auto messageResult = Kmplete::FileDialogs::OpenMessage("Message dialog test", "Press NO", Kmplete::FileDialogs::MessageChoice::YesNo);
    REQUIRE(messageResult == Kmplete::FileDialogs::MessageButton::No);

    messageResult = Kmplete::FileDialogs::OpenMessage("Message dialog test", "Press NO", Kmplete::FileDialogs::MessageChoice::YesNoCancel);
    REQUIRE(messageResult == Kmplete::FileDialogs::MessageButton::No);
}

TEST_CASE("Message dialog RETRY button", "[file_dialogs][message]")
{
    auto messageResult = Kmplete::FileDialogs::OpenMessage("Message dialog test", "Press RETRY", Kmplete::FileDialogs::MessageChoice::RetryCancel);
    REQUIRE(messageResult == Kmplete::FileDialogs::MessageButton::Retry);

    messageResult = Kmplete::FileDialogs::OpenMessage("Message dialog test", "Press RETRY", Kmplete::FileDialogs::MessageChoice::AbortRetryIgnore);
    REQUIRE(messageResult == Kmplete::FileDialogs::MessageButton::Retry);
}

TEST_CASE("Message dialog ABORT button", "[file_dialogs][message]")
{
    const auto messageResult = Kmplete::FileDialogs::OpenMessage("Message dialog test", "Press ABORT", Kmplete::FileDialogs::MessageChoice::AbortRetryIgnore);
    REQUIRE(messageResult == Kmplete::FileDialogs::MessageButton::Abort);
}

TEST_CASE("Message dialog IGNORE button", "[file_dialogs][message]")
{
    const auto messageResult = Kmplete::FileDialogs::OpenMessage("Message dialog test", "Press IGNORE", Kmplete::FileDialogs::MessageChoice::AbortRetryIgnore);
    REQUIRE(messageResult == Kmplete::FileDialogs::MessageButton::Ignore);
}
//--------------------------------------------------------------------------

TEST_CASE("Message dialog icon INFO", "[file_dialogs][message]")
{
    const auto messageResult = Kmplete::FileDialogs::OpenMessage("Message dialog test", "Press YES if this dialog looks like information dialog", 
        Kmplete::FileDialogs::MessageChoice::YesNo, Kmplete::FileDialogs::MessageIcon::Info);
    REQUIRE(messageResult == Kmplete::FileDialogs::MessageButton::Yes);
}

TEST_CASE("Message dialog icon WARNING", "[file_dialogs][message]")
{
    const auto messageResult = Kmplete::FileDialogs::OpenMessage("Message dialog test", "Press YES if this dialog looks like warning dialog",
        Kmplete::FileDialogs::MessageChoice::YesNo, Kmplete::FileDialogs::MessageIcon::Warning);
    REQUIRE(messageResult == Kmplete::FileDialogs::MessageButton::Yes);
}

TEST_CASE("Message dialog icon ERROR", "[file_dialogs][message]")
{
    const auto messageResult = Kmplete::FileDialogs::OpenMessage("Message dialog test", "Press YES if this dialog looks like error dialog",
        Kmplete::FileDialogs::MessageChoice::YesNo, Kmplete::FileDialogs::MessageIcon::Error);
    REQUIRE(messageResult == Kmplete::FileDialogs::MessageButton::Yes);
}

TEST_CASE("Message dialog icon QUESTION", "[file_dialogs][message]")
{
    const auto messageResult = Kmplete::FileDialogs::OpenMessage("Message dialog test", "Press YES if this dialog looks like question dialog",
        Kmplete::FileDialogs::MessageChoice::YesNo, Kmplete::FileDialogs::MessageIcon::Question);
    REQUIRE(messageResult == Kmplete::FileDialogs::MessageButton::Yes);
}
//--------------------------------------------------------------------------
