#include "Kmplete/Core/uuid.h"

#include <catch2/catch_test_macros.hpp>

#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>


TEST_CASE("UUID default constructor is not invalid", "[core][uuid]")
{
    REQUIRE(Kmplete::UUID() != Kmplete::UUID::InvalidUuid);
}
//--------------------------------------------------------------------------

TEST_CASE("UUID invalid is 0", "[core][uuid]")
{
    REQUIRE(Kmplete::UUID::InvalidUuid == Kmplete::UUID(0));
}
//--------------------------------------------------------------------------

TEST_CASE("UUID is not repeatable", "[core][uuid]")
{
    REQUIRE(Kmplete::UUID() != Kmplete::UUID());
}
//--------------------------------------------------------------------------

TEST_CASE("UUID compilation checks", "[core][uuid]")
{
    using UUIDMap = std::map<Kmplete::UUID, Kmplete::UUID>;
    using UUIDUnorderedMap = std::unordered_map<Kmplete::UUID, Kmplete::UUID>;
    using UUIDSet = std::set<Kmplete::UUID>;
    using UUIDUnorderedSet = std::unordered_set<Kmplete::UUID>;

    UUIDMap m1;
    m1.insert({{Kmplete::UUID(), Kmplete::UUID()}, {Kmplete::UUID(), Kmplete::UUID()}, {Kmplete::UUID(), Kmplete::UUID()}});
    const UUIDMap m2 = m1;
    UUIDMap m3;
    m3 = m1;

    UUIDUnorderedMap um1;
    um1.insert({{Kmplete::UUID(), Kmplete::UUID()}, {Kmplete::UUID(), Kmplete::UUID()}, {Kmplete::UUID(), Kmplete::UUID()}});
    const UUIDUnorderedMap um2 = um1;
    UUIDUnorderedMap um3;
    um3 = um1;

    UUIDSet s1;
    s1.insert({Kmplete::UUID(), Kmplete::UUID(), Kmplete::UUID()});
    const UUIDSet s2 = s1;
    UUIDSet s3;
    s3 = s1;

    UUIDUnorderedSet us1;
    us1.insert({Kmplete::UUID(), Kmplete::UUID(), Kmplete::UUID()});
    const UUIDUnorderedSet us2 = us1;
    UUIDUnorderedSet us3;
    us3 = us1;

    SUCCEED();
}
//--------------------------------------------------------------------------