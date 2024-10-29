#include "Kmplete/Core/file_dialogs.h"
#include "Kmplete/Core/filesystem.h"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("File dialog open single file - OPEN", "[core][file_dialogs][open]")
{
    const auto file = Kmplete::FileDialogs::OpenFile("Open any single file");
    REQUIRE(!file.empty());
    REQUIRE(Kmplete::Filesystem::FilePathIsValid(file));
    REQUIRE(Kmplete::Filesystem::PathExists(file));
}

TEST_CASE("File dialog open single file - CANCEL", "[core][file_dialogs][open]")
{
    const auto file = Kmplete::FileDialogs::OpenFile("Press cancel");
    REQUIRE(file.empty());
}

TEST_CASE("File dialog open single file - OPEN json", "[core][file_dialogs][open]")
{
    const auto file = Kmplete::FileDialogs::OpenFile("Open any single json file", { "JSON Files", "*.json" });
    REQUIRE(!file.empty());
    REQUIRE(Kmplete::Filesystem::ToGenericString(file).ends_with(".json"));
    REQUIRE(Kmplete::Filesystem::FilePathIsValid(file));
    REQUIRE(Kmplete::Filesystem::PathExists(file));
}

TEST_CASE("File dialog open single file - OPEN manual name non-existent", "[core][file_dialogs][open]")
{
    const auto file = Kmplete::FileDialogs::OpenFile("Open any manually entered non-existing file");
    REQUIRE(!file.empty());
    REQUIRE(Kmplete::Filesystem::FilePathIsValid(file));
    REQUIRE_FALSE(Kmplete::Filesystem::PathExists(file));
}

TEST_CASE("File dialog open single file - OPEN manual name existing", "[core][file_dialogs][open]")
{
    const auto file = Kmplete::FileDialogs::OpenFile("Open any manually entered existing file");
    REQUIRE(!file.empty());
    REQUIRE(Kmplete::Filesystem::FilePathIsValid(file));
    REQUIRE(Kmplete::Filesystem::PathExists(file));
}

TEST_CASE("File dialog open single file - OPEN manual name existing non-match filter", "[core][file_dialogs][open]")
{
    const auto file = Kmplete::FileDialogs::OpenFile("Open any manually entered existing file that do not match filter", { "JSON Files", "*.json" });
    REQUIRE(!file.empty());
    REQUIRE(Kmplete::Filesystem::FilePathIsValid(file));
    REQUIRE(Kmplete::Filesystem::PathExists(file));
}
//--------------------------------------------------------------------------

TEST_CASE("File dialog open multiple files - OPEN", "[core][file_dialogs][open]")
{
    const auto files = Kmplete::FileDialogs::OpenFiles("Open any multiple files");
    REQUIRE(!files.empty());
    bool ok = true;
    for (const auto& file : files)
    {
        ok &= Kmplete::Filesystem::FilePathIsValid(file);
        ok &= Kmplete::Filesystem::PathExists(file);

        if (!ok)
        {
            WARN(file);
            break;
        }
    }

    REQUIRE(ok);
}

TEST_CASE("File dialog open multiple files - CANCEL", "[core][file_dialogs][open]")
{
    const auto files = Kmplete::FileDialogs::OpenFiles("Press cancel");
    REQUIRE(files.empty());
}

TEST_CASE("File dialog open multiple files - OPEN json", "[core][file_dialogs][open]")
{
    const auto files = Kmplete::FileDialogs::OpenFiles("Open any multiple json files", { "JSON Files", "*.json" });
    REQUIRE(!files.empty());
    bool ok = true;
    for (const auto& file : files)
    {
        ok &= Kmplete::Filesystem::FilePathIsValid(file);
        ok &= Kmplete::Filesystem::PathExists(file);
        ok &= Kmplete::Filesystem::ToGenericString(file).ends_with(".json");

        if (!ok)
        {
            WARN(file);
            break;
        }
    }

    REQUIRE(ok);
}

TEST_CASE("File dialog open multiple files - OPEN manual names non-existent", "[core][file_dialogs][open]")
{
    const auto files = Kmplete::FileDialogs::OpenFiles("Open any manually entered non-existing files");
    REQUIRE(!files.empty());
    bool ok = true;
    for (const auto& file : files)
    {
        ok &= Kmplete::Filesystem::FilePathIsValid(file);
        ok &= !Kmplete::Filesystem::PathExists(file);

        if (!ok)
        {
            WARN(file);
            break;
        }
    }

    REQUIRE(ok);
}

TEST_CASE("File dialog open multiple files - OPEN manual names existent", "[core][file_dialogs][open]")
{
    const auto files = Kmplete::FileDialogs::OpenFiles("Open any manually entered existing files");
    REQUIRE(!files.empty());
    bool ok = true;
    for (const auto& file : files)
    {
        ok &= Kmplete::Filesystem::FilePathIsValid(file);
        ok &= Kmplete::Filesystem::PathExists(file);

        if (!ok)
        {
            WARN(file);
            break;
        }
    }

    REQUIRE(ok);
}
//--------------------------------------------------------------------------

