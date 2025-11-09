#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/macro.h"

#include <catch2/catch_test_macros.hpp>

using namespace std::string_literals;


TEST_CASE("Stringify macro string", "[core][macro]")
{
    const auto strMacro = KMP_M_STRINGIFY("Hello world");
    REQUIRE(strMacro == "\"Hello world\""s);
}

TEST_CASE("Stringify macro variable", "[core][macro]")
{
    KMP_MB_UNUSED const auto param = 4;
    const auto strMacro = KMP_M_STRINGIFY(param);
    REQUIRE(strMacro == "param"s);
}

TEST_CASE("Stringify macro expression", "[core][macro]")
{
    KMP_MB_UNUSED const auto param = 4;
    const auto strMacro = KMP_M_STRINGIFY(param == 4);
    REQUIRE(strMacro == "param == 4"s);
}

TEST_CASE("Stringify macro itself", "[core][macro]")
{
    KMP_MB_UNUSED const auto param = 4;
    const auto strMacro = KMP_M_STRINGIFY(KMP_M_STRINGIFY(param));
    REQUIRE(strMacro == "\"param\""s);
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

TEST_CASE("Funcsig macro", "[core][macro]")
{
    const auto functionString = std::string(KMP_FUNC_SIG);
    REQUIRE(!functionString.empty());
}
//--------------------------------------------------------------------------

TEST_CASE("Compiler diagnostics macros", "[core][macro]")
{
    KMP_COMPILER_DIAGNOSTIC_PUSH
#if defined (KMP_COMPILER_MSVC)
    KMP_COMPILER_DIAGNOSTIC_IGNORE(4189)
#else
    KMP_COMPILER_DIAGNOSTIC_IGNORE("-Wunused-variable")
#endif
    int unusedValueThatShouldEmitError = 23;
    KMP_COMPILER_DIAGNOSTIC_POP
    SUCCEED();
}
//--------------------------------------------------------------------------

TEST_CASE("Packed structs", "[core][macro]")
{
    struct NonPackedStruct
    {
        bool a;
        int b;
        bool c;
        int d;
    };

    KMP_BEGIN_PACKED_STRUCT(PackedStruct)
    {
        bool a;
        int b;
        bool c;
        int d;
    };
    KMP_END_PACKED_STRUCT

    REQUIRE((sizeof(NonPackedStruct) != sizeof(PackedStruct)));
    REQUIRE(sizeof(PackedStruct) == 10UL);
}
//--------------------------------------------------------------------------
