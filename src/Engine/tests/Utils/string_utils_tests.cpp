#include "Kmplete/Core/platform.h"
#include "Kmplete/Utils/string_utils.h"

#include <catch2/catch_test_macros.hpp>

#if !defined (KMP_COMPILER_MSVC)
#include <cstddef>
#endif

TEST_CASE("Concatenate empty", "[utils][string]")
{
    const auto result = Kmplete::Utils::Concatenate("");
    REQUIRE(result.empty());
}

TEST_CASE("Concatenate empty multiple", "[utils][string]")
{
    const auto result = Kmplete::Utils::Concatenate("", "", "");
    REQUIRE(result.empty());
}

TEST_CASE("Concatenate", "[utils][string]")
{
    auto result = Kmplete::Utils::Concatenate("string", 123, "other_string", " ", '.');
    REQUIRE(result == std::string("string123other_string ."));

    result = Kmplete::Utils::Concatenate(1L, 2UL, 3.3f, 4.4);
    REQUIRE(result == std::string("123.34.4"));

    result = Kmplete::Utils::Concatenate(1UL, 2L, 3.0, 4.0f);
    REQUIRE(result == std::string("1234"));

    result = Kmplete::Utils::Concatenate(1, ' ', 2, "", 'c');
    REQUIRE(result == std::string("1 2c"));

    result = Kmplete::Utils::Concatenate(std::hex, 15);
    REQUIRE(result == std::string("f"));

    result = Kmplete::Utils::Concatenate(std::hex, std::showbase, 15);
    REQUIRE(result == std::string("0xf"));

    result = Kmplete::Utils::Concatenate(true, false, std::boolalpha, true, false);
    REQUIRE(result == std::string("10truefalse"));

    result = Kmplete::Utils::Concatenate(nullptr);
    REQUIRE(result == std::string("nullptr"));

    result = Kmplete::Utils::Concatenate(std::nullptr_t{});
    REQUIRE(result == std::string("nullptr"));

    result = std::string("");
    result = Kmplete::Utils::Concatenate(&result);
    REQUIRE(!result.empty());
}
//--------------------------------------------------------------------------

TEST_CASE("StringVectorToString empty vector (char delimiter)", "[utils][string]")
{
    std::vector<std::string> vec;
    const auto result = Kmplete::Utils::StringVectorToString(vec, '/');
    REQUIRE(result.empty());
}

TEST_CASE("StringVectorToString empty vector (string delimiter)", "[utils][string]")
{
    std::vector<std::string> vec;
    const auto result = Kmplete::Utils::StringVectorToString(vec, "-YAY-");
    REQUIRE(result.empty());
}

TEST_CASE("StringVectorToString (char delimiter)", "[utils][string]")
{
    std::vector<std::string> vec{"Hello", "World", "!"};
    const auto result = Kmplete::Utils::StringVectorToString(vec, '/');
    REQUIRE(result == std::string("/Hello/World/!"));
}

TEST_CASE("StringVectorToString (string delimiter)", "[utils][string]")
{
    std::vector<std::string> vec{ "Hello", "World", "!" };
    const auto result = Kmplete::Utils::StringVectorToString(vec, "-YAY-");
    REQUIRE(result == std::string("-YAY-Hello-YAY-World-YAY-!"));
}

TEST_CASE("StringVectorToString with some empty strings (char delimiter)", "[utils][string]")
{
    std::vector<std::string> vec{ "", "This", "", "Is", "Sparta", ""};
    const auto result = Kmplete::Utils::StringVectorToString(vec, '/');
    REQUIRE(result == std::string("/This/Is/Sparta"));
}

TEST_CASE("StringVectorToString with some empty strings (string delimiter)", "[utils][string]")
{
    std::vector<std::string> vec{ "", "This", "", "Is", "Sparta", "" };
    const auto result = Kmplete::Utils::StringVectorToString(vec, "-YAY-");
    REQUIRE(result == std::string("-YAY-This-YAY-Is-YAY-Sparta"));
}

TEST_CASE("StringVectorToString with all empty strings (char delimiter)", "[utils][string]")
{
    std::vector<std::string> vec{ "", "", "", "", "", "" };
    const auto result = Kmplete::Utils::StringVectorToString(vec, '/');
    REQUIRE(result.empty());
}