TEST_CASE("File dialog save file - SAVE new", "[core][file_dialogs][save]")
{
    const auto file = Kmplete::FileDialogs::SaveFile("Save to any new file");
    REQUIRE(!file.empty());
    REQUIRE(Kmplete::Filesystem::FilePathIsValid(file));
    REQUIRE_FALSE(Kmplete::Filesystem::PathExists(file));
}

TEST_CASE("File dialog save file - CANCEL", "[core][file_dialogs][save]")
{
    const auto file = Kmplete::FileDialogs::SaveFile("Press Cancel");
    REQUIRE(file.empty());
}

TEST_CASE("File dialog save file - SAVE overwrite", "[core][file_dialogs][save]")
{
    const auto file = Kmplete::FileDialogs::SaveFile("Save to any existing file", {"Any files", "*.*"}, true);
    REQUIRE(!file.empty());
    REQUIRE(Kmplete::Filesystem::PathExists(file));
    REQUIRE(Kmplete::Filesystem::IsFile(file));
}

TEST_CASE("File dialog save file - SAVE new non-matching filter", "[core][file_dialogs][save]")
{
    const auto file = Kmplete::FileDialogs::SaveFile("Save to any new file that do not match filter", {"JSON files", "*.json"});
    REQUIRE(!file.empty());
    REQUIRE(Kmplete::Filesystem::FilePathIsValid(file));
    REQUIRE_FALSE(Kmplete::Filesystem::PathExists(file));
}

TEST_CASE("File dialog save file - Cyrillic characters", "[core][file_dialogs][open]")
{
    const auto file = Kmplete::FileDialogs::SaveFile("Save to new file with cyrillic characters");
    REQUIRE(!file.empty());
    REQUIRE(Kmplete::Filesystem::FilePathIsValid(file));
}
//--------------------------------------------------------------------------

TEST_CASE("File dialog open folder - OPEN", "[core][file_dialogs][folder]")
{
    const auto folder = Kmplete::FileDialogs::OpenDirectory("Open any directory");
    REQUIRE(!folder.empty());
    REQUIRE(Kmplete::Filesystem::IsDirectory(folder));
}

TEST_CASE("File dialog open folder - CANCEL", "[core][file_dialogs][folder]")
{
    const auto folder = Kmplete::FileDialogs::OpenDirectory("Press Cancel");
    REQUIRE(folder.empty());
}
//--------------------------------------------------------------------------

TEST_CASE("Message dialog OK button", "[core][file_dialogs][message]")
{
    auto messageResult = Kmplete::FileDialogs::OpenMessage("Message dialog test", "Press OK");
    REQUIRE(messageResult == Kmplete::FileDialogs::MessageButton::Ok);

    messageResult = Kmplete::FileDialogs::OpenMessage("Message dialog test", "Press OK", Kmplete::FileDialogs::MessageChoice::Ok);
    REQUIRE(messageResult == Kmplete::FileDialogs::MessageButton::Ok);
}

TEST_CASE("Message dialog CANCEL button", "[core][file_dialogs][message]")
{
    auto messageResult = Kmplete::FileDialogs::OpenMessage("Message dialog test", "Press CANCEL");
    REQUIRE(messageResult == Kmplete::FileDialogs::MessageButton::Cancel);

    messageResult = Kmplete::FileDialogs::OpenMessage("Message dialog test", "Press CANCEL", Kmplete::FileDialogs::MessageChoice::YesNoCancel);
    REQUIRE(messageResult == Kmplete::FileDialogs::MessageButton::Cancel);

    messageResult = Kmplete::FileDialogs::OpenMessage("Message dialog test", "Press CANCEL", Kmplete::FileDialogs::MessageChoice::RetryCancel);
    REQUIRE(messageResult == Kmplete::FileDialogs::MessageButton::Cancel);
}

TEST_CASE("Message dialog close button", "[core][file_dialogs][message]")
{
    auto messageResult = Kmplete::FileDialogs::OpenMessage("Message dialog test", "Close this dialog (Expect Cancel)");
    REQUIRE(messageResult == Kmplete::FileDialogs::MessageButton::Cancel);

    messageResult = Kmplete::FileDialogs::OpenMessage("Message dialog test", "Close this dialog (Expect OK)", Kmplete::FileDialogs::MessageChoice::Ok);
    REQUIRE(messageResult == Kmplete::FileDialogs::MessageButton::Ok);

    messageResult = Kmplete::FileDialogs::OpenMessage("Message dialog test", "Close this dialog (Expect Cancel)", Kmplete::FileDialogs::MessageChoice::RetryCancel);
    REQUIRE(messageResult == Kmplete::FileDialogs::MessageButton::Cancel);

    messageResult = Kmplete::FileDialogs::OpenMessage("Message dialog test", "Close this dialog (Expect Cancel)", Kmplete::FileDialogs::MessageChoice::YesNoCancel);
    REQUIRE(messageResult == Kmplete::FileDialogs::MessageButton::Cancel);
}

