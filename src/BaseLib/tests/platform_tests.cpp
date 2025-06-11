#include "Kmplete/Base/platform.h"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("Platform compiler definition", "[core][platform][compiler]")
{
#if defined (KMP_COMPILER_OTHER)
    FAIL("Current compiler is not defined!");
#else
    SUCCEED();
#endif
}
//--------------------------------------------------------------------------

TEST_CASE("Platform OS definition", "[core][platform][OS]")
{
#if defined (KMP_PLATFORM_OTHER)
    FAIL("Current OS is not defined!");
#else
    SUCCEED();
#endif
}
//--------------------------------------------------------------------------