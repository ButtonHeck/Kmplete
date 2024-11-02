#include "Kmplete/Core/locale_manager.h"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("Locale tests", "[core][locale]")
{
    bool ok = true;

    // Before any locale handling
    auto localeC = std::setlocale(LC_ALL, NULL);
    REQUIRE(std::string(localeC) == std::string("C"));
    REQUIRE(std::locale().name() == std::string("C"));

    // Create default localeManager
    Kmplete::LocaleManager localeManager;
    localeC = std::setlocale(LC_ALL, NULL);
    auto currentLocale = localeManager.GetLocale();
    REQUIRE(std::string(localeC) == std::string("C"));
    REQUIRE(std::locale().name() == std::string("C"));
    REQUIRE(std::locale().name() == localeManager.GetLocale());

// Only "C" and "POSIX" locales are supported under MinGW port of GCC and libstdc++.
#if defined (KMP_COMPILER_MINGW)
    // Set empty locale
    REQUIRE_NOTHROW(ok = localeManager.SetLocale(""));
    REQUIRE(ok);
    localeC = std::setlocale(LC_ALL, NULL);
    currentLocale = localeManager.GetLocale();
    REQUIRE(std::locale().name() == std::string("*"));
    REQUIRE(std::string(localeC) == std::string("C"));
    REQUIRE(currentLocale == std::string("C"));

    // Set 'C' locale
    REQUIRE_NOTHROW(ok = localeManager.SetLocale("C"));
    REQUIRE(ok);
    localeC = std::setlocale(LC_ALL, NULL);
    currentLocale = localeManager.GetLocale();
    REQUIRE(std::locale().name() == std::string("*"));
    REQUIRE(std::string(localeC) == std::string("C"));
    REQUIRE(currentLocale == std::string("C"));

#else
    // Set 'en_US.UTF-8' locale
    REQUIRE_NOTHROW(ok = localeManager.SetLocale("en_US.UTF-8"));
    REQUIRE(ok);
    localeC = std::setlocale(LC_ALL, NULL);
    currentLocale = localeManager.GetLocale();
    REQUIRE(std::locale().name() == std::string("*"));
    REQUIRE(std::string(localeC) == std::string("en_US.UTF-8"));
    REQUIRE(currentLocale == std::string("en_US.UTF-8"));

    // Set empty locale
    REQUIRE_NOTHROW(ok = localeManager.SetLocale(""));
    REQUIRE(ok);
    localeC = std::setlocale(LC_ALL, NULL);
    currentLocale = localeManager.GetLocale();
    REQUIRE(std::locale().name() == std::string("*"));
    REQUIRE(std::string(localeC) == std::string("C"));
    REQUIRE(currentLocale == std::string("C"));

    // Set 'ru_RU.UTF-8' locale
    REQUIRE_NOTHROW(ok = localeManager.SetLocale("ru_RU.UTF-8"));
    REQUIRE(ok);
    localeC = std::setlocale(LC_ALL, NULL);
    currentLocale = localeManager.GetLocale();
    REQUIRE(std::locale().name() == std::string("*"));
    REQUIRE(std::string(localeC) == std::string("ru_RU.UTF-8"));
    REQUIRE(currentLocale == std::string("ru_RU.UTF-8"));

    // Set 'C' locale
    REQUIRE_NOTHROW(ok = localeManager.SetLocale("C"));
    REQUIRE(ok);
    localeC = std::setlocale(LC_ALL, NULL);
    currentLocale = localeManager.GetLocale();
    REQUIRE(std::locale().name() == std::string("*"));
    REQUIRE(std::string(localeC) == std::string("C"));
    REQUIRE(currentLocale == std::string("C"));

    // Set '.UTF-8' locale
    REQUIRE_NOTHROW(ok = localeManager.SetLocale(".UTF8"));
    localeC = std::setlocale(LC_ALL, NULL);
    currentLocale = localeManager.GetLocale();
    REQUIRE(std::locale().name() == std::string("*"));
    if (ok)
    {
        REQUIRE_FALSE(std::string(localeC) == std::string("C")); // may be like 'Russian_Russia.utf8' and should not be C from previous call
        REQUIRE(currentLocale == std::string(".UTF8"));
    }
    else // Linux might not accept ".UTF8"
    {
        REQUIRE(std::string(localeC) == std::string("C"));
        REQUIRE(currentLocale == std::string("C"));
    }

    // Set 'de_DE' locale
    REQUIRE_NOTHROW(ok = localeManager.SetLocale("de_DE"));
    localeC = std::setlocale(LC_ALL, NULL);
    currentLocale = localeManager.GetLocale();
    REQUIRE(std::locale().name() == std::string("*"));
    bool deutschLocaleOk = ok;
    if (ok)
    {
        REQUIRE(std::string(localeC) == std::string("de_DE"));
        REQUIRE(currentLocale == std::string("de_DE"));
    }
    else // Linux might not accept ".de_DE" (or other lang_LANG like)
    {
        REQUIRE(std::string(localeC) == std::string("C"));
        REQUIRE(currentLocale == std::string("C"));
    }

    // Set invalid locale
    REQUIRE_NOTHROW(ok = localeManager.SetLocale("definitely_invalid_locale_name"));
    localeC = std::setlocale(LC_ALL, NULL);
    currentLocale = localeManager.GetLocale();
    REQUIRE(std::locale().name() == std::string("*"));
    REQUIRE_FALSE(ok);
    if (deutschLocaleOk)
    {
        REQUIRE(std::string(localeC) == std::string("de_DE")); // should be same from previous successful call
        REQUIRE(currentLocale == std::string("de_DE")); // should be same from previous successful call
    }
    else
    {
        REQUIRE(std::string(localeC) == std::string("C")); // should be same from previous unsuccessful call
        REQUIRE(currentLocale == std::string("C")); // should be same from previous unsuccessful call
    }

    // Set 'ru_RU.UTF-8' locale again
    REQUIRE_NOTHROW(ok = localeManager.SetLocale("ru_RU.UTF-8"));
    REQUIRE(ok);
    localeC = std::setlocale(LC_ALL, NULL);
    currentLocale = localeManager.GetLocale();
    REQUIRE(std::locale().name() == std::string("*"));
    REQUIRE(std::string(localeC) == std::string("ru_RU.UTF-8"));
    REQUIRE(currentLocale == std::string("ru_RU.UTF-8"));
#endif
}
//--------------------------------------------------------------------------