TEST_CASE("Message dialog YES button", "[core][file_dialogs][message]")
{
    auto messageResult = Kmplete::FileDialogs::OpenMessage("Message dialog test", "Press YES", Kmplete::FileDialogs::MessageChoice::YesNo);
    REQUIRE(messageResult == Kmplete::FileDialogs::MessageButton::Yes);

    messageResult = Kmplete::FileDialogs::OpenMessage("Message dialog test", "Press YES", Kmplete::FileDialogs::MessageChoice::YesNoCancel);
    REQUIRE(messageResult == Kmplete::FileDialogs::MessageButton::Yes);
}

TEST_CASE("Message dialog NO button", "[core][file_dialogs][message]")
{
    auto messageResult = Kmplete::FileDialogs::OpenMessage("Message dialog test", "Press NO", Kmplete::FileDialogs::MessageChoice::YesNo);
    REQUIRE(messageResult == Kmplete::FileDialogs::MessageButton::No);

    messageResult = Kmplete::FileDialogs::OpenMessage("Message dialog test", "Press NO", Kmplete::FileDialogs::MessageChoice::YesNoCancel);
    REQUIRE(messageResult == Kmplete::FileDialogs::MessageButton::No);
}

TEST_CASE("Message dialog RETRY button", "[core][file_dialogs][message]")
{
    auto messageResult = Kmplete::FileDialogs::OpenMessage("Message dialog test", "Press RETRY", Kmplete::FileDialogs::MessageChoice::RetryCancel);
    REQUIRE(messageResult == Kmplete::FileDialogs::MessageButton::Retry);

    messageResult = Kmplete::FileDialogs::OpenMessage("Message dialog test", "Press RETRY", Kmplete::FileDialogs::MessageChoice::AbortRetryIgnore);
    REQUIRE(messageResult == Kmplete::FileDialogs::MessageButton::Retry);
}

TEST_CASE("Message dialog ABORT button", "[core][file_dialogs][message]")
{
    const auto messageResult = Kmplete::FileDialogs::OpenMessage("Message dialog test", "Press ABORT", Kmplete::FileDialogs::MessageChoice::AbortRetryIgnore);
    REQUIRE(messageResult == Kmplete::FileDialogs::MessageButton::Abort);
}

TEST_CASE("Message dialog IGNORE button", "[core][file_dialogs][message]")
{
    const auto messageResult = Kmplete::FileDialogs::OpenMessage("Message dialog test", "Press IGNORE", Kmplete::FileDialogs::MessageChoice::AbortRetryIgnore);
    REQUIRE(messageResult == Kmplete::FileDialogs::MessageButton::Ignore);
}
//--------------------------------------------------------------------------

TEST_CASE("Message dialog icon INFO", "[core][file_dialogs][message]")
{
    const auto messageResult = Kmplete::FileDialogs::OpenMessage("Message dialog test", "Press YES if this dialog looks like information dialog", 
        Kmplete::FileDialogs::MessageChoice::YesNo, Kmplete::FileDialogs::MessageIcon::Info);
    REQUIRE(messageResult == Kmplete::FileDialogs::MessageButton::Yes);
}

TEST_CASE("Message dialog icon WARNING", "[core][file_dialogs][message]")
{
    const auto messageResult = Kmplete::FileDialogs::OpenMessage("Message dialog test", "Press YES if this dialog looks like warning dialog",
        Kmplete::FileDialogs::MessageChoice::YesNo, Kmplete::FileDialogs::MessageIcon::Warning);
    REQUIRE(messageResult == Kmplete::FileDialogs::MessageButton::Yes);
}

TEST_CASE("Message dialog icon ERROR", "[core][file_dialogs][message]")
{
    const auto messageResult = Kmplete::FileDialogs::OpenMessage("Message dialog test", "Press YES if this dialog looks like error dialog",
        Kmplete::FileDialogs::MessageChoice::YesNo, Kmplete::FileDialogs::MessageIcon::Error);
    REQUIRE(messageResult == Kmplete::FileDialogs::MessageButton::Yes);
}

TEST_CASE("Message dialog icon QUESTION", "[core][file_dialogs][message]")
{
    const auto messageResult = Kmplete::FileDialogs::OpenMessage("Message dialog test", "Press YES if this dialog looks like question dialog",
        Kmplete::FileDialogs::MessageChoice::YesNo, Kmplete::FileDialogs::MessageIcon::Question);
    REQUIRE(messageResult == Kmplete::FileDialogs::MessageButton::Yes);
}
//--------------------------------------------------------------------------
