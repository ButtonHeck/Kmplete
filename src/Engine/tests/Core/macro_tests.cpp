#include "Kmplete/Core/kmplete_api.h"
#include "Kmplete/Core/macro.h"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("Stringify macro string", "[core][macro]")
{
    const auto strMacro = KMP_M_STRINGIFY("Hello world");
    REQUIRE(strMacro == std::string("\"Hello world\""));
}

TEST_CASE("Stringify macro variable", "[core][macro]")
{
    KMP_MB_UNUSED const auto param = 4;
    const auto strMacro = KMP_M_STRINGIFY(param);
    REQUIRE(strMacro == std::string("param"));
}

TEST_CASE("Stringify macro expression", "[core][macro]")
{
    KMP_MB_UNUSED const auto param = 4;
    const auto strMacro = KMP_M_STRINGIFY(param == 4);
    REQUIRE(strMacro == std::string("param == 4"));
}

TEST_CASE("Stringify macro itself", "[core][macro]")
{
    KMP_MB_UNUSED const auto param = 4;
    const auto strMacro = KMP_M_STRINGIFY(KMP_M_STRINGIFY(param));
    REQUIRE(strMacro == std::string("\"param\""));
}
//--------------------------------------------------------------------------

TEST_CASE("Concat macro integers", "[core][macro]")
{
    auto macroResult = KMP_M_CONCAT(10, 4);
    REQUIRE(macroResult == 104);

    macroResult = KMP_M_CONCAT(-10, 4);
    REQUIRE(macroResult == -104);
}

TEST_CASE("Concat macro variables", "[core][macro]")
{
    KMP_MB_UNUSED const auto a = 66;
    KMP_MB_UNUSED const auto b = 77;
    KMP_MB_UNUSED const auto ab = "Hello";

    auto macroResult = KMP_M_CONCAT(a, b);
    REQUIRE(macroResult == ab);

    const auto ab11 = 48;
    const auto macroResult2 = KMP_M_CONCAT(ab, 11);
    REQUIRE(macroResult2 == ab11);
}
//--------------------------------------------------------------------------

TEST_CASE("Dispatch VA_ARGS macro", "[core][macro]")
{
#define KMP_TEST_1_(x1) 10
#define KMP_TEST_2_(x1, x2) 20
#define KMP_TEST_3_(x1, x2, x3) 30
#define KMP_TEST_4_(x1, x2, x3, x4) 40
#define KMP_TEST_5_(...) 999

    auto macroResult = KMP_M_DISPATCH_VA(KMP_TEST_, 1);
    REQUIRE(macroResult == 10);

    macroResult = KMP_M_DISPATCH_VA(KMP_TEST_, 'a');
    REQUIRE(macroResult == 10);

    macroResult = KMP_M_DISPATCH_VA(KMP_TEST_, "hello", "world");
    REQUIRE(macroResult == 20);

    macroResult = KMP_M_DISPATCH_VA(KMP_TEST_, nullptr, nullptr);
    REQUIRE(macroResult == 20);

    macroResult = KMP_M_DISPATCH_VA(KMP_TEST_, 1, 1, 1);
    REQUIRE(macroResult == 30);

    macroResult = KMP_M_DISPATCH_VA(KMP_TEST_, 'a', 'b', nullptr, "string");
    REQUIRE(macroResult == 40);

    macroResult = KMP_M_DISPATCH_VA(KMP_TEST_, 1, 2, 3, 4, 5);
    REQUIRE(macroResult == 999);
}
//--------------------------------------------------------------------------