TEST_CASE("StringVectorToString with all empty strings (string delimiter)", "[utils][string]")
{
    std::vector<std::string> vec{ "", "", "", "", "", "" };
    const auto result = Kmplete::Utils::StringVectorToString(vec, "-YAY-");
    REQUIRE(result.empty());
}

TEST_CASE("StringVectorToString single element (char delimiter)", "[utils][string]")
{
    std::vector<std::string> vec{"Alone"};
    const auto result = Kmplete::Utils::StringVectorToString(vec, ".");
    REQUIRE(result == std::string(".Alone"));
}

TEST_CASE("StringVectorToString single element (string delimiter)", "[utils][string]")
{
    std::vector<std::string> vec{ "Alone" };
    const auto result = Kmplete::Utils::StringVectorToString(vec, "Home ");
    REQUIRE(result == std::string("Home Alone"));
}

TEST_CASE("StringVectorToString (empty delimiter)", "[utils][string]")
{
    std::vector<std::string> vec{"Harry", "Potter"};
    const auto result = Kmplete::Utils::StringVectorToString(vec, "");
    REQUIRE(result == std::string("HarryPotter"));
}

TEST_CASE("StringVectorToString with some empty elements (empty delimiter)", "[utils][string]")
{
    std::vector<std::string> vec{ "", "A", "B", "", "C", "", "", "D", "", ""};
    const auto result = Kmplete::Utils::StringVectorToString(vec, "");
    REQUIRE(result == std::string("ABCD"));
}
//--------------------------------------------------------------------------

TEST_CASE("ToSStream empty", "[utils][string]")
{
    std::ostringstream oss;
    Kmplete::Utils::ToSStream(oss, "");
    const auto result = oss.str();
    REQUIRE(result.empty());
}

TEST_CASE("ToSStream empty multiple", "[utils][string]")
{
    std::ostringstream oss;
    Kmplete::Utils::ToSStream(oss, "", "", "");
    const auto result = oss.str();
    REQUIRE(result.empty());
}

TEST_CASE("ToSStream", "[utils][string]")
{
    std::ostringstream oss;
    Kmplete::Utils::ToSStream(oss, "string", 123, "other_string", " ", '.');
    auto result = oss.str();
    REQUIRE(result == std::string("string123other_string ."));

    oss.str("");
    Kmplete::Utils::ToSStream(oss, 1L, 2UL, 3.3f, 4.4);
    result = oss.str();
    REQUIRE(result == std::string("123.34.4"));

    oss.str("");
    Kmplete::Utils::ToSStream(oss, 1UL, 2L, 3.0, 4.0f);
    result = oss.str();
    REQUIRE(result == std::string("1234"));

    oss.str("");
    Kmplete::Utils::ToSStream(oss, 1, ' ', 2, "", 'c');
    result = oss.str();
    REQUIRE(result == std::string("1 2c"));

    oss.str("");
    Kmplete::Utils::ToSStream(oss, std::hex, 15);
    result = oss.str();
    REQUIRE(result == std::string("f"));

    oss.str("");
    Kmplete::Utils::ToSStream(oss, std::hex, std::showbase, 15);
    result = oss.str();
    REQUIRE(result == std::string("0xf"));

    oss.str("");
    Kmplete::Utils::ToSStream(oss, std::noshowbase, true, false, std::boolalpha, true, false);
    result = oss.str();
    REQUIRE(result == std::string("10truefalse"));

    oss.str("");
    Kmplete::Utils::ToSStream(oss, nullptr);
    result = oss.str();
    REQUIRE(result == std::string("nullptr"));

    oss.str("");
    Kmplete::Utils::ToSStream(oss, std::nullptr_t{});
    result = oss.str();
    REQUIRE(result == std::string("nullptr"));

    oss.str("");
    Kmplete::Utils::ToSStream(oss, &result);
    result = oss.str();
    REQUIRE(!result.empty());
}
//--------------------------------------------------------------------------

TEST_CASE("Utf8ToNarrow english", "[utils][string]")
{
    const auto englishString = std::string("bear");
    REQUIRE(englishString.size() == 4);
    const auto fromUtf8 = Kmplete::Utils::Utf8ToNarrow(englishString);
    REQUIRE(englishString == fromUtf8);
}

