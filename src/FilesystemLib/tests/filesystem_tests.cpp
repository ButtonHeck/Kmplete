#include "Kmplete/Filesystem/filesystem.h"
#include "Kmplete/Core/uuid.h"
#include "Kmplete/Base/platform.h"

#include <catch2/catch_test_macros.hpp>


TEST_CASE("Filesystem Current path", "[core][filesystem]")
{
    SECTION("Current path is not empty")
    {
        const auto path = Kmplete::Filesystem::GetCurrentFilepath();
        REQUIRE(!path.empty());
    }

    SECTION("Current path exists")
    {
        const auto path = Kmplete::Filesystem::GetCurrentFilepath();
        REQUIRE(Kmplete::Filesystem::FilepathExists(path));
    }

    SECTION("Current path is directory")
    {
        const auto path = Kmplete::Filesystem::GetCurrentFilepath();
        REQUIRE(Kmplete::Filesystem::IsDirectory(path));
    }
}
//--------------------------------------------------------------------------

TEST_CASE("Filesystem string conversions", "[core][filesystem]")
{
    SECTION("Current path ToGenericU8String is not empty")
    {
        const auto path = Kmplete::Filesystem::GetCurrentFilepath();
        const auto pathStr = Kmplete::Filesystem::ToGenericU8String(path);
        REQUIRE(!pathStr.empty());
    }

    SECTION("Current path ToGenericString is not empty")
    {
        const auto path = Kmplete::Filesystem::GetCurrentFilepath();
        const auto pathStr = Kmplete::Filesystem::ToGenericString(path);
        REQUIRE(!pathStr.empty());
    }

    SECTION("Current path ToNativeU8String is not empty")
    {
        const auto path = Kmplete::Filesystem::GetCurrentFilepath();
        const auto pathStr = Kmplete::Filesystem::ToNativeU8String(path);
        REQUIRE(!pathStr.empty());
    }

    SECTION("Current path ToNativeString is not empty")
    {
        const auto path = Kmplete::Filesystem::GetCurrentFilepath();
        const auto pathStr = Kmplete::Filesystem::ToNativeString(path);
        REQUIRE(!pathStr.empty());
    }
}
//--------------------------------------------------------------------------

TEST_CASE("Filesystem directories functions", "[core][filesystem]")
{
    SECTION("CreateDirectories/RemoveDirectories test directory")
    {
        auto path = Kmplete::Filesystem::GetCurrentFilepath();
        path /= "test";
        REQUIRE(Kmplete::Filesystem::CreateDirectories(path));
        REQUIRE(Kmplete::Filesystem::RemoveDirectories(path));
    }

    SECTION("CreateDirectories test nested directories")
    {
        auto path = Kmplete::Filesystem::GetCurrentFilepath();
        path /= "test_nest_0";
        path /= "test_nest_1";
        path /= "test_nest_2";
        REQUIRE(Kmplete::Filesystem::CreateDirectories(path));
    }

    SECTION("CreateDirectories test empty directory")
    {
        REQUIRE_FALSE(Kmplete::Filesystem::CreateDirectories(""));
    }

    SECTION("CreateDirectories test invalid character in a name")
    {
        auto path = Kmplete::Filesystem::GetCurrentFilepath();
#if defined (KMP_PLATFORM_WINDOWS)
        path /= "*";
        REQUIRE_FALSE(Kmplete::Filesystem::CreateDirectories(path));
#else
        SUCCEED();
#endif
    }

    SECTION("CopyDirectories non-existing 'from' path")
    {
        const auto path = Kmplete::Filesystem::GetCurrentFilepath();
        const auto from = path / "non-existing_directory";
        const auto to = path / "existing_test_directory";
        REQUIRE(Kmplete::Filesystem::CreateDirectories(to));
        REQUIRE_FALSE(Kmplete::Filesystem::CopyDirectories(from, to));
    }

    SECTION("CopyDirectories same 'from' and 'to' paths")
    {
        const auto path = Kmplete::Filesystem::GetCurrentFilepath();
        const auto from = path / "existing_test_directory";
        REQUIRE(Kmplete::Filesystem::CreateDirectories(from));
        REQUIRE_FALSE(Kmplete::Filesystem::CopyDirectories(from, from));
    }

    SECTION("CopyDirectories invalid 'to' path")
    {
        const auto path = Kmplete::Filesystem::GetCurrentFilepath();
        const auto from = path / "existing_test_directory";
        REQUIRE(Kmplete::Filesystem::CreateDirectories(from));
        const auto to = "DD:\\con";
        REQUIRE_FALSE(Kmplete::Filesystem::CopyDirectories(from, to));
    }

    SECTION("CopyDirectories with 'none' behaviour")
    {
        const auto path = Kmplete::Filesystem::GetCurrentFilepath();
        const auto from = path / "existing_test_directory1";
        const auto fromSecondDir = from / "internal_test_dir1";
        const auto to = path / "existing_test_directory_copy1";
        const auto toSecondDir = to / "internal_test_dir1";
        REQUIRE(Kmplete::Filesystem::CreateDirectories(from));
        REQUIRE(Kmplete::Filesystem::CreateDirectories(to));
        REQUIRE(Kmplete::Filesystem::CreateDirectories(fromSecondDir));
        REQUIRE(Kmplete::Filesystem::CopyDirectories(from, to, std::filesystem::copy_options::none));
        REQUIRE_FALSE(Kmplete::Filesystem::FilepathExists(toSecondDir));
    }

    SECTION("CopyDirectories with default 'recursive' behaviour")
    {
        const auto path = Kmplete::Filesystem::GetCurrentFilepath();
        const auto from = path / "existing_test_directory2";
        const auto fromSecondDir = from / "internal_test_dir2";
        const auto to = path / "existing_test_directory_copy2";
        const auto toSecondDir = to / "internal_test_dir2";
        REQUIRE(Kmplete::Filesystem::CreateDirectories(from));
        REQUIRE(Kmplete::Filesystem::CreateDirectories(to));
        REQUIRE(Kmplete::Filesystem::CreateDirectories(fromSecondDir));
        REQUIRE(Kmplete::Filesystem::CopyDirectories(from, to));
        REQUIRE(Kmplete::Filesystem::FilepathExists(toSecondDir));
    }

    SECTION("CopyDirectories with 'from' as a filepath")
    {
        const auto path = Kmplete::Filesystem::GetCurrentFilepath();
        const auto from = path / "not_a_directory.txt";
        const auto to = path / "existing_test_directory";
        REQUIRE(Kmplete::Filesystem::CreateFile(from));
        REQUIRE(Kmplete::Filesystem::CreateDirectories(to));
        REQUIRE_FALSE(Kmplete::Filesystem::CopyDirectories(from, to));
    }
}
//--------------------------------------------------------------------------

