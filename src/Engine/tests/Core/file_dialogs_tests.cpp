#include "Kmplete/Core/file_dialogs.h"
#include "Kmplete/Core/filesystem.h"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("File dialog open single file (OPEN)", "[core][file_dialogs][open]")
{
    const auto file = Kmplete::FileDialogs::OpenFile("Open any single file");
    REQUIRE(!file.empty());
    REQUIRE(Kmplete::Filesystem::FilePathIsValid(file));
    REQUIRE(Kmplete::Filesystem::PathExists(file));
}

TEST_CASE("File dialog open single file (CANCEL)", "[core][file_dialogs][open]")
{
    const auto file = Kmplete::FileDialogs::OpenFile("Press cancel");
    REQUIRE(file.empty());
}

TEST_CASE("File dialog open single file (OPEN json)", "[core][file_dialogs][open]")
{
    const auto file = Kmplete::FileDialogs::OpenFile("Open any single json file", { "JSON Files", "*.json" });
    REQUIRE(!file.empty());
    REQUIRE(file.ends_with(".json"));
    REQUIRE(Kmplete::Filesystem::FilePathIsValid(file));
    REQUIRE(Kmplete::Filesystem::PathExists(file));
}

TEST_CASE("File dialog open single file (OPEN manual name non-existent)", "[core][file_dialogs][open]")
{
    const auto file = Kmplete::FileDialogs::OpenFile("Open any manually entered non-existing file");
    REQUIRE(!file.empty());
    REQUIRE(Kmplete::Filesystem::FilePathIsValid(file));
    REQUIRE_FALSE(Kmplete::Filesystem::PathExists(file));
}

TEST_CASE("File dialog open single file (OPEN manual name existing)", "[core][file_dialogs][open]")
{
    const auto file = Kmplete::FileDialogs::OpenFile("Open any manually entered existing file");
    REQUIRE(!file.empty());
    REQUIRE(Kmplete::Filesystem::FilePathIsValid(file));
    REQUIRE(Kmplete::Filesystem::PathExists(file));
}

TEST_CASE("File dialog open single file (OPEN manual name existing non-match filter)", "[core][file_dialogs][open]")
{
    const auto file = Kmplete::FileDialogs::OpenFile("Open any manually entered existing file that do not match filter", { "JSON Files", "*.json" });
    REQUIRE(!file.empty());
    REQUIRE(Kmplete::Filesystem::FilePathIsValid(file));
    REQUIRE(Kmplete::Filesystem::PathExists(file));
}
//--------------------------------------------------------------------------

TEST_CASE("File dialog open multiple files (OPEN)", "[core][file_dialogs][open]")
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

TEST_CASE("File dialog open multiple files (CANCEL)", "[core][file_dialogs][open]")
{
    const auto files = Kmplete::FileDialogs::OpenFiles("Press cancel");
    REQUIRE(files.empty());
}

TEST_CASE("File dialog open multiple files (OPEN json)", "[core][file_dialogs][open]")
{
    const auto files = Kmplete::FileDialogs::OpenFiles("Open any multiple json files", { "JSON Files", "*.json" });
    REQUIRE(!files.empty());
    bool ok = true;
    for (const auto& file : files)
    {
        ok &= Kmplete::Filesystem::FilePathIsValid(file);
        ok &= Kmplete::Filesystem::PathExists(file);
        ok &= file.ends_with(".json");

        if (!ok)
        {
            WARN(file);
            break;
        }
    }

    REQUIRE(ok);
}

TEST_CASE("File dialog open multiple files (OPEN manual names non-existent", "[core][file_dialogs][open]")
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

TEST_CASE("File dialog open multiple files (OPEN manual names existent", "[core][file_dialogs][open]")
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

TEST_CASE("File dialog save file (SAVE new)", "[core][file_dialogs][save]")
{
    const auto file = Kmplete::FileDialogs::SaveFile("Save to any new file");
    REQUIRE(!file.empty());
    REQUIRE(Kmplete::Filesystem::FilePathIsValid(file));
    REQUIRE_FALSE(Kmplete::Filesystem::PathExists(file));
}

TEST_CASE("File dialog save file (CANCEL)", "[core][file_dialogs][save]")
{
    const auto file = Kmplete::FileDialogs::SaveFile("Press Cancel");
    REQUIRE(file.empty());
}

TEST_CASE("File dialog save file (SAVE overwrite)", "[core][file_dialogs][save]")
{
    const auto file = Kmplete::FileDialogs::SaveFile("Save to any existing file", {"Any files", "*.*"}, true);
    REQUIRE(!file.empty());
    REQUIRE(Kmplete::Filesystem::PathExists(file));
    REQUIRE(Kmplete::Filesystem::IsFile(file));
}

TEST_CASE("File dialog save file (SAVE new non-matching filter", "[core][file_dialogs][save]")
{
    const auto file = Kmplete::FileDialogs::SaveFile("Save to any file that do not match filter", {"JSON files", "*.json"});
    REQUIRE(!file.empty());
    REQUIRE(Kmplete::Filesystem::FilePathIsValid(file));
    REQUIRE_FALSE(Kmplete::Filesystem::PathExists(file));
}
//--------------------------------------------------------------------------

TEST_CASE("File dialog open folder (OPEN)", "[core][file_dialogs][folder]")
{
    const auto folder = Kmplete::FileDialogs::OpenDirectory("Open any directory");
    REQUIRE(!folder.empty());
    REQUIRE(Kmplete::Filesystem::IsDirectory(folder));
}

TEST_CASE("File dialog open folder (CANCEL)", "[core][file_dialogs][folder]")
{
    const auto folder = Kmplete::FileDialogs::OpenDirectory("Press Cancel");
    REQUIRE(folder.empty());
}
//--------------------------------------------------------------------------
