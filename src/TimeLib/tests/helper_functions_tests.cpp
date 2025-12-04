#include "Kmplete/Time/helper_functions.h"
#include "Kmplete/Base/pointers.h"
#include "Kmplete/Base/types_aliases.h"

#include <catch2/catch_test_macros.hpp>


TEST_CASE("GetCurrentTimeString default format", "[time][clock]")
{
    Kmplete::String result;
    REQUIRE_NOTHROW(result = Kmplete::Time::GetCurrentTimeString());
    REQUIRE(!result.empty());
}

TEST_CASE("GetCurrentTimeString nullptr format", "[time][clock]")
{
    Kmplete::String result;
    REQUIRE_NOTHROW(result = Kmplete::Time::GetCurrentTimeString(nullptr));
    REQUIRE(result.empty());
}

TEST_CASE("GetCurrentTimeString empty format", "[time][clock]")
{
    Kmplete::String result;
    REQUIRE_NOTHROW(result = Kmplete::Time::GetCurrentTimeString(""));
    REQUIRE(result.empty());
}

TEST_CASE("GetCurrentTimeString invalid format", "[time][clock]")
{
    Kmplete::String result;
    REQUIRE_NOTHROW(result = Kmplete::Time::GetCurrentTimeString("garbage"));
    REQUIRE(!result.empty());
    REQUIRE(result == "garbage");
}
//--------------------------------------------------------------------------