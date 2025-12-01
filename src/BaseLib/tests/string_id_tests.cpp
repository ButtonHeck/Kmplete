#include "Kmplete/Base/string_id.h"

#include <catch2/catch_test_macros.hpp>


TEST_CASE("StringID of nullptr", "[core][string_id]")
{
    constexpr auto nullSid = Kmplete::ToStringID(nullptr);
    REQUIRE(nullSid == Kmplete::StringID(0));
}

TEST_CASE("StringID of empty c-string", "[core][string_id]")
{
    constexpr auto emptySid = Kmplete::ToStringID("");
    REQUIRE(emptySid == Kmplete::StringID(0));
}

TEST_CASE("StringID of 0 c-string", "[core][string_id]")
{
    constexpr auto sid = Kmplete::ToStringID("\0");
    REQUIRE(sid == Kmplete::StringID(0));
}

TEST_CASE("StringID of non-empty c-string", "[core][string_id]")
{
    constexpr auto sid = Kmplete::ToStringID("some string");
    REQUIRE(sid != Kmplete::StringID(0));
}

TEST_CASE("StringID of different c-strings are different", "[core][string_id]")
{
    constexpr auto sid1 = Kmplete::ToStringID("string1");
    constexpr auto sid2 = Kmplete::ToStringID("string2");
    REQUIRE(sid1 != Kmplete::StringID(0));
    REQUIRE(sid2 != Kmplete::StringID(0));
    REQUIRE(sid1 != sid2);
}

TEST_CASE("StringID of c-string via named variable", "[core][string_id]")
{
    constexpr const char* someText = "some_text";
    constexpr auto sid = Kmplete::ToStringID(someText);
    REQUIRE(sid != Kmplete::StringID(0));
}
//--------------------------------------------------------------------------


TEST_CASE("StringID of empty String is 0", "[core][string_id]")
{
    const auto str = Kmplete::String("");
    REQUIRE(Kmplete::ToStringID(KMP_SID_PARAM(str)) == Kmplete::StringID(0));
}

TEST_CASE("StringID of 0 String is 0", "[core][string_id]")
{
    const auto str = Kmplete::String("\0");
    REQUIRE(Kmplete::ToStringID(KMP_SID_PARAM(str)) == Kmplete::StringID(0));
}

TEST_CASE("StringID of an String is non-zero", "[core][string_id]")
{
    const auto str = Kmplete::String("Test");
    REQUIRE(Kmplete::ToStringID(KMP_SID_PARAM(str)) != Kmplete::StringID(0));
}

TEST_CASE("StringID of different Strings are different", "[core][string_id]")
{
    const auto str1 = Kmplete::String("One");
    const auto str2 = Kmplete::String("Two");
    REQUIRE(Kmplete::ToStringID(KMP_SID_PARAM(str1)) != Kmplete::StringID(0));
    REQUIRE(Kmplete::ToStringID(KMP_SID_PARAM(str2)) != Kmplete::StringID(0));
    REQUIRE(Kmplete::ToStringID(KMP_SID_PARAM(str1)) != Kmplete::ToStringID(KMP_SID_PARAM(str2)));
}
//--------------------------------------------------------------------------


TEST_CASE("_sid literal compilation check", "[core][string_id]")
{
    using namespace Kmplete; // for _sid literal
    const StringID sid = 0;
    KMP_MB_UNUSED int a = 0;

    // make sure _sid is constexpr (can be used in switch cases)
    switch (sid)
    {
    case "one"_sid:
        a = 1;
        break;

    case ""_sid:
        a = 0;
        break;

    case "very long string that contains important information"_sid:
        a = 999;
        break;
    }
    
    SUCCEED();
}

TEST_CASE("_sid of different c-strings are different", "[core][string_id]")
{
    using namespace Kmplete;
    REQUIRE("one"_sid != "two"_sid);
}
//--------------------------------------------------------------------------


TEST_CASE("_sid and function of a c-string return same results", "[core][string_id]")
{
    using namespace Kmplete; // for _sid literal

    constexpr auto sid = Kmplete::ToStringID("test_test");
    REQUIRE(sid == "test_test"_sid);
}

TEST_CASE("_sid and function of different c-strings return different results", "[core][string_id]")
{
    using namespace Kmplete; // for _sid literal

    constexpr auto sid = Kmplete::ToStringID("cat");
    REQUIRE(sid != "dog"_sid);
}

TEST_CASE("_sid and function of same String return same results", "[core][string_id]")
{
    using namespace Kmplete; // for _sid literal

    const auto str1 = String("Test");
    REQUIRE(Kmplete::ToStringID(KMP_SID_PARAM(str1)) == "Test"_sid);
}

TEST_CASE("_sid and function of different String return different results", "[core][string_id]")
{
    using namespace Kmplete; // for _sid literal

    const auto str1 = String("Test");
    REQUIRE(Kmplete::ToStringID(KMP_SID_PARAM(str1)) != "blah"_sid);
}
//--------------------------------------------------------------------------