TEST_CASE("Utf8ToNarrow cyrillic", "[utils][string]")
{
    auto cyrillicStr = std::string("абв");
    if (cyrillicStr.size() != 3) // file is in UTF
    {
        REQUIRE(cyrillicStr.size() == 6);

        const auto fromUtf8 = Kmplete::Utils::Utf8ToNarrow(cyrillicStr);
#if defined (KMP_PLATFORM_WINDOWS)
        // assuming Windows1251 code page
        REQUIRE((fromUtf8.size() == 3 &&
            static_cast<unsigned char>(fromUtf8[0]) == 0xE0 &&
            static_cast<unsigned char>(fromUtf8[1]) == 0xE1 &&
            static_cast<unsigned char>(fromUtf8[2]) == 0xE2));
#else
        // assuming UTF8 code page
        REQUIRE((fromUtf8.size() == 6 && fromUtf8 == cyrillicStr));
#endif
    }

    cyrillicStr = std::string({ 
        static_cast<char>(0xD0),static_cast<char>(0xB3), 
        static_cast<char>(0xD0),static_cast<char>(0xB4), 
        static_cast<char>(0xD0),static_cast<char>(0xB5)});

    const auto fromUtf8 = Kmplete::Utils::Utf8ToNarrow(cyrillicStr);
#if defined (KMP_PLATFORM_WINDOWS)
    // assuming Windows1251 code page
    REQUIRE((fromUtf8.size() == 3 &&
        static_cast<unsigned char>(fromUtf8[0]) == 0xE3 &&
        static_cast<unsigned char>(fromUtf8[1]) == 0xE4 &&
        static_cast<unsigned char>(fromUtf8[2]) == 0xE5));
#else
    // assuming UTF8 code page
    REQUIRE((fromUtf8.size() == 6 && fromUtf8 == cyrillicStr));
#endif
}

TEST_CASE("Utf8ToNarrow mixed", "[utils][string]")
{
    const auto mixedString = std::string("bear абв");
    if (mixedString.size() != 8) // file is in UTF
    {
        REQUIRE(mixedString.size() == (4 + 1 + 6));

        const auto fromUtf8 = Kmplete::Utils::Utf8ToNarrow(mixedString);
#if defined (KMP_PLATFORM_WINDOWS)
        // assuming Windows1251 code page
        REQUIRE((fromUtf8.size() == 8 &&
            static_cast<unsigned char>(fromUtf8[0]) == 98 &&
            static_cast<unsigned char>(fromUtf8[1]) == 101 &&
            static_cast<unsigned char>(fromUtf8[2]) == 97 &&
            static_cast<unsigned char>(fromUtf8[3]) == 114 &&
            static_cast<unsigned char>(fromUtf8[4]) == 32 &&
            static_cast<unsigned char>(fromUtf8[5]) == 0xE0 &&
            static_cast<unsigned char>(fromUtf8[6]) == 0xE1 &&
            static_cast<unsigned char>(fromUtf8[7]) == 0xE2));
#else
        // assuming UTF8 code page
        REQUIRE((fromUtf8.size() == 11 && fromUtf8 == mixedString));
#endif
    }
}
//--------------------------------------------------------------------------

TEST_CASE("NarrowToUtf8 english", "[utils][string]")
{
    const auto englishString = std::string("bear");
    REQUIRE(englishString.size() == 4);
    const auto toUtf8 = Kmplete::Utils::NarrowToUtf8(englishString);
    REQUIRE(englishString == toUtf8);
}

TEST_CASE("NarrowToUtf8 cyrillic", "[utils][string]")
{
    const auto cyrillicStr = std::string({static_cast<char>(0xE0), static_cast<char>(0xE1), static_cast<char>(0xE2)});
    const auto toUtf8 = Kmplete::Utils::NarrowToUtf8(cyrillicStr);
#if defined (KMP_PLATFORM_WINDOWS)
    REQUIRE((toUtf8.size() == 6 &&
        static_cast<unsigned char>(toUtf8[0]) == 0xD0 && static_cast<unsigned char>(toUtf8[1]) == 0xB0 &&
        static_cast<unsigned char>(toUtf8[2]) == 0xD0 && static_cast<unsigned char>(toUtf8[3]) == 0xB1 &&
        static_cast<unsigned char>(toUtf8[4]) == 0xD0 && static_cast<unsigned char>(toUtf8[5]) == 0xB2));
#else
    REQUIRE((toUtf8.size() == 3 && toUtf8 == cyrillicStr));
#endif
}

