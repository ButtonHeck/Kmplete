#include "Kmplete/Utils/function_utils.h"

#include <catch2/catch_test_macros.hpp>

#include <functional>


TEST_CASE("GetArgsCount 0", "[utils][function]")
{
    REQUIRE(Kmplete::Utils::GetArgsCount() == 0);
}
//--------------------------------------------------------------------------

TEST_CASE("GetArgsCount 1", "[utils][function]")
{
    const auto arg1 = 0;
    REQUIRE(Kmplete::Utils::GetArgsCount(arg1) == 1);
    REQUIRE(Kmplete::Utils::GetArgsCount(nullptr) == 1);
    REQUIRE(Kmplete::Utils::GetArgsCount((arg1 ? arg1 : 42)) == 1);
}
//--------------------------------------------------------------------------

TEST_CASE("GetArgsCount 2", "[utils][function]")
{
    const auto arg1 = 0;
    const auto arg2 = "s";
    REQUIRE(Kmplete::Utils::GetArgsCount(arg1, arg2) == 2);
    REQUIRE(Kmplete::Utils::GetArgsCount(nullptr, nullptr) == 2);
}
//--------------------------------------------------------------------------

TEST_CASE("GetArgsCount 3", "[utils][function]")
{
    const auto arg1 = 0;
    const auto arg2 = "s";
    const auto arg3 = 'c';
    REQUIRE(Kmplete::Utils::GetArgsCount(arg1, arg2, arg3) == 3);
}
//--------------------------------------------------------------------------

TEST_CASE("GetArgsCount 4", "[utils][function]")
{
    struct S {};
    const auto arg1 = 0;
    const auto arg2 = "s";
    const auto arg3 = 'c';
    const auto arg4 = S();
    REQUIRE(Kmplete::Utils::GetArgsCount(arg1, arg2, arg3, arg4) == 4);
}
//--------------------------------------------------------------------------

TEST_CASE("KMP_BIND", "[utils][function]")
{
    using CallbackIntNoParamsFn = std::function<int()>;
    using CallbackIntParamsFn = std::function<int(int)>;

    struct CallbackTest
    {
        int CallbackReturn42() { return 42; }
        int CallbackReturnParam(int param) { return param; }

        void SetCallbacks()
        {
            callbackIntNoParams = KMP_BIND(CallbackReturn42);
            callbackIntParam = KMP_BIND(CallbackReturnParam);
        }

        void SetCallbacksNonClass(const CallbackIntNoParamsFn& noParamsFn, const CallbackIntParamsFn& paramFn)
        {
            callbackIntNoParams = noParamsFn;
            callbackIntParam = paramFn;
        }

        CallbackIntNoParamsFn callbackIntNoParams;
        CallbackIntParamsFn callbackIntParam;
    };

    struct Outer
    {
        int Return142() { return 142; }
        int ReturnParamX2(int param) { return param * 2; }

        void SetCallbacks(CallbackTest& callbackTest)
        {
            callbackTest.SetCallbacksNonClass(KMP_BIND(Outer::Return142), KMP_BIND(Outer::ReturnParamX2));
        }
    };

    CallbackTest t;
    REQUIRE_THROWS(t.callbackIntNoParams());
    REQUIRE_THROWS(t.callbackIntParam(69));

    t.SetCallbacks();
    REQUIRE(t.callbackIntNoParams() == 42);
    REQUIRE(t.callbackIntParam(69) == 69);

    Outer outer;
    outer.SetCallbacks(t);
    REQUIRE(t.callbackIntNoParams() == 142);
    REQUIRE(t.callbackIntParam(69) == (69 * 2));
}
//--------------------------------------------------------------------------

static int Return555() { return 555; }
static int ReturnParamX2(int param) { return param * 2; }
TEST_CASE("KMP_BIND_FREE_FN", "[utils][function]")
{
    using CallbackIntNoParamsFn = std::function<int()>;
    using CallbackIntParamsFn = std::function<int(int)>;

    struct CallbackTest
    {
        void SetCallbacks()
        {
            callbackIntNoParams = KMP_BIND_FREE_FN(Return555);
            callbackIntParam = KMP_BIND_FREE_FN(ReturnParamX2);
        }

        CallbackIntNoParamsFn callbackIntNoParams;
        CallbackIntParamsFn callbackIntParam;
    };

    CallbackTest t;
    REQUIRE_THROWS(t.callbackIntNoParams());
    REQUIRE_THROWS(t.callbackIntParam(69));

    t.SetCallbacks();
    REQUIRE(t.callbackIntNoParams() == 555);
    REQUIRE(t.callbackIntParam(69) == (69 * 2));
}
//--------------------------------------------------------------------------