TEST_CASE("Filesystem files functions", "[core][filesystem]")
{
    SECTION("CreateFile/RemoveFile in existing directory")
    {
        auto path = Kmplete::Filesystem::GetCurrentFilepath();
        path /= "test_file.txt";
        REQUIRE(Kmplete::Filesystem::CreateFile(path));
        REQUIRE(Kmplete::Filesystem::RemoveFile(path));
    }

    SECTION("CreateFile in non-existent directory")
    {
        auto path = Kmplete::Filesystem::GetCurrentFilepath();
        path /= "test";
        Kmplete::UUID uuid;
        path /= std::to_string(uuid);
        path /= "test_file.txt";
        REQUIRE(Kmplete::Filesystem::CreateFile(path));
    }

    SECTION("CopyFile non-existing file")
    {
        const auto path = Kmplete::Filesystem::GetCurrentFilepath();
        const auto from = path / "non-existing.file";
        const auto to = path / "non-existing_copy.file";
        REQUIRE_FALSE(Kmplete::Filesystem::CopyFile(from, to));
    }

    SECTION("CopyFile with invalid 'to' path")
    {
        const auto path = Kmplete::Filesystem::GetCurrentFilepath();
        const auto from = path / "test_file_to_copy1.txt";
        const auto to = "DD:\\con\\blah.txt";
        REQUIRE(Kmplete::Filesystem::CreateFile(from));
        REQUIRE_FALSE(Kmplete::Filesystem::CopyFile(from, to));
    }

    SECTION("CopyFile with same 'from' and 'to'")
    {
        auto path = Kmplete::Filesystem::GetCurrentFilepath();
        path /= "test_file_to_copy.txt";
        REQUIRE(Kmplete::Filesystem::CreateFile(path));
        REQUIRE_FALSE(Kmplete::Filesystem::CopyFile(path, path));
    }

    SECTION("CopyFile with default skip-existing behaviour")
    {
        const auto path = Kmplete::Filesystem::GetCurrentFilepath();
        const auto from = path / "test_file_to_copy1.txt";
        const auto to = path / "test_file_to_copy2.txt";
        REQUIRE(Kmplete::Filesystem::CreateFile(from));
        REQUIRE(Kmplete::Filesystem::CreateFile(to));
        REQUIRE_FALSE(Kmplete::Filesystem::CopyFile(from, to));
    }

    SECTION("CopyFile with overwrite behaviour")
    {
        const auto path = Kmplete::Filesystem::GetCurrentFilepath();
        const auto from = path / "test_file_to_copy1.txt";
        const auto to = path / "test_file_to_copy2.txt";
        REQUIRE(Kmplete::Filesystem::CreateFile(from));
        REQUIRE(Kmplete::Filesystem::CreateFile(to));
        REQUIRE(Kmplete::Filesystem::CopyFile(from, to, std::filesystem::copy_options::overwrite_existing));
    }

    SECTION("CopyFile with 'none' behaviour")
    {
        const auto path = Kmplete::Filesystem::GetCurrentFilepath();
        const auto from = path / "test_file_to_copy1.txt";
        const auto to = path / "test_file_to_copy2.txt";
        REQUIRE(Kmplete::Filesystem::CreateFile(from));
        REQUIRE(Kmplete::Filesystem::CreateFile(to));
        REQUIRE_FALSE(Kmplete::Filesystem::CopyFile(from, to, std::filesystem::copy_options::none));
    }
}
//--------------------------------------------------------------------------