TEST_CASE("NarrowToUtf8 mixed", "[utils][string]")
{
    const auto mixedStr = std::string({
        static_cast<char>(0xE0), 
        static_cast<char>(98),
        static_cast<char>(101),
        static_cast<char>(97),
        static_cast<char>(114), 
        static_cast<char>(32), 
        static_cast<char>(0xE1), 
        static_cast<char>(0xE2)});

    const auto toUtf8 = Kmplete::Utils::NarrowToUtf8(mixedStr);
#if defined (KMP_PLATFORM_WINDOWS)
    REQUIRE((toUtf8.size() == 11 &&
        static_cast<unsigned char>(toUtf8[0]) == 0xD0 && static_cast<unsigned char>(toUtf8[1]) == 0xB0 &&
        static_cast<unsigned char>(toUtf8[2]) == 98 &&
        static_cast<unsigned char>(toUtf8[3]) == 101 &&
        static_cast<unsigned char>(toUtf8[4]) == 97 &&
        static_cast<unsigned char>(toUtf8[5]) == 114 &&
        static_cast<unsigned char>(toUtf8[6]) == 32 &&
        static_cast<unsigned char>(toUtf8[7]) == 0xD0 && static_cast<unsigned char>(toUtf8[8]) == 0xB1 &&
        static_cast<unsigned char>(toUtf8[9]) == 0xD0 && static_cast<unsigned char>(toUtf8[10]) == 0xB2));
#else
    REQUIRE((toUtf8.size() == 8 && toUtf8 == mixedStr));
#endif
}
//--------------------------------------------------------------------------

TEST_CASE("Conversion ping-pong Utf8", "[utils][string]")
{
    const auto str = std::string("12 test абв АБ_В!\"№;%:?*");
    if (str.size() != 24)
    {
        REQUIRE(str.size() == 32);
        REQUIRE(str == Kmplete::Utils::NarrowToUtf8(Kmplete::Utils::Utf8ToNarrow(str)));
    }
}
//--------------------------------------------------------------------------

TEST_CASE("NarrowToWide english", "[utils][string]")
{
    auto wstr = Kmplete::Utils::NarrowToWide("basic");
    REQUIRE((wstr.size() == 5 && 
        wstr[0] == 98 &&
        wstr[1] == 97 &&
        wstr[2] == 115 &&
        wstr[3] == 105 &&
        wstr[4] == 99));
}

TEST_CASE("NarrowToWide cyrillic", "[utils][string]")
{
#if defined (KMP_PLATFORM_WINDOWS)
    const auto narrow = std::string({static_cast<char>(0xE0), static_cast<char>(0xE1), static_cast<char>(0xE2)});
#else
    setlocale(LC_ALL, "ru_RU.UTF-8");
    const auto narrow = std::string({static_cast<char>(0xD0), static_cast<char>(0xB0), 
                                     static_cast<char>(0xD0), static_cast<char>(0xB1),
                                     static_cast<char>(0xD0), static_cast<char>(0xB2)});
#endif
    const auto wstr = Kmplete::Utils::NarrowToWide(narrow);
    REQUIRE((wstr.size() == 3 &&
        wstr[0] == 0x430 &&
        wstr[1] == 0x431 &&
        wstr[2] == 0x432));
}

TEST_CASE("NarrowToWide mixed", "[utils][string]")
{
#if defined (KMP_PLATFORM_WINDOWS)
    const auto narrow = std::string({static_cast<char>(0xE0), 'a', 'b', 'c', static_cast<char>(0xE1), 'd', 'e'});
#else
    setlocale(LC_ALL, "ru_RU.UTF-8");
    const auto narrow = std::string({static_cast<char>(0xD0), static_cast<char>(0xB0), 'a', 'b', 'c', static_cast<char>(0xD0), static_cast<char>(0xB1), 'd', 'e'});
#endif
    const auto wstr = Kmplete::Utils::NarrowToWide(narrow);
    REQUIRE((wstr.size() == 7 &&
        wstr[0] == 0x430 &&
        wstr[1] == 97 &&
        wstr[2] == 98 &&
        wstr[3] == 99 &&
        wstr[4] == 0x431 &&
        wstr[5] == 100 &&
        wstr[6] == 101));
}
//--------------------------------------------------------------------------

