#include "Kmplete/Utils/memory_utils.h"

#include <catch2/catch_test_macros.hpp>


TEST_CASE("AlignedAlloc and AlignedFree int tests", "[utils][memory]")
{
    using namespace Kmplete::Utils;

    int* alignedInt = nullptr;
    const auto allocationSize = sizeof(int);

    REQUIRE_NOTHROW(alignedInt = (int*)AlignedAlloc(allocationSize, 0));
    REQUIRE(alignedInt == nullptr);
    REQUIRE_NOTHROW(AlignedFree(alignedInt));

    REQUIRE_NOTHROW(alignedInt = (int*)AlignedAlloc(allocationSize, 1));
    REQUIRE(alignedInt != nullptr);
    REQUIRE_NOTHROW(AlignedFree(alignedInt));
    REQUIRE(alignedInt != nullptr); // does not set pointer to nullptr

    uintptr_t address = 0ULL;
    for (auto i = 0; i <= 10; i++)
    {
        const auto alignment = 2 << i;
        REQUIRE_NOTHROW(alignedInt = (int*)AlignedAlloc(allocationSize, alignment));
        *alignedInt = 33;
        REQUIRE(alignedInt != nullptr);
        address = *reinterpret_cast<uintptr_t*>(&alignedInt);
        REQUIRE(address != 0ULL);
        REQUIRE(address % alignment == 0ULL);
        REQUIRE_NOTHROW(AlignedFree(alignedInt));
        REQUIRE(alignedInt != nullptr);
    }
}
//--------------------------------------------------------------------------

TEST_CASE("AlignedAlloc and AlignedFree char tests", "[utils][memory]")
{
    using namespace Kmplete::Utils;

    char* alignedChar = nullptr;
    const auto allocationSize = sizeof(char);

    REQUIRE_NOTHROW(alignedChar = (char*)AlignedAlloc(allocationSize, 0));
    REQUIRE(alignedChar == nullptr);
    REQUIRE_NOTHROW(AlignedFree(alignedChar));

    REQUIRE_NOTHROW(alignedChar = (char*)AlignedAlloc(allocationSize, 1));
    REQUIRE(alignedChar != nullptr);
    REQUIRE_NOTHROW(AlignedFree(alignedChar));
    REQUIRE(alignedChar != nullptr); // does not set pointer to nullptr

    uintptr_t address = 0ULL;
    for (auto i = 0; i <= 10; i++)
    {
        const auto alignment = 2 << i;
        REQUIRE_NOTHROW(alignedChar = (char*)AlignedAlloc(allocationSize, alignment));
        *alignedChar = 'a';
        REQUIRE(alignedChar != nullptr);
        address = *reinterpret_cast<uintptr_t*>(&alignedChar);
        REQUIRE(address != 0ULL);
        REQUIRE(address % alignment == 0ULL);
        REQUIRE_NOTHROW(AlignedFree(alignedChar));
        REQUIRE(alignedChar != nullptr);
    }
}
//--------------------------------------------------------------------------

TEST_CASE("AlignedAlloc and AlignedFree double tests", "[utils][memory]")
{
    using namespace Kmplete::Utils;

    double* alignedDouble = nullptr;
    const auto allocationSize = sizeof(double);

    REQUIRE_NOTHROW(alignedDouble = (double*)AlignedAlloc(allocationSize, 0));
    REQUIRE(alignedDouble == nullptr);
    REQUIRE_NOTHROW(AlignedFree(alignedDouble));

    REQUIRE_NOTHROW(alignedDouble = (double*)AlignedAlloc(allocationSize, 1));
    REQUIRE(alignedDouble != nullptr);
    REQUIRE_NOTHROW(AlignedFree(alignedDouble));
    REQUIRE(alignedDouble != nullptr); // does not set pointer to nullptr

    uintptr_t address = 0ULL;
    for (auto i = 0; i <= 10; i++)
    {
        const auto alignment = 2 << i;
        REQUIRE_NOTHROW(alignedDouble = (double*)AlignedAlloc(allocationSize, alignment));
        *alignedDouble = 123.4;
        REQUIRE(alignedDouble != nullptr);
        address = *reinterpret_cast<uintptr_t*>(&alignedDouble);
        REQUIRE(address != 0ULL);
        REQUIRE(address % alignment == 0ULL);
        REQUIRE_NOTHROW(AlignedFree(alignedDouble));
        REQUIRE(alignedDouble != nullptr);
    }
}
//--------------------------------------------------------------------------