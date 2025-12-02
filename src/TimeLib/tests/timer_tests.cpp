#include "Kmplete/Time/timer.h"
#include "Kmplete/Base/pointers.h"

#include <catch2/catch_test_macros.hpp>

#include <thread>


static constexpr auto timerTimeMs = 200;


TEST_CASE("Timer creation nothrow", "[time][timer]")
{
    Kmplete::Ptr<Kmplete::Time::Timer> timer = nullptr;
    REQUIRE_NOTHROW(timer = Kmplete::CreatePtr<Kmplete::Time::Timer>(timerTimeMs));
}
//--------------------------------------------------------------------------


TEST_CASE("Timer ReachedTimeout is correct", "[time][timer]")
{
    Kmplete::Ptr<Kmplete::Time::Timer> timer = nullptr;
    REQUIRE_NOTHROW(timer = Kmplete::CreatePtr<Kmplete::Time::Timer>(timerTimeMs));

    REQUIRE_FALSE(timer->ReachedTimeout());
    std::this_thread::sleep_for(std::chrono::milliseconds(timerTimeMs));
    REQUIRE(timer->ReachedTimeout());

    Kmplete::Ptr<Kmplete::Time::Timer> timer0 = nullptr;
    REQUIRE_NOTHROW(timer0 = Kmplete::CreatePtr<Kmplete::Time::Timer>());
    REQUIRE(timer0->ReachedTimeout());
}
//--------------------------------------------------------------------------


TEST_CASE("Timer GetTimeout is correct", "[time][timer]")
{
    Kmplete::Ptr<Kmplete::Time::Timer> timer = nullptr;
    REQUIRE_NOTHROW(timer = Kmplete::CreatePtr<Kmplete::Time::Timer>(timerTimeMs));
    REQUIRE(timer->GetTimeout() == timerTimeMs);
}
//--------------------------------------------------------------------------


TEST_CASE("Timer SetTimeout is correct", "[time][timer]")
{
    Kmplete::Ptr<Kmplete::Time::Timer> timer = nullptr;
    REQUIRE_NOTHROW(timer = Kmplete::CreatePtr<Kmplete::Time::Timer>(timerTimeMs));

    std::this_thread::sleep_for(std::chrono::milliseconds(timerTimeMs / 2));
    REQUIRE_FALSE(timer->ReachedTimeout());

    timer->SetTimeout(timerTimeMs * 2);
    std::this_thread::sleep_for(std::chrono::milliseconds(timerTimeMs));
    REQUIRE_FALSE(timer->ReachedTimeout());

    timer->SetTimeout(timerTimeMs / 2);
    std::this_thread::sleep_for(std::chrono::milliseconds(timerTimeMs));
    REQUIRE(timer->ReachedTimeout());
}
//--------------------------------------------------------------------------


TEST_CASE("Timer Peek is correct", "[time][timer]")
{
    Kmplete::Ptr<Kmplete::Time::Timer> timer = nullptr;
    REQUIRE_NOTHROW(timer = Kmplete::CreatePtr<Kmplete::Time::Timer>(timerTimeMs));

    REQUIRE((timer->Peek() < timerTimeMs && timer->Peek() > 0));

    std::this_thread::sleep_for(std::chrono::milliseconds(timerTimeMs / 2));
    REQUIRE((timer->Peek() < timerTimeMs && timer->Peek() > timerTimeMs / 2));

    std::this_thread::sleep_for(std::chrono::milliseconds(timerTimeMs / 2));
    REQUIRE(timer->Peek() > timerTimeMs);
}
//--------------------------------------------------------------------------


TEST_CASE("Timer Mark is correct", "[time][timer]")
{
    Kmplete::Ptr<Kmplete::Time::Timer> timer = nullptr;
    REQUIRE_NOTHROW(timer = Kmplete::CreatePtr<Kmplete::Time::Timer>(timerTimeMs));

    std::this_thread::sleep_for(std::chrono::milliseconds(timerTimeMs * 2));
    auto elapsed = 0.0f;
    REQUIRE_NOTHROW(elapsed = timer->Mark());
    REQUIRE(elapsed >= timerTimeMs * 2);
    REQUIRE_FALSE(timer->ReachedTimeout());

    std::this_thread::sleep_for(std::chrono::milliseconds(timerTimeMs));
    REQUIRE(timer->ReachedTimeout());
}
//--------------------------------------------------------------------------