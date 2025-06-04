#include "Kmplete/Core/rng.h"

#include <catch2/catch_test_macros.hpp>
#include <unordered_set>

TEST_CASE("RNG uint32_t unbounded", "[core][rng]")
{
    Kmplete::RNGUInt32 rng;

    std::unordered_set<uint32_t> set;
    const auto iterations = 1000;
    bool conditionCheck = true;
    for (auto i = 0; i < iterations; i++)
    {
        const auto value = rng.Generate();
        conditionCheck &= value >= 0;
        set.insert(value);
    }

    const auto notRepeated = set.size() == iterations;
    REQUIRE(conditionCheck);
    REQUIRE(set.find(4074212915) != set.begin());
    REQUIRE(notRepeated);
}
//--------------------------------------------------------------------------

TEST_CASE("RNG uint32_t bounded", "[core][rng]")
{
    const auto min = 5;
    const auto max = 100;
    Kmplete::RNGUInt32 rng(min, max);

    const auto iterations = 1000;
    bool conditionMinCheck = true;
    bool conditionMaxCheck = true;
    for (auto i = 0; i < iterations; i++)
    {
        const auto value = rng.Generate();
        conditionMinCheck &= value >= min;
        conditionMaxCheck &= value <= max;
    }

    REQUIRE(conditionMinCheck);
    REQUIRE(conditionMaxCheck);
}
//--------------------------------------------------------------------------

TEST_CASE("RNG float unbounded", "[core][rng]")
{
    Kmplete::RNGFloat rng;

    std::unordered_set<float> set;
    const auto iterations = 1000;
    for (auto i = 0; i < iterations; i++)
    {
        const auto value = rng.Generate();
        set.insert(value);
    }

    const auto notRepeated = set.size() == iterations;
    REQUIRE(notRepeated);
}
//--------------------------------------------------------------------------

TEST_CASE("RNG float bounded", "[core][rng]")
{
    const auto min = -100.0f;
    const auto max = 100.0f;
    Kmplete::RNGFloat rng(min, max);

    const auto iterations = 1000;
    bool conditionMinCheck = true;
    bool conditionMaxCheck = true;
    for (auto i = 0; i < iterations; i++)
    {
        const auto value = rng.Generate();
        conditionMinCheck &= value >= min;
        conditionMaxCheck &= value <= max;
    }

    REQUIRE(conditionMinCheck);
    REQUIRE(conditionMaxCheck);
}
//--------------------------------------------------------------------------