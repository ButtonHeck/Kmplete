#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Utils/vector_utils.h"

#include <catch2/catch_test_macros.hpp>


using namespace Kmplete;

TEST_CASE("MergeVectors empty source", "[utils][vector]")
{
    Vector<int> src;
    Vector<int> dst{ 0, 1, 2, 3 };

    Utils::MergeVectors(src, dst);
    REQUIRE(dst.size() == 4ULL);
    REQUIRE(src.empty());
}

TEST_CASE("MergeVectors empty source empty destination", "[utils][vector]")
{
    Vector<int> src;
    Vector<int> dst;

    Utils::MergeVectors(src, dst);
    REQUIRE(dst.empty());
    REQUIRE(src.empty());
}

TEST_CASE("MergeVectors empty destination", "[utils][vector]")
{
    Vector<int> src{ 0, 1, 2, 3 };
    Vector<int> dst;

    Utils::MergeVectors(src, dst);
    REQUIRE(dst.size() == 4ULL);
    REQUIRE(src.empty());
}

TEST_CASE("MergeVectors default", "[utils][vector]")
{
    Vector<int> src{ 10, 11, 12, 13 };
    Vector<int> dst{ 0, 1, 2, 3 };

    Utils::MergeVectors(src, dst);
    REQUIRE(dst.size() == 8ULL);
    REQUIRE(dst[4] == 10);
    REQUIRE(dst[5] == 11);
    REQUIRE(dst[6] == 12);
    REQUIRE(dst[7] == 13);
    REQUIRE(src.empty());
}

TEST_CASE("MergeVectors ping-pong", "[utils][vector]")
{
    Vector<int> src{ 10, 11, 12, 13 };
    Vector<int> dst{ 0, 1, 2, 3 };

    Utils::MergeVectors(src, dst);
    REQUIRE(dst.size() == 8ULL);
    REQUIRE(dst[4] == 10);
    REQUIRE(dst[5] == 11);
    REQUIRE(dst[6] == 12);
    REQUIRE(dst[7] == 13);
    REQUIRE(src.empty());

    Utils::MergeVectors(dst, src);
    REQUIRE(src.size() == 8ULL);
    REQUIRE(src[4] == 10);
    REQUIRE(src[5] == 11);
    REQUIRE(src[6] == 12);
    REQUIRE(src[7] == 13);
    REQUIRE(dst.empty());
}
//--------------------------------------------------------------------------


TEST_CASE("AppendVectors empty source", "[utils][vector]")
{
    Vector<int> src;
    Vector<int> dst{ 0, 1, 2, 3 };

    Utils::AppendVectors(src, dst);
    REQUIRE(dst.size() == 4ULL);
    REQUIRE(src.empty());
}

TEST_CASE("AppendVectors empty source empty destination", "[utils][vector]")
{
    Vector<int> src;
    Vector<int> dst;

    Utils::AppendVectors(src, dst);
    REQUIRE(dst.empty());
    REQUIRE(src.empty());
}

TEST_CASE("AppendVectors empty destination", "[utils][vector]")
{
    Vector<int> src{ 0, 1, 2, 3 };
    Vector<int> dst;

    Utils::AppendVectors(src, dst);
    REQUIRE(dst.size() == 4ULL);
    REQUIRE(src.size() == 4ULL);
}

TEST_CASE("AppendVectors default", "[utils][vector]")
{
    Vector<int> src{ 10, 11, 12, 13 };
    Vector<int> dst{ 0, 1, 2, 3 };

    Utils::AppendVectors(src, dst);
    REQUIRE(dst.size() == 8ULL);
    REQUIRE(dst[4] == 10);
    REQUIRE(dst[5] == 11);
    REQUIRE(dst[6] == 12);
    REQUIRE(dst[7] == 13);
    REQUIRE(src.size() == 4ULL);
}

TEST_CASE("AppendVectors ping-pong", "[utils][vector]")
{
    Vector<int> src{ 10, 11, 12, 13 };
    Vector<int> dst{ 0, 1, 2, 3 };

    Utils::AppendVectors(src, dst);
    REQUIRE(dst.size() == 8ULL);
    REQUIRE(dst[4] == 10);
    REQUIRE(dst[5] == 11);
    REQUIRE(dst[6] == 12);
    REQUIRE(dst[7] == 13);
    REQUIRE(src.size() == 4ULL);

    Utils::AppendVectors(dst, src);
    REQUIRE(src.size() == 12ULL);
    REQUIRE(src[4] == 0);
    REQUIRE(src[5] == 1);
    REQUIRE(src[6] == 2);
    REQUIRE(src[7] == 3);
    REQUIRE(dst.size() == 8ULL);
}
//--------------------------------------------------------------------------


TEST_CASE("VectorContains empty vector", "[utils][vector]")
{
    Vector<int> vec{};

    auto found = false;
    REQUIRE_NOTHROW(found = Utils::VectorContains(vec, 1));
    REQUIRE_FALSE(found);
}

TEST_CASE("VectorContains not found", "[utils][vector]")
{
    Vector<int> vec{ 1, 2, 3, 4, 5 };

    auto found = false;
    REQUIRE_NOTHROW(found = Utils::VectorContains(vec, 10));
    REQUIRE_FALSE(found);
}

TEST_CASE("VectorContains found", "[utils][vector]")
{
    Vector<int> vec{ 1, 2, 3, 4, 5 };

    auto found = false;
    REQUIRE_NOTHROW(found = Utils::VectorContains(vec, 5));
    REQUIRE(found);
}
//--------------------------------------------------------------------------


TEST_CASE("VectorContainsIf empty vector", "[utils][vector]")
{
    Vector<int> vec{};

    auto found = false;
    const auto lambda = [](int value) { return value == 0; };
    REQUIRE_NOTHROW(found = Utils::VectorContainsIf(vec, lambda));
    REQUIRE_FALSE(found);
}

TEST_CASE("VectorContainsIf not found", "[utils][vector]")
{
    Vector<int> vec{ 1, 2, 3, 4, 5 };

    auto found = false;
    const auto lambda = [](int value) { return value == 0; };
    REQUIRE_NOTHROW(found = Utils::VectorContainsIf(vec, lambda));
    REQUIRE_FALSE(found);
}

TEST_CASE("VectorContainsIf found", "[utils][vector]")
{
    Vector<int> vec{ 1, 2, 3, 4, 5 };

    auto found = false;
    const auto lambda = [](int value) { return value == 5; };
    REQUIRE_NOTHROW(found = Utils::VectorContainsIf(vec, lambda));
    REQUIRE(found);
}

TEST_CASE("VectorContainsIf empty vector with true predicate", "[utils][vector]")
{
    Vector<int> vec{};

    auto found = false;
    const auto lambda = [](int) { return true; };
    REQUIRE_NOTHROW(found = Utils::VectorContainsIf(vec, lambda));
    REQUIRE_FALSE(found);
}

TEST_CASE("VectorContainsIf non-empty vector with true predicate", "[utils][vector]")
{
    Vector<int> vec{ 1, 2, 3, 4, 5 };

    auto found = false;
    const auto lambda = [](int) { return true; };
    REQUIRE_NOTHROW(found = Utils::VectorContainsIf(vec, lambda));
    REQUIRE(found);
}
//--------------------------------------------------------------------------