#include "Kmplete/Core/filesystem.h"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("Filesystem", "[core][filesystem]")
{
    REQUIRE(Kmplete::Filesystem::Initialize());

    SECTION("Application path is not empty")
    {
        const auto path = Kmplete::Filesystem::GetApplicationPath();
        REQUIRE(!path.empty());
    }

    SECTION("Application path exists")
    {
        const auto path = Kmplete::Filesystem::GetApplicationPath();
        REQUIRE(Kmplete::Filesystem::PathExists(path));
    }

    SECTION("Application path is directory")
    {
        const auto path = Kmplete::Filesystem::GetApplicationPath();
        REQUIRE(Kmplete::Filesystem::IsDirectory(path));
    }

    SECTION("Application path ToGenericU8String is not empty")
    {
        const auto path = Kmplete::Filesystem::GetApplicationPath();
        const auto pathStr = Kmplete::Filesystem::ToGenericU8String(path);
        REQUIRE(!pathStr.empty());
    }

    SECTION("Application path ToGenericString is not empty")
    {
        const auto path = Kmplete::Filesystem::GetApplicationPath();
        const auto pathStr = Kmplete::Filesystem::ToGenericString(path);
        REQUIRE(!pathStr.empty());
    }

    SECTION("Application path ToNativeU8String is not empty")
    {
        const auto path = Kmplete::Filesystem::GetApplicationPath();
        const auto pathStr = Kmplete::Filesystem::ToNativeU8String(path);
        REQUIRE(!pathStr.empty());
    }

    SECTION("Application path ToNativeString is not empty")
    {
        const auto path = Kmplete::Filesystem::GetApplicationPath();
        const auto pathStr = Kmplete::Filesystem::ToNativeString(path);
        REQUIRE(!pathStr.empty());
    }

    SECTION("CreateDirectories test directory")
    {
        auto path = Kmplete::Filesystem::GetApplicationPath();
        path /= "test";
        REQUIRE(Kmplete::Filesystem::CreateDirectories(path));
    }

    SECTION("CreateDirectories test empty directory")
    {
        REQUIRE_FALSE(Kmplete::Filesystem::CreateDirectories(""));
    }
}
//--------------------------------------------------------------------------