#include "Kmplete/Core/assertion.h"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("Assertion exception", "[core][assertion]")
{
    const auto var = 10;
#if KMP_ASSERTS_ACTIVE
    REQUIRE_THROWS(KMP_ASSERT(var != 10).Exception());
#else
    REQUIRE_NOTHROW(KMP_ASSERT(var != 10).Exception());
#endif

    REQUIRE_THROWS(KMP_CHECK(var != 10).Exception());
    REQUIRE_NOTHROW(KMP_CHECK(var == 10).Exception());
}

TEST_CASE("Assertion nothrows", "[core][assertion]")
{
    const auto var = 10;
    REQUIRE_NOTHROW(KMP_ASSERT(var == 10));
}

TEST_CASE("Assertion chain watch", "[core][assertion]")
{
    const auto var = 10;
    const auto var2 = 88;
    REQUIRE_NOTHROW((KMP_ASSERT(var == 10 && var2 == 88).KMP_ASSERT_WATCH(var).KMP_ASSERT_WATCH(var2).Exception()));
    REQUIRE_NOTHROW((KMP_ASSERT(var == 10 && var2 == 88).KMP_ASSERT_WATCH(var, var2).Exception()));
}
//--------------------------------------------------------------------------