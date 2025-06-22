#include "Kmplete/Core/assertion.h"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("Assertion exception", "[core][assertion]")
{
#if !defined (KMP_PRODUCTION_BUILD)
    const auto var = 10;
#if !KMP_ASSERTS_ACTIVE
    REQUIRE_NOTHROW(KMP_ASSERT(var != 10));
#endif

    REQUIRE_NOTHROW(KMP_CHECK(var == 10));
#else
    SUCCEED();
#endif
}
//--------------------------------------------------------------------------

TEST_CASE("Assertion nothrows", "[core][assertion]")
{
#if !defined (KMP_PRODUCTION_BUILD)
    const auto var = 10;
    REQUIRE_NOTHROW(KMP_ASSERT(var == 10));
#else
    SUCCEED();
#endif
}
//--------------------------------------------------------------------------

TEST_CASE("Assertion chain watch", "[core][assertion]")
{
#if !defined (KMP_PRODUCTION_BUILD)
    const auto var = 10;
    const auto var2 = 88;
    REQUIRE_NOTHROW(KMP_ASSERT_WATCH(var == 10 && var2 == 88, var, var2));
    REQUIRE_NOTHROW(KMP_CHECK_WATCH(var == 10 && var2 == 88, var, var2));
#else
    SUCCEED();
#endif
}
//--------------------------------------------------------------------------