#include "Kmplete/Core/filesystem.h"
#include "Kmplete/Core/uuid.h"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("Filesystem initialization", "[core][filesystem]")
{
    const auto path = Kmplete::Filesystem::GetCurrentPath();
    REQUIRE(!path.empty());
}
//--------------------------------------------------------------------------

TEST_CASE("Filesystem", "[core][filesystem]")
{
    SECTION("Application path is not empty")
    {
        const auto path = Kmplete::Filesystem::GetCurrentPath();
        REQUIRE(!path.empty());
    }

    SECTION("Application path exists")
    {
        const auto path = Kmplete::Filesystem::GetCurrentPath();
        REQUIRE(Kmplete::Filesystem::PathExists(path));
    }

    SECTION("Application path is directory")
    {
        const auto path = Kmplete::Filesystem::GetCurrentPath();
        REQUIRE(Kmplete::Filesystem::IsDirectory(path));
    }

    SECTION("Application path ToGenericU8String is not empty")
    {
        const auto path = Kmplete::Filesystem::GetCurrentPath();
        const auto pathStr = Kmplete::Filesystem::ToGenericU8String(path);
        REQUIRE(!pathStr.empty());
    }

    SECTION("Application path ToGenericString is not empty")
    {
        const auto path = Kmplete::Filesystem::GetCurrentPath();
        const auto pathStr = Kmplete::Filesystem::ToGenericString(path);
        REQUIRE(!pathStr.empty());
    }

    SECTION("Application path ToNativeU8String is not empty")
    {
        const auto path = Kmplete::Filesystem::GetCurrentPath();
        const auto pathStr = Kmplete::Filesystem::ToNativeU8String(path);
        REQUIRE(!pathStr.empty());
    }

    SECTION("Application path ToNativeString is not empty")
    {
        const auto path = Kmplete::Filesystem::GetCurrentPath();
        const auto pathStr = Kmplete::Filesystem::ToNativeString(path);
        REQUIRE(!pathStr.empty());
    }

    SECTION("CreateDirectories test directory")
    {
        auto path = Kmplete::Filesystem::GetCurrentPath();
        path /= "test";
        REQUIRE(Kmplete::Filesystem::CreateDirectories(path));
    }

    SECTION("CreateDirectories test nested directories")
    {
        auto path = Kmplete::Filesystem::GetCurrentPath();
        path /= "test_nest_0";
        path /= "test_nest_1";
        path /= "test_nest_2";
        REQUIRE(Kmplete::Filesystem::CreateDirectories(path));
    }

    SECTION("CreateDirectories test empty directory")
    {
        REQUIRE_FALSE(Kmplete::Filesystem::CreateDirectories(""));
    }

    SECTION("CreateFile in existing directory")
    {
        auto path = Kmplete::Filesystem::GetCurrentPath();
        path /= "test_file.txt";
        REQUIRE(Kmplete::Filesystem::CreateFile(path));
    }

    SECTION("CreateFile in non-existent directory")
    {
        auto path = Kmplete::Filesystem::GetCurrentPath();
        path /= "test";
        Kmplete::UUID uuid;
        path /= std::to_string(uuid);
        path /= "test_file.txt";
        REQUIRE(Kmplete::Filesystem::CreateFile(path));
    }
}
//--------------------------------------------------------------------------