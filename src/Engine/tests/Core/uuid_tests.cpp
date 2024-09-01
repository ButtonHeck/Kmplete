#include "Kmplete/Core/uuid.h"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("UUID default constructor is not invalid", "[core][uuid]")
{
    REQUIRE(Kmplete::UUID() != Kmplete::UUID::InvalidUuid);
}

TEST_CASE("UUID invalid is 0", "[core][uuid]")
{
    REQUIRE(Kmplete::UUID::InvalidUuid == Kmplete::UUID(0));
}

TEST_CASE("UUID is not repeatable", "[core][uuid]")
{
    REQUIRE(Kmplete::UUID() != Kmplete::UUID());
}
//--------------------------------------------------------------------------