#include "Kmplete/Utils/string_id.h"

#include <catch2/catch_test_macros.hpp>


TEST_CASE("StringID of nullptr", "[utils][string_id]")
{
    constexpr auto nullSid = Kmplete::Utils::ToStringID(nullptr);
    REQUIRE(nullSid == 0);
}

TEST_CASE("StringID of empty c-string", "[utils][string_id]")
{
    constexpr auto emptySid = Kmplete::Utils::ToStringID("");
    REQUIRE(emptySid == 0);
}

TEST_CASE("StringID of 0 c-string", "[utils][string_id]")
{
    constexpr auto sid = Kmplete::Utils::ToStringID("\0");
    REQUIRE(sid == 0);
}

TEST_CASE("StringID of non-empty c-string", "[utils][string_id]")
{
    constexpr auto sid = Kmplete::Utils::ToStringID("some string");
    REQUIRE(sid != 0);
}

TEST_CASE("StringID of different c-strings are different", "[utils][string_id]")
{
    constexpr auto sid1 = Kmplete::Utils::ToStringID("string1");
    constexpr auto sid2 = Kmplete::Utils::ToStringID("string2");
    REQUIRE(sid1 != 0);
    REQUIRE(sid2 != 0);
    REQUIRE(sid1 != sid2);
}

TEST_CASE("StringID of c-string via named variable", "[utils][string_id]")
{
    constexpr const char* someText = "some_text";
    constexpr auto sid = Kmplete::Utils::ToStringID(someText);
    REQUIRE(sid != 0);
}
//--------------------------------------------------------------------------

TEST_CASE("StringID of empty String is 0", "[utils][string_id]")
{
    const auto str = Kmplete::String("");
    REQUIRE(Kmplete::Utils::ToStringID(str) == 0);
}

TEST_CASE("StringID of 0 String is 0", "[utils][string_id]")
{
    const auto str = Kmplete::String("\0");
    REQUIRE(Kmplete::Utils::ToStringID(str) == 0);
}

TEST_CASE("StringID of an String is non-zero", "[utils][string_id]")
{
    const auto str = Kmplete::String("Test");
    REQUIRE(Kmplete::Utils::ToStringID(str) != 0);
}

TEST_CASE("StringID of different Strings are different", "[utils][string_id]")
{
    const auto str1 = Kmplete::String("One");
    const auto str2 = Kmplete::String("Two");
    REQUIRE(Kmplete::Utils::ToStringID(str1) != 0);
    REQUIRE(Kmplete::Utils::ToStringID(str2) != 0);
    REQUIRE(Kmplete::Utils::ToStringID(str1) != Kmplete::Utils::ToStringID(str2));
}
//--------------------------------------------------------------------------

TEST_CASE("_sid literal compilation check", "[utils][string_id]")
{
    using namespace Kmplete; // for _sid literal
    const Utils::StringID sid = 0;
    int a = 0;

    // make sure _sid is constexpr (can be used in switch cases)
    switch (sid)
    {
    case "one"_sid:
        a = 1;
        break;

    case ""_sid:
        a = 0;

    case "very long string that contains important information"_sid:
        a = 999;
    }
    
    SUCCEED();
}

TEST_CASE("_sid of different c-strings are different", "[utils][string_id]")
{
    using namespace Kmplete;
    REQUIRE("one"_sid != "two"_sid);
}
//--------------------------------------------------------------------------

TEST_CASE("_sid and function of a c-string return same results", "[utils][string_id]")
{
    using namespace Kmplete; // for _sid literal

    constexpr auto sid = Kmplete::Utils::ToStringID("test_test");
    REQUIRE(sid == "test_test"_sid);
}

TEST_CASE("_sid and function of different c-strings return different results", "[utils][string_id]")
{
    using namespace Kmplete; // for _sid literal

    constexpr auto sid = Kmplete::Utils::ToStringID("cat");
    REQUIRE(sid != "dog"_sid);
}

TEST_CASE("_sid and function of same String return same results", "[utils][string_id]")
{
    using namespace Kmplete; // for _sid literal

    const auto str1 = String("Test");
    REQUIRE(Kmplete::Utils::ToStringID(str1) == "Test"_sid);
}

TEST_CASE("_sid and function of different String return different results", "[utils][string_id]")
{
    using namespace Kmplete; // for _sid literal

    const auto str1 = String("Test");
    REQUIRE(Kmplete::Utils::ToStringID(str1) != "blah"_sid);
}
//--------------------------------------------------------------------------