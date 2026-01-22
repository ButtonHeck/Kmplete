#include "Kmplete/Time/clock.h"
#include "Kmplete/Base/pointers.h"

#include <catch2/catch_test_macros.hpp>

#include <thread>


static constexpr auto timerTimeMs = 200;


TEST_CASE("Clock creation nothrow", "[time][clock]")
{
    Kmplete::Ptr<Kmplete::Time::Clock> clock = nullptr;
    REQUIRE_NOTHROW(clock = Kmplete::CreatePtr<Kmplete::Time::Clock>());
}
//--------------------------------------------------------------------------


TEST_CASE("Clock Peek is correct", "[time][clock]")
{
    Kmplete::Ptr<Kmplete::Time::Clock> clock = nullptr;
    REQUIRE_NOTHROW(clock = Kmplete::CreatePtr<Kmplete::Time::Clock>());

    REQUIRE((clock->Peek() < timerTimeMs && clock->Peek() > 0));

    std::this_thread::sleep_for(std::chrono::milliseconds(timerTimeMs / 2));
    REQUIRE((clock->Peek() < timerTimeMs && clock->Peek() > timerTimeMs / 2));

    std::this_thread::sleep_for(std::chrono::milliseconds(timerTimeMs / 2));
    REQUIRE(clock->Peek() > timerTimeMs);
}
//--------------------------------------------------------------------------


TEST_CASE("Clock Mark is correct", "[time][clock]")
{
    Kmplete::Ptr<Kmplete::Time::Clock> clock = nullptr;
    REQUIRE_NOTHROW(clock = Kmplete::CreatePtr<Kmplete::Time::Clock>());

    std::this_thread::sleep_for(std::chrono::milliseconds(timerTimeMs * 2));
    auto elapsed = 0.0f;
    REQUIRE_NOTHROW(elapsed = clock->Mark());
    REQUIRE(elapsed >= timerTimeMs * 2);
}
//--------------------------------------------------------------------------