TEST_CASE("WideToNarrow english", "[utils][string]")
{
    const auto wstr = std::wstring(L"abc-def");
    const auto narrow = Kmplete::Utils::WideToNarrow(wstr);
    REQUIRE(narrow == std::string("abc-def"));
}

TEST_CASE("WideToNarrow cyrillic", "[utils][string]")
{
#if !defined (KMP_PLATFORM_WINDOWS)
    setlocale(LC_ALL, "ru_RU.UTF-8");
#endif
    const auto wstr = std::wstring({static_cast<wchar_t>(0x430), static_cast<wchar_t>(0x431), static_cast<wchar_t>(0x432)});
    const auto narrow = Kmplete::Utils::WideToNarrow(wstr);
#if defined (KMP_PLATFORM_WINDOWS)
    REQUIRE((narrow.size() == 3 &&
        static_cast<unsigned char>(narrow[0]) == 0xE0 &&
        static_cast<unsigned char>(narrow[1]) == 0xE1 &&
        static_cast<unsigned char>(narrow[2]) == 0xE2));
#else
    REQUIRE((narrow.size() == 6 &&
        static_cast<unsigned char>(narrow[0]) == 0xD0 &&
        static_cast<unsigned char>(narrow[1]) == 0xB0 &&
        static_cast<unsigned char>(narrow[2]) == 0xD0 &&
        static_cast<unsigned char>(narrow[3]) == 0xB1 &&
        static_cast<unsigned char>(narrow[4]) == 0xD0 &&
        static_cast<unsigned char>(narrow[5]) == 0xB2));
#endif
}

TEST_CASE("WideToNarrow mixed", "[utils][string]")
{
#if !defined (KMP_PLATFORM_WINDOWS)
    setlocale(LC_ALL, "ru_RU.UTF-8");
#endif
    const auto wstr = std::wstring({L'a', static_cast<wchar_t>(0x430), L'b', static_cast<wchar_t>(0x431), L'c', static_cast<wchar_t>(0x432)});
    const auto narrow = Kmplete::Utils::WideToNarrow(wstr);
#if defined (KMP_PLATFORM_WINDOWS)
    REQUIRE((narrow.size() == 6 &&
        static_cast<unsigned char>(narrow[0]) == 'a' &&
        static_cast<unsigned char>(narrow[1]) == 0xE0 && 
        static_cast<unsigned char>(narrow[2]) == 'b' &&
        static_cast<unsigned char>(narrow[3]) == 0xE1 &&
        static_cast<unsigned char>(narrow[4]) == 'c' &&
        static_cast<unsigned char>(narrow[5]) == 0xE2));
#else
    REQUIRE((narrow.size() == 9 &&
        static_cast<unsigned char>(narrow[0]) == 'a' &&
        static_cast<unsigned char>(narrow[1]) == 0xD0 && 
        static_cast<unsigned char>(narrow[2]) == 0xB0 &&
        static_cast<unsigned char>(narrow[3]) == 'b' &&
        static_cast<unsigned char>(narrow[4]) == 0xD0 &&
        static_cast<unsigned char>(narrow[5]) == 0xB1 &&        
        static_cast<unsigned char>(narrow[6]) == 'c' &&
        static_cast<unsigned char>(narrow[7]) == 0xD0 &&
        static_cast<unsigned char>(narrow[8]) == 0xB2));
#endif
}
//--------------------------------------------------------------------------

TEST_CASE("Conversion ping-pong wide-narrow", "[utils][string]")
{
#if !defined (KMP_PLATFORM_WINDOWS)
    setlocale(LC_ALL, "ru_RU.UTF-8");
#endif
    const auto str = std::string("12 test абв АБ_В!\"№;%:?*");
    if (str.size() != 24)
    {
        REQUIRE(str.size() == 32);
        REQUIRE(str == Kmplete::Utils::WideToNarrow(Kmplete::Utils::NarrowToWide(str)));
    }
}
//--------------------------------------------------------------------------
