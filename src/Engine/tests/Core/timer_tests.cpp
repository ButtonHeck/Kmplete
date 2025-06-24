#include "Kmplete/Core/timer.h"
#include "Kmplete/Base/pointers.h"

#include <catch2/catch_test_macros.hpp>

#include <thread>

static constexpr auto timerTimeMs = 200;

TEST_CASE("Timer creation nothrow", "[core][timer]")
{
    Kmplete::Ptr<Kmplete::Timer> timer = nullptr;
    REQUIRE_NOTHROW(timer = Kmplete::CreatePtr<Kmplete::Timer>(timerTimeMs));
}
//--------------------------------------------------------------------------

TEST_CASE("Timer ReachedTimeout is correct", "[core][timer]")
{
    Kmplete::Ptr<Kmplete::Timer> timer = nullptr;
    REQUIRE_NOTHROW(timer = Kmplete::CreatePtr<Kmplete::Timer>(timerTimeMs));

    REQUIRE_FALSE(timer->ReachedTimeout());
    std::this_thread::sleep_for(std::chrono::milliseconds(timerTimeMs));
    REQUIRE(timer->ReachedTimeout());

    Kmplete::Ptr<Kmplete::Timer> timer0 = nullptr;
    REQUIRE_NOTHROW(timer0 = Kmplete::CreatePtr<Kmplete::Timer>());
    REQUIRE(timer0->ReachedTimeout());
}
//--------------------------------------------------------------------------

TEST_CASE("Timer GetTimeout is correct", "[core][timer]")
{
    Kmplete::Ptr<Kmplete::Timer> timer = nullptr;
    REQUIRE_NOTHROW(timer = Kmplete::CreatePtr<Kmplete::Timer>(timerTimeMs));
    REQUIRE(timer->GetTimeout() == timerTimeMs);
}
//--------------------------------------------------------------------------

TEST_CASE("Timer SetTimeout is correct", "[core][timer]")
{
    Kmplete::Ptr<Kmplete::Timer> timer = nullptr;
    REQUIRE_NOTHROW(timer = Kmplete::CreatePtr<Kmplete::Timer>(timerTimeMs));

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

TEST_CASE("Timer Peek is correct", "[core][timer]")
{
    Kmplete::Ptr<Kmplete::Timer> timer = nullptr;
    REQUIRE_NOTHROW(timer = Kmplete::CreatePtr<Kmplete::Timer>(timerTimeMs));

    REQUIRE((timer->Peek() < timerTimeMs && timer->Peek() > 0));

    std::this_thread::sleep_for(std::chrono::milliseconds(timerTimeMs / 2));
    REQUIRE((timer->Peek() < timerTimeMs && timer->Peek() > timerTimeMs / 2));

    std::this_thread::sleep_for(std::chrono::milliseconds(timerTimeMs / 2));
    REQUIRE(timer->Peek() > timerTimeMs);
}
//--------------------------------------------------------------------------

TEST_CASE("Timer Mark is correct", "[core][timer]")
{
    Kmplete::Ptr<Kmplete::Timer> timer = nullptr;
    REQUIRE_NOTHROW(timer = Kmplete::CreatePtr<Kmplete::Timer>(timerTimeMs));

    std::this_thread::sleep_for(std::chrono::milliseconds(timerTimeMs * 2));
    auto elapsed = 0.0f;
    REQUIRE_NOTHROW(elapsed = timer->Mark());
    REQUIRE(elapsed >= timerTimeMs * 2);
    REQUIRE_FALSE(timer->ReachedTimeout());

    std::this_thread::sleep_for(std::chrono::milliseconds(timerTimeMs));
    REQUIRE(timer->ReachedTimeout());
}
//--------------------------------------------------------------------------