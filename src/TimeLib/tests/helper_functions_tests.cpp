#include "Kmplete/Time/helper_functions.h"
#include "Kmplete/Base/pointers.h"
#include "Kmplete/Base/types_aliases.h"

#include <catch2/catch_test_macros.hpp>


TEST_CASE("GetCurrentTimeString default format return non-empty", "[time][clock]")
{
    Kmplete::String result;
    REQUIRE_NOTHROW(result = Kmplete::Time::GetCurrentTimeString());
    REQUIRE(!result.empty());
}
//--------------------------------------------------------------------------