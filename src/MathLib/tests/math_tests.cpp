#include "Kmplete/Math/math.h"

#include <catch2/catch_test_macros.hpp>

#include <limits>


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
//--------------------------------------------------------------------------


TEST_CASE("Math Min", "[math]")
{
    using namespace Kmplete::Math;

    int minValueInt = 0;
    REQUIRE_NOTHROW(minValueInt = Min(10, 20));
    REQUIRE(minValueInt == 10);

    REQUIRE_NOTHROW(minValueInt = Min(15, 15));
    REQUIRE(minValueInt == 15);

    REQUIRE_NOTHROW(minValueInt = Min(-10, -20));
    REQUIRE(minValueInt == -20);

    REQUIRE_NOTHROW(minValueInt = Min(std::numeric_limits<int>::min(), 12));
    REQUIRE(minValueInt == std::numeric_limits<int>::min());

    float minValueFloat = 0.0f;
    REQUIRE_NOTHROW(minValueFloat = Min(10.0f, 20.0f));
    REQUIRE(minValueFloat == 10.0f);

    REQUIRE_NOTHROW(minValueFloat = Min(15.0f, 15.0f));
    REQUIRE(minValueFloat == 15.0f);

    REQUIRE_NOTHROW(minValueFloat = Min(-10.0f, -20.0f));
    REQUIRE(minValueFloat == -20.0f);

    REQUIRE_NOTHROW(minValueFloat = Min(std::numeric_limits<float>::min(), 123.0f));
    REQUIRE(minValueFloat == std::numeric_limits<float>::min());
}
//--------------------------------------------------------------------------


TEST_CASE("Math Max", "[math]")
{
    using namespace Kmplete::Math;

    int maxValueInt = 0;
    REQUIRE_NOTHROW(maxValueInt = Max(10, 20));
    REQUIRE(maxValueInt == 20);

    REQUIRE_NOTHROW(maxValueInt = Max(15, 15));
    REQUIRE(maxValueInt == 15);

    REQUIRE_NOTHROW(maxValueInt = Max(-10, -20));
    REQUIRE(maxValueInt == -10);

    float maxValueFloat = 0.0f;
    REQUIRE_NOTHROW(maxValueFloat = Max(10.0f, 20.0f));
    REQUIRE(maxValueFloat == 20.0f);

    REQUIRE_NOTHROW(maxValueFloat = Max(15.0f, 15.0f));
    REQUIRE(maxValueFloat == 15.0f);

    REQUIRE_NOTHROW(maxValueFloat = Max(-10.0f, -20.0f));
    REQUIRE(maxValueFloat == -10.0f);
}
//--------------------------------------------------------------------------


TEST_CASE("Math Log2", "[math]")
{
    using namespace Kmplete::Math;

    int resultInt = 0;
    REQUIRE_NOTHROW(resultInt = Log2(16));
    REQUIRE(resultInt == 4);

    REQUIRE_NOTHROW(resultInt = Log2(-16));
    REQUIRE(resultInt != 4);

    REQUIRE_NOTHROW(resultInt = Log2(1));
    REQUIRE(resultInt == 0);

    REQUIRE_NOTHROW(resultInt = int(Log2(-0.25f)));
    REQUIRE(resultInt < 0);

    float resultFloat = 0.0f;
    REQUIRE_NOTHROW(resultFloat = Log2(35.0f));
    REQUIRE((resultFloat > 5.0f && resultFloat < 6.0f));

    REQUIRE_NOTHROW(resultFloat = Log2(0.0002f));
    REQUIRE(resultFloat < 0.0f);
}
//--------------------------------------------------------------------------
