#include "Kmplete/Filesystem/filesystem.h"
#include "Kmplete/Utils/string_utils.h"
#include "Kmplete/Core/uuid.h"
#include "Kmplete/Base/platform.h"

#include <catch2/catch_test_macros.hpp>

#include <fstream>


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
        const auto to = "\0DD:\\con";
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

    SECTION("CreateFile/RemoveFile with cyrillic characters")
    {
        auto path = Kmplete::Filesystem::GetCurrentFilepath();
        const auto cyrillic = Kmplete::Utils::NarrowToFilepath("Привет_мир");
        REQUIRE_NOTHROW(path.append(cyrillic));
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
        const auto to = "\0DD:\\con\\blah.txt";
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


TEST_CASE("Filesystem renaming tests", "[core][filesystem]")
{
    SECTION("Rename valid case no overwrite")
    {
        const auto path = Kmplete::Filesystem::GetCurrentFilepath();
        const auto from = path / "test_file_to_rename.txt";
        const auto to = path / "test_file_to_rename2.txt";
        REQUIRE_NOTHROW(Kmplete::Filesystem::CreateFile(from));
        REQUIRE_NOTHROW(Kmplete::Filesystem::RemoveFile(to));
        bool renamed = false;
        REQUIRE_NOTHROW(renamed = Kmplete::Filesystem::Rename(from, to, false));
        REQUIRE(renamed);
        REQUIRE_NOTHROW(Kmplete::Filesystem::RemoveFile(from));
    }

    SECTION("Rename invalid source valid destination")
    {
        const auto path = Kmplete::Filesystem::GetCurrentFilepath();
        const auto from = path / "test_file_to_rename.txt";
        const auto to = path / "test_file_to_rename2.txt";
        REQUIRE_NOTHROW(Kmplete::Filesystem::RemoveFile(from));
        REQUIRE_NOTHROW(Kmplete::Filesystem::RemoveFile(to));
        bool renamed = false;
        REQUIRE_NOTHROW(renamed = Kmplete::Filesystem::Rename(from, to, false));
        REQUIRE_FALSE(renamed);
    }

    SECTION("Rename valid source invalid destination")
    {
        const auto path = Kmplete::Filesystem::GetCurrentFilepath();
        const auto from = path / "test_file_to_rename.txt";
        const auto to = path / "::invalid.txt";
        REQUIRE_NOTHROW(Kmplete::Filesystem::CreateFile(from));
        REQUIRE_NOTHROW(Kmplete::Filesystem::RemoveFile(to));
        bool renamed = false;
        REQUIRE_NOTHROW(renamed = Kmplete::Filesystem::Rename(from, to, false));
        REQUIRE_FALSE(renamed);
        REQUIRE_NOTHROW(Kmplete::Filesystem::RemoveFile(from));
    }

    SECTION("Rename valid source existing destination no overwrite")
    {
        const auto path = Kmplete::Filesystem::GetCurrentFilepath();
        const auto from = path / "test_file_to_rename.txt";
        const auto to = path / "test_file_to_rename2.txt";
        REQUIRE_NOTHROW(Kmplete::Filesystem::CreateFile(from));
        REQUIRE_NOTHROW(Kmplete::Filesystem::CreateFile(to));
        bool renamed = false;
        REQUIRE_NOTHROW(renamed = Kmplete::Filesystem::Rename(from, to, false));
        REQUIRE_FALSE(renamed);
        REQUIRE_NOTHROW(Kmplete::Filesystem::RemoveFile(from));
        REQUIRE_NOTHROW(Kmplete::Filesystem::RemoveFile(to));
    }

    SECTION("Rename valid source existing destination overwrite")
    {
        const auto path = Kmplete::Filesystem::GetCurrentFilepath();
        const auto from = path / "test_file_to_rename.txt";
        const auto to = path / "test_file_to_rename2.txt";
        REQUIRE_NOTHROW(Kmplete::Filesystem::CreateFile(from));
        REQUIRE_NOTHROW(Kmplete::Filesystem::CreateFile(to));
        bool renamed = false;
        REQUIRE_NOTHROW(renamed = Kmplete::Filesystem::Rename(from, to, true));
        REQUIRE(renamed);
        REQUIRE_NOTHROW(Kmplete::Filesystem::RemoveFile(from));
        REQUIRE_NOTHROW(Kmplete::Filesystem::RemoveFile(to));
    }

    SECTION("Rename source directory to destination file")
    {
        const auto path = Kmplete::Filesystem::GetCurrentFilepath();
        const auto from = path;
        const auto to = path / "test_file_to_rename2.txt";
        REQUIRE_NOTHROW(Kmplete::Filesystem::RemoveFile(to));
        bool renamed = false;
        REQUIRE_NOTHROW(renamed = Kmplete::Filesystem::Rename(from, to, true));
        REQUIRE_FALSE(renamed);
        REQUIRE_NOTHROW(Kmplete::Filesystem::RemoveFile(to));
    }

    SECTION("Rename source file to destination directory")
    {
        const auto path = Kmplete::Filesystem::GetCurrentFilepath();
        const auto from = path / "test_file_to_rename.txt";
        const auto to = path;
        REQUIRE_NOTHROW(Kmplete::Filesystem::RemoveFile(from));
        bool renamed = false;
        REQUIRE_NOTHROW(renamed = Kmplete::Filesystem::Rename(from, to, true));
        REQUIRE_FALSE(renamed);
        REQUIRE_NOTHROW(Kmplete::Filesystem::RemoveFile(from));
    }

    SECTION("Rename existing directory to non-existing directory")
    {
        const auto path = Kmplete::Filesystem::GetCurrentFilepath();
        const auto from = path / "renaming_dir";
        const auto to = path / "renaming_dir_2";
        REQUIRE_NOTHROW(Kmplete::Filesystem::CreateDirectories(from));
        REQUIRE_NOTHROW(Kmplete::Filesystem::RemoveDirectories(to));
        bool renamed = false;
        REQUIRE_NOTHROW(renamed = Kmplete::Filesystem::Rename(from, to));
        REQUIRE(renamed);
        REQUIRE_NOTHROW(Kmplete::Filesystem::RemoveDirectories(from));
    }

    SECTION("Rename existing directory to existing directory no overwrite")
    {
        const auto path = Kmplete::Filesystem::GetCurrentFilepath();
        const auto from = path / "renaming_dir";
        const auto to = path / "renaming_dir_2";
        REQUIRE_NOTHROW(Kmplete::Filesystem::CreateDirectories(from));
        REQUIRE_NOTHROW(Kmplete::Filesystem::CreateDirectories(to));
        bool renamed = false;
        REQUIRE_NOTHROW(renamed = Kmplete::Filesystem::Rename(from, to, false));
        REQUIRE_FALSE(renamed);
        REQUIRE_NOTHROW(Kmplete::Filesystem::RemoveDirectories(from));
        REQUIRE_NOTHROW(Kmplete::Filesystem::RemoveDirectories(to));
    }

    SECTION("Rename existing directory to existing directory overwrite")
    {
        const auto path = Kmplete::Filesystem::GetCurrentFilepath();
        const auto from = path / "renaming_dir";
        const auto to = path / "renaming_dir_2";
        REQUIRE_NOTHROW(Kmplete::Filesystem::CreateDirectories(from));
        REQUIRE_NOTHROW(Kmplete::Filesystem::CreateDirectories(to));
        bool renamed = false;
        REQUIRE_NOTHROW(renamed = Kmplete::Filesystem::Rename(from, to, true));
        REQUIRE(renamed);
        REQUIRE_NOTHROW(Kmplete::Filesystem::RemoveDirectories(from));
        REQUIRE_NOTHROW(Kmplete::Filesystem::RemoveDirectories(to));
    }
}
//--------------------------------------------------------------------------


TEST_CASE("Filesystem read text files functions", "[core][filesystem]")
{
    SECTION("ReadFileAsText non-existing file")
    {
        const auto path = Kmplete::Filesystem::GetCurrentFilepath() / "this_file_do_not_exist.blah";
        REQUIRE_FALSE(Kmplete::Filesystem::FilepathExists(path));

        Kmplete::String content;
        REQUIRE_NOTHROW(content = Kmplete::Filesystem::ReadFileAsText(path));
        REQUIRE(content.empty());
    }

    SECTION("ReadFileAsText empty file")
    {
        const auto path = Kmplete::Filesystem::GetCurrentFilepath() / "this_file_is_empty.txt";
        REQUIRE(Kmplete::Filesystem::CreateFile(path));

        Kmplete::String content;
        REQUIRE_NOTHROW(content = Kmplete::Filesystem::ReadFileAsText(path));
        REQUIRE(content.empty());
    }

    SECTION("ReadFileAsText regular text file")
    {
        const auto path = Kmplete::Filesystem::GetCurrentFilepath() / "this_file_contains_text.txt";
        REQUIRE(Kmplete::Filesystem::CreateFile(path));

        const auto contentToWrite = Kmplete::String("13 characters");
        std::ofstream outputFile(path);
        if (outputFile.is_open())
        {
            outputFile << contentToWrite;
            outputFile.close();

            Kmplete::String contentToRead;
            REQUIRE_NOTHROW(contentToRead = Kmplete::Filesystem::ReadFileAsText(path));
            REQUIRE_FALSE(contentToRead.empty());
            REQUIRE(contentToRead == "13 characters");
        }
        else
        {
            FAIL();
        }
    }
}
//--------------------------------------------------------------------------


TEST_CASE("Filesystem read binary files functions", "[core][filesystem]")
{
    SECTION("ReadFileAsBinary non-existing file")
    {
        const auto path = Kmplete::Filesystem::GetCurrentFilepath() / "this_file_do_not_exist.bin";
        REQUIRE_FALSE(Kmplete::Filesystem::FilepathExists(path));

        Kmplete::Vector<Kmplete::UByte> content;
        REQUIRE_NOTHROW(content = Kmplete::Filesystem::ReadFileAsBinary(path));
        REQUIRE(content.empty());
    }

    SECTION("ReadFileAsBinary empty file")
    {
        const auto path = Kmplete::Filesystem::GetCurrentFilepath() / "this_file_is_empty.bin";
        REQUIRE(Kmplete::Filesystem::CreateFile(path));

        Kmplete::Vector<Kmplete::UByte> content;
        REQUIRE_NOTHROW(content = Kmplete::Filesystem::ReadFileAsBinary(path));
        REQUIRE(content.empty());
    }

    SECTION("ReadFileAsBinary regular binary file")
    {
        const auto path = Kmplete::Filesystem::GetCurrentFilepath() / "this_file_contains_binary.bin";
        REQUIRE(Kmplete::Filesystem::CreateFile(path));

        Kmplete::UByte contentToWrite[] = {
            Kmplete::UByte(10),
            Kmplete::UByte(11),
            Kmplete::UByte(12),
            Kmplete::UByte(13),
            Kmplete::UByte(14)
        };

        std::ofstream outputFile(path, std::ios::binary);
        if (outputFile.is_open())
        {
            outputFile.write(reinterpret_cast<const char*>(contentToWrite), sizeof(contentToWrite));
            outputFile.close();

            Kmplete::Vector<Kmplete::UByte> contentToRead;
            REQUIRE_NOTHROW(contentToRead = Kmplete::Filesystem::ReadFileAsBinary(path));
            REQUIRE_FALSE(contentToRead.empty());
            REQUIRE(contentToRead.size() == size_t(5));
        }
        else
        {
            FAIL();
        }
    }
}
//--------------------------------------------------------------------------


TEST_CASE("Filesystem write string function", "[core][filesystem]")
{
    SECTION("WriteFile empty string")
    {
        const auto path = Kmplete::Filesystem::GetCurrentFilepath() / "WriteFile_empty.txt";
        REQUIRE_NOTHROW(Kmplete::Filesystem::RemoveFile(path));
        REQUIRE_FALSE(Kmplete::Filesystem::FilepathExists(path));

        Kmplete::String emptyString;
        REQUIRE_FALSE(Kmplete::Filesystem::WriteFile(path, emptyString, true));
    }

    SECTION("WriteFile valid string and path")
    {
        const auto path = Kmplete::Filesystem::GetCurrentFilepath() / "WriteFile_new.txt";
        REQUIRE_NOTHROW(Kmplete::Filesystem::RemoveFile(path));
        REQUIRE_FALSE(Kmplete::Filesystem::FilepathExists(path));

        Kmplete::String string = "string";
        REQUIRE(Kmplete::Filesystem::WriteFile(path, string, true));
    }

    SECTION("WriteFile invalid file valid string")
    {
        const auto path = Kmplete::Filesystem::GetCurrentFilepath() / "invalid::path.txt";
        REQUIRE_FALSE(Kmplete::Filesystem::FilepathExists(path));

        Kmplete::String string = "string";
        REQUIRE_FALSE(Kmplete::Filesystem::WriteFile(path, string, true));
    }

    SECTION("WriteFile append string")
    {
        const auto path = Kmplete::Filesystem::GetCurrentFilepath() / "WriteFile_new.txt";
        REQUIRE_NOTHROW(Kmplete::Filesystem::RemoveFile(path));
        REQUIRE_FALSE(Kmplete::Filesystem::FilepathExists(path));

        Kmplete::String string = "string";
        REQUIRE(Kmplete::Filesystem::WriteFile(path, string, true));

        Kmplete::String contentToRead;
        REQUIRE_NOTHROW(contentToRead = Kmplete::Filesystem::ReadFileAsText(path));
        REQUIRE(contentToRead == string);

        REQUIRE(Kmplete::Filesystem::WriteFile(path, string, true));
        REQUIRE_NOTHROW(contentToRead = Kmplete::Filesystem::ReadFileAsText(path));
        REQUIRE(contentToRead == "stringstring");
    }

    SECTION("WriteFile truncate string")
    {
        const auto path = Kmplete::Filesystem::GetCurrentFilepath() / "WriteFile_new.txt";
        REQUIRE_NOTHROW(Kmplete::Filesystem::RemoveFile(path));
        REQUIRE_FALSE(Kmplete::Filesystem::FilepathExists(path));

        Kmplete::String string = "string";
        REQUIRE(Kmplete::Filesystem::WriteFile(path, string, false));

        Kmplete::String contentToRead;
        REQUIRE_NOTHROW(contentToRead = Kmplete::Filesystem::ReadFileAsText(path));
        REQUIRE(contentToRead == string);

        REQUIRE(Kmplete::Filesystem::WriteFile(path, string, false));
        REQUIRE_NOTHROW(contentToRead = Kmplete::Filesystem::ReadFileAsText(path));
        REQUIRE(contentToRead == string);
    }
}
//--------------------------------------------------------------------------


TEST_CASE("Filesystem write binary buffer function", "[core][filesystem]")
{
    SECTION("WriteFile empty binary buffer")
    {
        const auto path = Kmplete::Filesystem::GetCurrentFilepath() / "WriteFile_empty.txt";
        REQUIRE_NOTHROW(Kmplete::Filesystem::RemoveFile(path));
        REQUIRE_FALSE(Kmplete::Filesystem::FilepathExists(path));

        Kmplete::BinaryBuffer emptyBuffer;
        REQUIRE_FALSE(Kmplete::Filesystem::WriteFile(path, emptyBuffer, true));
    }

    SECTION("WriteFile valid buffer and path")
    {
        const auto path = Kmplete::Filesystem::GetCurrentFilepath() / "WriteFile_new.txt";
        REQUIRE_NOTHROW(Kmplete::Filesystem::RemoveFile(path));
        REQUIRE_FALSE(Kmplete::Filesystem::FilepathExists(path));

        Kmplete::BinaryBuffer binaryBuffer = { 10, 11, 12, 13 };
        REQUIRE(Kmplete::Filesystem::WriteFile(path, binaryBuffer, true));
    }

    SECTION("WriteFile invalid file valid buffer")
    {
        const auto path = Kmplete::Filesystem::GetCurrentFilepath() / "invalid::path.txt";
        REQUIRE_FALSE(Kmplete::Filesystem::FilepathExists(path));

        Kmplete::BinaryBuffer binaryBuffer = { 10, 11, 12, 13 };
        REQUIRE_FALSE(Kmplete::Filesystem::WriteFile(path, binaryBuffer, true));
    }

    SECTION("WriteFile append binary buffer")
    {
        const auto path = Kmplete::Filesystem::GetCurrentFilepath() / "WriteFile_new.txt";
        REQUIRE_NOTHROW(Kmplete::Filesystem::RemoveFile(path));
        REQUIRE_FALSE(Kmplete::Filesystem::FilepathExists(path));

        Kmplete::BinaryBuffer binaryBuffer = { 10, 11, 12, 13 };
        REQUIRE(Kmplete::Filesystem::WriteFile(path, binaryBuffer, true));

        Kmplete::Vector<Kmplete::UByte> contentToRead;
        REQUIRE_NOTHROW(contentToRead = Kmplete::Filesystem::ReadFileAsBinary(path));
        REQUIRE(contentToRead.size() == 4ULL);

        REQUIRE(Kmplete::Filesystem::WriteFile(path, binaryBuffer, true));
        REQUIRE_NOTHROW(contentToRead = Kmplete::Filesystem::ReadFileAsBinary(path));
        REQUIRE(contentToRead.size() == 8ULL);
    }

    SECTION("WriteFile truncate binary buffer")
    {
        const auto path = Kmplete::Filesystem::GetCurrentFilepath() / "WriteFile_new.txt";
        REQUIRE_NOTHROW(Kmplete::Filesystem::RemoveFile(path));
        REQUIRE_FALSE(Kmplete::Filesystem::FilepathExists(path));

        Kmplete::BinaryBuffer binaryBuffer = { 10, 11, 12, 13 };
        REQUIRE(Kmplete::Filesystem::WriteFile(path, binaryBuffer, false));

        Kmplete::Vector<Kmplete::UByte> contentToRead;
        REQUIRE_NOTHROW(contentToRead = Kmplete::Filesystem::ReadFileAsBinary(path));
        REQUIRE(contentToRead.size() == 4ULL);

        REQUIRE(Kmplete::Filesystem::WriteFile(path, binaryBuffer, false));
        REQUIRE_NOTHROW(contentToRead = Kmplete::Filesystem::ReadFileAsBinary(path));
        REQUIRE(contentToRead.size() == 4ULL);
    }
}
//--------------------------------------------------------------------------


TEST_CASE("Filesystem write binary buffer 32 function", "[core][filesystem]")
{
    SECTION("WriteFile empty binary buffer 32")
    {
        const auto path = Kmplete::Filesystem::GetCurrentFilepath() / "WriteFile_empty.txt";
        REQUIRE_NOTHROW(Kmplete::Filesystem::RemoveFile(path));
        REQUIRE_FALSE(Kmplete::Filesystem::FilepathExists(path));

        Kmplete::BinaryBuffer32 emptyBuffer32;
        REQUIRE_FALSE(Kmplete::Filesystem::WriteFile(path, emptyBuffer32, true));
    }

    SECTION("WriteFile valid buffer 32 and path")
    {
        const auto path = Kmplete::Filesystem::GetCurrentFilepath() / "WriteFile_new.txt";
        REQUIRE_NOTHROW(Kmplete::Filesystem::RemoveFile(path));
        REQUIRE_FALSE(Kmplete::Filesystem::FilepathExists(path));

        Kmplete::BinaryBuffer32 binaryBuffer32 = { 10, 11, 12, 13 };
        REQUIRE(Kmplete::Filesystem::WriteFile(path, binaryBuffer32, true));

        auto binaryBuffer = Kmplete::Filesystem::ReadFileAsBinary(path);
        Kmplete::UInt32* dataPtr = reinterpret_cast<Kmplete::UInt32*>(binaryBuffer.data());
        REQUIRE(*dataPtr == 10);
        dataPtr++;
        REQUIRE(*dataPtr == 11);
        dataPtr++;
        REQUIRE(*dataPtr == 12);
        dataPtr++;
        REQUIRE(*dataPtr == 13);
    }

    SECTION("WriteFile invalid file valid buffer 32")
    {
        const auto path = Kmplete::Filesystem::GetCurrentFilepath() / "invalid::path.txt";
        REQUIRE_FALSE(Kmplete::Filesystem::FilepathExists(path));

        Kmplete::BinaryBuffer32 binaryBuffer32 = { 10, 11, 12, 13 };
        REQUIRE_FALSE(Kmplete::Filesystem::WriteFile(path, binaryBuffer32, true));
    }

    SECTION("WriteFile append binary buffer 32")
    {
        const auto path = Kmplete::Filesystem::GetCurrentFilepath() / "WriteFile_new.txt";
        REQUIRE_NOTHROW(Kmplete::Filesystem::RemoveFile(path));
        REQUIRE_FALSE(Kmplete::Filesystem::FilepathExists(path));

        Kmplete::BinaryBuffer32 binaryBuffer32 = { 10, 11, 12, 13 };
        REQUIRE(Kmplete::Filesystem::WriteFile(path, binaryBuffer32, true));

        Kmplete::Vector<Kmplete::UByte> contentToRead;
        REQUIRE_NOTHROW(contentToRead = Kmplete::Filesystem::ReadFileAsBinary(path));
        REQUIRE(contentToRead.size() == 4ULL * sizeof(Kmplete::UInt32));

        REQUIRE(Kmplete::Filesystem::WriteFile(path, binaryBuffer32, true));
        REQUIRE_NOTHROW(contentToRead = Kmplete::Filesystem::ReadFileAsBinary(path));
        REQUIRE(contentToRead.size() == 8ULL * sizeof(Kmplete::UInt32));
    }

    SECTION("WriteFile truncate binary buffer 32")
    {
        const auto path = Kmplete::Filesystem::GetCurrentFilepath() / "WriteFile_new.txt";
        REQUIRE_NOTHROW(Kmplete::Filesystem::RemoveFile(path));
        REQUIRE_FALSE(Kmplete::Filesystem::FilepathExists(path));

        Kmplete::BinaryBuffer32 binaryBuffer32 = { 10, 11, 12, 13 };
        REQUIRE(Kmplete::Filesystem::WriteFile(path, binaryBuffer32, false));

        Kmplete::Vector<Kmplete::UByte> contentToRead;
        REQUIRE_NOTHROW(contentToRead = Kmplete::Filesystem::ReadFileAsBinary(path));
        REQUIRE(contentToRead.size() == 4ULL * sizeof(Kmplete::UInt32));

        REQUIRE(Kmplete::Filesystem::WriteFile(path, binaryBuffer32, false));
        REQUIRE_NOTHROW(contentToRead = Kmplete::Filesystem::ReadFileAsBinary(path));
        REQUIRE(contentToRead.size() == 4ULL * sizeof(Kmplete::UInt32));
    }
}
//--------------------------------------------------------------------------