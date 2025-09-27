#include "Kmplete/Math/math.h"

#include <catch2/catch_test_macros.hpp>


TEST_CASE("Math Clamp value in range", "[math]")
{
    using namespace Kmplete::Math;

    float valueFloat = 5.0f;
    REQUIRE_NOTHROW(valueFloat = Clamp(valueFloat, 0.0f, 10.0f));
    REQUIRE(valueFloat == 5.0f);

    int valueInt = 4;
    REQUIRE_NOTHROW(valueInt = Clamp(valueInt, 0, 10));
    REQUIRE(valueInt == 4);
}

TEST_CASE("Math Clamp value not in range", "[math]")
{
    using namespace Kmplete::Math;

    float valueFloat = 5.0f;
    REQUIRE_NOTHROW(valueFloat = Clamp(valueFloat, 0.0f, 1.0f));
    REQUIRE(valueFloat == 1.0f);
    REQUIRE_NOTHROW(valueFloat = Clamp(valueFloat, 2.0f, 10.0f));
    REQUIRE(valueFloat == 2.0f);

    int valueInt = 4;
    REQUIRE_NOTHROW(valueInt = Clamp(valueInt, 0, 1));
    REQUIRE(valueInt == 1);
    REQUIRE_NOTHROW(valueInt = Clamp(valueInt, 2, 10));
    REQUIRE(valueInt == 2);
}

TEST_CASE("Math Clamp range reversed", "[math]")
{
    using namespace Kmplete::Math;

    float valueFloat = 5.0f;
    REQUIRE_NOTHROW(valueFloat = Clamp(valueFloat, 10.0f, 0.0f));
    REQUIRE(valueFloat == 0.0f);

    int valueInt = 5;
    REQUIRE_NOTHROW(valueInt = Clamp(valueInt, 10, 0));
    REQUIRE(valueInt == 0);
}