#include "Kmplete/Core/filesystem.h"
#include "Kmplete/Utils/string_utils.h"
#include "Kmplete/Utils/string_id.h"
#include "Kmplete/Localization/localization_base.h"
#include "Kmplete/Localization/localization_manager.h"
#include "Kmplete/Localization/localization_translator.h"

#include <catch2/catch_test_macros.hpp>

using namespace Kmplete; // for _sid literal and Utils
static constexpr auto SidTrDomainTests = Utils::ToStringID(KMP_TR_DOMAIN_TESTS);

TEST_CASE("Locale tests", "[localization][locale]")
{
    bool ok = true;

    // Before any locale handling
    auto localeC = std::setlocale(LC_ALL, NULL);
    REQUIRE(std::string(localeC) == "C");
    REQUIRE(std::locale().name() == "C");

    // Create default localizationManager
    LocalizationManager localizationManager;
    localeC = std::setlocale(LC_ALL, NULL);
    auto currentLocale = localizationManager.GetLocale();
    REQUIRE(std::string(localeC) == "C");
    REQUIRE(std::locale().name() == "C");
    REQUIRE(std::locale().name() == localizationManager.GetLocale());

// Only "C" and "POSIX" locales are supported under MinGW port of GCC and libstdc++.
#if defined (KMP_COMPILER_MINGW)
    // Set empty locale
    REQUIRE_NOTHROW(ok = localizationManager.SetLocale(""));
    REQUIRE(ok);
    localeC = std::setlocale(LC_ALL, NULL);
    currentLocale = localizationManager.GetLocale();
    REQUIRE(std::locale().name() == "*");
    REQUIRE(std::string(localeC) == "C");
    REQUIRE(currentLocale == "C");

    // Set 'C' locale
    REQUIRE_NOTHROW(ok = localizationManager.SetLocale("C"));
    REQUIRE(ok);
    localeC = std::setlocale(LC_ALL, NULL);
    currentLocale = localizationManager.GetLocale();
    REQUIRE(std::locale().name() == "*");
    REQUIRE(std::string(localeC) == "C");
    REQUIRE(currentLocale == "C");

#else
    // Set 'en_EN.UTF-8' locale
    REQUIRE_NOTHROW(ok = localizationManager.SetLocale(LocaleEnUTF8Keyword));
    REQUIRE(ok);
    localeC = std::setlocale(LC_ALL, NULL);
    currentLocale = localizationManager.GetLocale();
    REQUIRE(std::locale().name() == "*");
    REQUIRE(std::string(localeC) == LocaleEnUTF8Keyword);
    REQUIRE(currentLocale == LocaleEnUTF8Keyword);

    // Set empty locale
    REQUIRE_NOTHROW(ok = localizationManager.SetLocale(""));
    REQUIRE(ok);
    localeC = std::setlocale(LC_ALL, NULL);
    currentLocale = localizationManager.GetLocale();
    REQUIRE(std::locale().name() == "*");
    REQUIRE(std::string(localeC) == "C");
    REQUIRE(currentLocale == "C");

    // Set 'ru_RU.UTF-8' locale
    REQUIRE_NOTHROW(ok = localizationManager.SetLocale(LocaleRuUTF8Keyword));
    REQUIRE(ok);
    localeC = std::setlocale(LC_ALL, NULL);
    currentLocale = localizationManager.GetLocale();
    REQUIRE(std::locale().name() == "*");
    REQUIRE(std::string(localeC) == LocaleRuUTF8Keyword);
    REQUIRE(currentLocale == LocaleRuUTF8Keyword);

    // Set 'C' locale
    REQUIRE_NOTHROW(ok = localizationManager.SetLocale("C"));
    REQUIRE(ok);
    localeC = std::setlocale(LC_ALL, NULL);
    currentLocale = localizationManager.GetLocale();
    REQUIRE(std::locale().name() == "*");
    REQUIRE(std::string(localeC) == "C");
    REQUIRE(currentLocale == "C");

    // Set '.UTF-8' locale
    REQUIRE_NOTHROW(ok = localizationManager.SetLocale(".UTF8"));
    localeC = std::setlocale(LC_ALL, NULL);
    currentLocale = localizationManager.GetLocale();
    REQUIRE(std::locale().name() == "*");
    if (ok)
    {
        REQUIRE_FALSE(std::string(localeC) == "C"); // may be like 'Russian_Russia.utf8' and should not be C from previous call
        REQUIRE(currentLocale == ".UTF8");
    }
    else // Linux might not accept ".UTF8"
    {
        REQUIRE(std::string(localeC) == "C");
        REQUIRE(currentLocale == "C");
    }

    // Set 'de_DE' locale
    REQUIRE_NOTHROW(ok = localizationManager.SetLocale("de_DE"));
    localeC = std::setlocale(LC_ALL, NULL);
    currentLocale = localizationManager.GetLocale();
    REQUIRE(std::locale().name() == "*");
    bool deutschLocaleOk = ok;
    if (ok)
    {
        REQUIRE(std::string(localeC) == "de_DE");
        REQUIRE(currentLocale == "de_DE");
    }
    else // Linux might not accept ".de_DE" (or other lang_LANG like)
    {
        REQUIRE(std::string(localeC) == "C");
        REQUIRE(currentLocale == "C");
    }

    // Set invalid locale
    REQUIRE_NOTHROW(ok = localizationManager.SetLocale("definitely_invalid_locale_name"));
    localeC = std::setlocale(LC_ALL, NULL);
    currentLocale = localizationManager.GetLocale();
    REQUIRE(std::locale().name() == "*");
    REQUIRE_FALSE(ok);
    if (deutschLocaleOk)
    {
        REQUIRE(std::string(localeC) == "de_DE"); // should be same from previous successful call
        REQUIRE(currentLocale == "de_DE"); // should be same from previous successful call
    }
    else
    {
        REQUIRE(std::string(localeC) == "C"); // should be same from previous unsuccessful call
        REQUIRE(currentLocale == "C"); // should be same from previous unsuccessful call
    }

    // Set 'ru_RU.UTF-8' locale again
    REQUIRE_NOTHROW(ok = localizationManager.SetLocale(LocaleRuUTF8Keyword));
    REQUIRE(ok);
    localeC = std::setlocale(LC_ALL, NULL);
    currentLocale = localizationManager.GetLocale();
    REQUIRE(std::locale().name() == "*");
    REQUIRE(std::string(localeC) == LocaleRuUTF8Keyword);
    REQUIRE(currentLocale == LocaleRuUTF8Keyword);
#endif
}
//--------------------------------------------------------------------------

TEST_CASE("Localization manager set/get locale", "[localization][locale]")
{
    LocalizationManager localizationManager;

    // set en_EN.UTF-8 first time
    bool ok = false;
    REQUIRE_NOTHROW(ok = localizationManager.SetLocale(LocaleEnUTF8Keyword));
    REQUIRE(ok);

    LocaleStr localeStr = "";
    REQUIRE_NOTHROW(localeStr = localizationManager.GetLocale());
    REQUIRE(localeStr == LocaleEnUTF8Keyword);

    // set en_EN.UTF-8 again
    REQUIRE_NOTHROW(ok = localizationManager.SetLocale(LocaleEnUTF8Keyword));
    REQUIRE_FALSE(ok);

    // set ru_RU.UTF-8
    REQUIRE_NOTHROW(ok = localizationManager.SetLocale(LocaleRuUTF8Keyword));
    REQUIRE(ok);
    REQUIRE_NOTHROW(localeStr = localizationManager.GetLocale());
    REQUIRE(localeStr == LocaleRuUTF8Keyword);

    // set default locale
    REQUIRE_NOTHROW(ok = localizationManager.SetLocale(""));
    REQUIRE(ok);
    REQUIRE_NOTHROW(localeStr = localizationManager.GetLocale());
    REQUIRE(localeStr != LocaleRuUTF8Keyword);
    REQUIRE(!localeStr.empty());

    // set invalid locale
    REQUIRE_NOTHROW(ok = localizationManager.SetLocale("jkrjtkjer"));
    REQUIRE_FALSE(ok);
}
//--------------------------------------------------------------------------

TEST_CASE("Localization manager callbacks", "[localization][locale]")
{
    int testValue = 0;
    LocalizationManager localizationManager;

    REQUIRE_NOTHROW(localizationManager.AddLocaleChangedCallback([&](){ testValue++; }));

    // en_EN.UTF-8
    REQUIRE_NOTHROW(localizationManager.SetLocale(LocaleEnUTF8Keyword));
    REQUIRE(testValue == 1);

    int anotherTestValue = 100;
    REQUIRE_NOTHROW(localizationManager.AddLocaleChangedCallback([&](){ anotherTestValue++; }));

    // en_EN.UTF-8 again
    REQUIRE_NOTHROW(localizationManager.SetLocale(LocaleEnUTF8Keyword));
    REQUIRE(testValue == 1);
    REQUIRE(anotherTestValue == 100);

    // ru_RU.UTF-8
    REQUIRE_NOTHROW(localizationManager.SetLocale(LocaleRuUTF8Keyword));
    REQUIRE(testValue == 2);
    REQUIRE(anotherTestValue == 101);
}
//--------------------------------------------------------------------------

TEST_CASE("Localization translator without context singular", "[localization][locale]")
{
    REQUIRE(Filesystem::Initialize());

    LocalizationManager localizationManager;
    // set ru_RU locale
    REQUIRE_NOTHROW(localizationManager.SetLocale(LocaleRuUTF8Keyword));

    TranslationStr yesStr;
    TranslationStr noStr;
    TranslationStr untranslatedStr;

    // Without domain "Tests" and without "locale" path
    REQUIRE_NOTHROW(yesStr = Translator::Translate(KMP_TR_DOMAIN_TESTS, "Yes"));
    REQUIRE(yesStr == "Yes"); // without translation

    // add path
    const auto defaultTranslationsPath = Filesystem::ToGenericU8String(Filesystem::GetApplicationPath().append(LocalesDirectory));
    REQUIRE_NOTHROW(localizationManager.AddMessagesPath(defaultTranslationsPath));
    REQUIRE_NOTHROW(yesStr = Translator::Translate(KMP_TR_DOMAIN_TESTS, "Yes"));
    REQUIRE(yesStr == "Yes"); // without translation

    // add domain (translations must be ready)
    REQUIRE_NOTHROW(localizationManager.AddMessagesDomain(KMP_TR_DOMAIN_TESTS));
    REQUIRE_NOTHROW(yesStr = Translator::Translate(KMP_TR_DOMAIN_TESTS, "Yes"));
    REQUIRE(Utils::Utf8ToNarrow(yesStr) == "Да");
    REQUIRE_NOTHROW(noStr = Translator::Translate(KMP_TR_DOMAIN_TESTS, "No"));
    REQUIRE(Utils::Utf8ToNarrow(noStr) == "Нет");
    REQUIRE_NOTHROW(untranslatedStr = Translator::Translate(KMP_TR_DOMAIN_TESTS, "Untranslated"));
    REQUIRE(untranslatedStr == "Untranslated");

    // switch to en_EN
    REQUIRE(localizationManager.SetLocale(LocaleEnUTF8Keyword));
    REQUIRE_NOTHROW(yesStr = Translator::Translate(KMP_TR_DOMAIN_TESTS, "Yes"));
    REQUIRE(yesStr == "Yes");
    REQUIRE_NOTHROW(noStr = Translator::Translate(KMP_TR_DOMAIN_TESTS, "No"));
    REQUIRE(noStr == "No");
    REQUIRE_NOTHROW(untranslatedStr = Translator::Translate(KMP_TR_DOMAIN_TESTS, "Untranslated"));
    REQUIRE(untranslatedStr == "Untranslated");

    // switch to ru_RU again
    REQUIRE_NOTHROW(localizationManager.SetLocale(LocaleRuUTF8Keyword));
    REQUIRE_NOTHROW(yesStr = Translator::Translate(KMP_TR_DOMAIN_TESTS, "Yes"));
    REQUIRE(Utils::Utf8ToNarrow(yesStr) == "Да");
    REQUIRE_NOTHROW(noStr = Translator::Translate(KMP_TR_DOMAIN_TESTS, "No"));
    REQUIRE(Utils::Utf8ToNarrow(noStr) == "Нет");
    REQUIRE_NOTHROW(untranslatedStr = Translator::Translate(KMP_TR_DOMAIN_TESTS, "Untranslated"));
    REQUIRE(untranslatedStr == "Untranslated");
}
//--------------------------------------------------------------------------

TEST_CASE("Localization translator with context singular", "[localization][locale]")
{
    REQUIRE(Filesystem::Initialize());

    LocalizationManager localizationManager;
    REQUIRE_NOTHROW(localizationManager.SetLocale(LocaleRuUTF8Keyword));

    TranslationStr openVerbStr;
    TranslationStr openAdjStr;
    TranslationStr untranslatedStr;

    // Without domain "Tests" and without messages path
    REQUIRE_NOTHROW(openVerbStr = Translator::TranslateCtx(KMP_TR_DOMAIN_TESTS, "Open", "Verb"));
    REQUIRE(openVerbStr == "Open");

    // add path
    const auto defaultTranslationsPath = Filesystem::ToGenericU8String(Filesystem::GetApplicationPath().append(LocalesDirectory));
    REQUIRE_NOTHROW(localizationManager.AddMessagesPath(defaultTranslationsPath));
    REQUIRE_NOTHROW(openVerbStr = Translator::TranslateCtx(KMP_TR_DOMAIN_TESTS, "Open", "Verb"));
    REQUIRE(openVerbStr == "Open");

    // add domain "Tests"
    REQUIRE_NOTHROW(localizationManager.AddMessagesDomain(KMP_TR_DOMAIN_TESTS));
    REQUIRE_NOTHROW(openVerbStr = Translator::TranslateCtx(KMP_TR_DOMAIN_TESTS, "Open", "Verb"));
    REQUIRE(Utils::Utf8ToNarrow(openVerbStr) == "Открыть");
    REQUIRE_NOTHROW(openAdjStr = Translator::TranslateCtx(KMP_TR_DOMAIN_TESTS, "Open", "Adjective"));
    REQUIRE(Utils::Utf8ToNarrow(openAdjStr) == "Открытый");
    REQUIRE_NOTHROW(untranslatedStr = Translator::TranslateCtx(KMP_TR_DOMAIN_TESTS, "Untranslated", "Some context"));
    REQUIRE(untranslatedStr == "Untranslated");

    // switch to en_EN
    REQUIRE(localizationManager.SetLocale(LocaleEnUTF8Keyword));
    REQUIRE_NOTHROW(openVerbStr = Translator::TranslateCtx(KMP_TR_DOMAIN_TESTS, "Open", "Verb"));
    REQUIRE(openVerbStr == "Open");
    REQUIRE_NOTHROW(openAdjStr = Translator::TranslateCtx(KMP_TR_DOMAIN_TESTS, "Open", "Adjective"));
    REQUIRE(openAdjStr == "Open");
    REQUIRE_NOTHROW(untranslatedStr = Translator::TranslateCtx(KMP_TR_DOMAIN_TESTS, "Untranslated", "Some context"));
    REQUIRE(untranslatedStr == "Untranslated");

    // switch to ru_RU again
    REQUIRE(localizationManager.SetLocale(LocaleRuUTF8Keyword));
    REQUIRE_NOTHROW(openVerbStr = Translator::TranslateCtx(KMP_TR_DOMAIN_TESTS, "Open", "Verb"));
    REQUIRE(Utils::Utf8ToNarrow(openVerbStr) == "Открыть");
    REQUIRE_NOTHROW(openAdjStr = Translator::TranslateCtx(KMP_TR_DOMAIN_TESTS, "Open", "Adjective"));
    REQUIRE(Utils::Utf8ToNarrow(openAdjStr) == "Открытый");
    REQUIRE_NOTHROW(untranslatedStr = Translator::TranslateCtx(KMP_TR_DOMAIN_TESTS, "Untranslated", "Some context"));
    REQUIRE(untranslatedStr == "Untranslated");
}
//--------------------------------------------------------------------------

TEST_CASE("Localization manager Translation functions without context singular", "[localization][locale]")
{
    REQUIRE(Filesystem::Initialize());

    LocalizationManager localizationManager;
    REQUIRE_NOTHROW(localizationManager.SetLocale(LocaleRuUTF8Keyword));
    const auto defaultTranslationsPath = Filesystem::ToGenericU8String(Filesystem::GetApplicationPath().append(LocalesDirectory));
    REQUIRE_NOTHROW(localizationManager.AddMessagesPath(defaultTranslationsPath));
    bool ok = false;
    REQUIRE_NOTHROW(ok = localizationManager.AddMessagesDomain(KMP_TR_DOMAIN_TESTS));
    REQUIRE(ok);

    REQUIRE_NOTHROW(localizationManager.Translate(KMP_TR_DOMAIN_TESTS, "Yes"));
    REQUIRE_NOTHROW(localizationManager.Translate(KMP_TR_DOMAIN_TESTS, "No"));
    REQUIRE_NOTHROW(localizationManager.Translate(KMP_TR_DOMAIN_TESTS, "Untranslated"));

    TranslationStr yesStr;
    TranslationStr noStr;
    TranslationStr untranslatedStr;
    TranslationStr unknownStr;

    // existing domain Translation
    REQUIRE_NOTHROW(yesStr = localizationManager.Translation(SidTrDomainTests, "Yes"_sid));
    REQUIRE(Utils::Utf8ToNarrow(yesStr) == "Да");
    REQUIRE_NOTHROW(noStr = localizationManager.Translation(SidTrDomainTests, "No"_sid));
    REQUIRE(Utils::Utf8ToNarrow(noStr) == "Нет");
    REQUIRE_NOTHROW(untranslatedStr = localizationManager.Translation(SidTrDomainTests, "Untranslated"_sid));
    REQUIRE(untranslatedStr == "Untranslated");
    REQUIRE_NOTHROW(unknownStr = localizationManager.Translation(SidTrDomainTests, "Unknown"_sid));
    REQUIRE(unknownStr.empty());

    // existing domain TranslationOrFallback
    REQUIRE_NOTHROW(yesStr = localizationManager.TranslationOrFallback(SidTrDomainTests, "Yes"_sid, "FALLBACK"));
    REQUIRE(Utils::Utf8ToNarrow(yesStr) == "Да");
    REQUIRE_NOTHROW(untranslatedStr = localizationManager.TranslationOrFallback(SidTrDomainTests, "Untranslated"_sid, "FALLBACK"));
    REQUIRE(untranslatedStr == "Untranslated");
    REQUIRE_NOTHROW(unknownStr = localizationManager.TranslationOrFallback(SidTrDomainTests, "Unknown"_sid, "FALLBACK"));
    REQUIRE(unknownStr == "FALLBACK");

    // non-existing domain Translation
    REQUIRE_NOTHROW(yesStr = localizationManager.Translation(123, "Yes"_sid));
    REQUIRE(yesStr.empty());
    REQUIRE_NOTHROW(noStr = localizationManager.Translation(123, "No"_sid));
    REQUIRE(noStr.empty());
    REQUIRE_NOTHROW(untranslatedStr = localizationManager.Translation(123, "Untranslated"_sid));
    REQUIRE(untranslatedStr.empty());

    // non-existing domain TranslationOrFallback
    REQUIRE_NOTHROW(yesStr = localizationManager.TranslationOrFallback(123, "Yes"_sid, "FALLBACK"));
    REQUIRE(yesStr == "FALLBACK");

    // check again after remove "Tests" domain
    REQUIRE_NOTHROW(localizationManager.RemoveMessagesDomain(KMP_TR_DOMAIN_TESTS));
    REQUIRE_NOTHROW(yesStr = localizationManager.Translation(SidTrDomainTests, "Yes"_sid));
    REQUIRE(yesStr.empty());
    REQUIRE_NOTHROW(noStr = localizationManager.Translation(SidTrDomainTests, "No"_sid));
    REQUIRE(noStr.empty());
    REQUIRE_NOTHROW(untranslatedStr = localizationManager.Translation(SidTrDomainTests, "Untranslated"_sid));
    REQUIRE(untranslatedStr.empty());

    // check again TranslationOrFallback
    REQUIRE_NOTHROW(yesStr = localizationManager.TranslationOrFallback(SidTrDomainTests, "Yes"_sid, "FALLBACK"));
    REQUIRE(yesStr == "FALLBACK");
    REQUIRE_NOTHROW(untranslatedStr = localizationManager.TranslationOrFallback(SidTrDomainTests, "Untranslated"_sid, "FALLBACK"));
    REQUIRE(untranslatedStr == "FALLBACK");
    REQUIRE_NOTHROW(unknownStr = localizationManager.TranslationOrFallback(SidTrDomainTests, "Unknown"_sid, "FALLBACK"));
    REQUIRE(unknownStr == "FALLBACK");
}
//--------------------------------------------------------------------------

TEST_CASE("Localization manager Translation functions with context singular", "[localization][locale]")
{
    REQUIRE(Filesystem::Initialize());

    TranslationStr openVerbStr;
    TranslationStr openAdjStr;
    TranslationStr untranslatedStr;
    TranslationStr unknownStr;

    LocalizationManager localizationManager;
    REQUIRE_NOTHROW(localizationManager.SetLocale(LocaleRuUTF8Keyword));
    const auto defaultTranslationsPath = Filesystem::ToGenericU8String(Filesystem::GetApplicationPath().append(LocalesDirectory));
    REQUIRE_NOTHROW(localizationManager.AddMessagesPath(defaultTranslationsPath));
    bool ok = false;
    REQUIRE_NOTHROW(ok = localizationManager.AddMessagesDomain(KMP_TR_DOMAIN_TESTS));
    REQUIRE(ok);

    // add existing translations with existing context
    REQUIRE_NOTHROW(localizationManager.TranslateCtx(KMP_TR_DOMAIN_TESTS, "Open", "Verb"));
    REQUIRE_NOTHROW(localizationManager.TranslateCtx(KMP_TR_DOMAIN_TESTS, "Open", "Adjective"));
    REQUIRE_NOTHROW(localizationManager.TranslateCtx(KMP_TR_DOMAIN_TESTS, "Untranslated", "Some context"));

    // check existing translations with existing context
    REQUIRE_NOTHROW(openVerbStr = localizationManager.TranslationCtx(SidTrDomainTests, "Open"_sid, "Verb"_sid));
    REQUIRE(Utils::Utf8ToNarrow(openVerbStr) == "Открыть");
    REQUIRE_NOTHROW(openAdjStr = localizationManager.TranslationCtx(SidTrDomainTests, "Open"_sid, "Adjective"_sid));
    REQUIRE(Utils::Utf8ToNarrow(openAdjStr) == "Открытый");
    REQUIRE_NOTHROW(untranslatedStr = localizationManager.TranslationCtx(SidTrDomainTests, "Untranslated"_sid, "Some context"_sid));
    REQUIRE(untranslatedStr == "Untranslated");
    REQUIRE_NOTHROW(unknownStr = localizationManager.TranslationCtx(SidTrDomainTests, "Unknown"_sid, "Some context"_sid));
    REQUIRE(unknownStr.empty());

    // check for invalid domain
    REQUIRE_NOTHROW(openVerbStr = localizationManager.TranslationCtx(123, "Open"_sid, "Verb"_sid));
    REQUIRE(openVerbStr.empty());
    REQUIRE_NOTHROW(openAdjStr = localizationManager.TranslationCtx(123, "Open"_sid, "Adjective"_sid));
    REQUIRE(openAdjStr.empty());
    REQUIRE_NOTHROW(untranslatedStr = localizationManager.TranslationCtx(123, "Untranslated"_sid, "Some context"_sid));
    REQUIRE(untranslatedStr.empty());
    REQUIRE_NOTHROW(unknownStr = localizationManager.TranslationCtx(123, "Unknown"_sid, "Some context"_sid));
    REQUIRE(unknownStr.empty());

    // check with unknown context
    REQUIRE_NOTHROW(openVerbStr = localizationManager.TranslationCtx(SidTrDomainTests, "Open"_sid, "FAILURE"_sid));
    REQUIRE(openVerbStr.empty());
    REQUIRE_NOTHROW(openAdjStr = localizationManager.TranslationCtx(SidTrDomainTests, "Open"_sid, "FAILURE"_sid));
    REQUIRE(openAdjStr.empty());
    REQUIRE_NOTHROW(untranslatedStr = localizationManager.TranslationCtx(SidTrDomainTests, "Untranslated"_sid, "FAILURE"_sid));
    REQUIRE(untranslatedStr.empty());
    REQUIRE_NOTHROW(unknownStr = localizationManager.TranslationCtx(SidTrDomainTests, "Unknown"_sid, "FAILURE"_sid));
    REQUIRE(unknownStr.empty());

    // check with valid context and with valid sources
    REQUIRE_NOTHROW(openVerbStr = localizationManager.TranslationCtx(SidTrDomainTests, "OPEN"_sid, "Verb"_sid));
    REQUIRE(openVerbStr.empty());
    REQUIRE_NOTHROW(openAdjStr = localizationManager.TranslationCtx(SidTrDomainTests, "OPEN"_sid, "Adjective"_sid));
    REQUIRE(openAdjStr.empty());
    REQUIRE_NOTHROW(untranslatedStr = localizationManager.TranslationCtx(SidTrDomainTests, "UNTRANSLATED"_sid, "Some context"_sid));
    REQUIRE(untranslatedStr.empty());
    REQUIRE_NOTHROW(unknownStr = localizationManager.TranslationCtx(SidTrDomainTests, "UNKNOWN"_sid, "Some context"_sid));
    REQUIRE(unknownStr.empty());

    // check TranslationCtxOrFallback with valid sources and valid contexts
    REQUIRE_NOTHROW(openVerbStr = localizationManager.TranslationCtxOrFallback(SidTrDomainTests, "Open"_sid, "Verb"_sid, "FALLBACK"));
    REQUIRE(Utils::Utf8ToNarrow(openVerbStr) == "Открыть");
    REQUIRE_NOTHROW(openAdjStr = localizationManager.TranslationCtxOrFallback(SidTrDomainTests, "Open"_sid, "Adjective"_sid, "FALLBACK"));
    REQUIRE(Utils::Utf8ToNarrow(openAdjStr) == "Открытый");
    REQUIRE_NOTHROW(untranslatedStr = localizationManager.TranslationCtxOrFallback(SidTrDomainTests, "Untranslated"_sid, "Some context"_sid, "FALLBACK"));
    REQUIRE(untranslatedStr == "Untranslated");
    REQUIRE_NOTHROW(unknownStr = localizationManager.TranslationCtxOrFallback(SidTrDomainTests, "Unknown"_sid, "Some context"_sid, "FALLBACK"));
    REQUIRE(unknownStr == "FALLBACK");

    // check TranslationCtxOrFallback with invalid sources/contexts
    REQUIRE_NOTHROW(openVerbStr = localizationManager.TranslationCtxOrFallback(SidTrDomainTests, "Open"_sid, "FAILURE"_sid, "FALLBACK"));
    REQUIRE(openVerbStr == "FALLBACK");
    REQUIRE_NOTHROW(openAdjStr = localizationManager.TranslationCtxOrFallback(SidTrDomainTests, "OPEN"_sid, "Adjective"_sid, "FALLBACK"));
    REQUIRE(openAdjStr == "FALLBACK");

    // check TranslationCtxOrFallback with invalid domain
    REQUIRE_NOTHROW(openVerbStr = localizationManager.TranslationCtxOrFallback(123, "Open"_sid, "Verb"_sid, "FALLBACK"));
    REQUIRE(openVerbStr == "FALLBACK");
    REQUIRE_NOTHROW(untranslatedStr = localizationManager.TranslationCtxOrFallback(123, "Untranslated"_sid, "Some context"_sid, "FALLBACK"));
    REQUIRE(untranslatedStr == "FALLBACK");

    // check TranslationCtx with all entries valid but without domain
    REQUIRE_NOTHROW(localizationManager.RemoveMessagesDomain(KMP_TR_DOMAIN_TESTS));
    REQUIRE_NOTHROW(openVerbStr = localizationManager.TranslationCtx(SidTrDomainTests, "Open"_sid, "Verb"_sid));
    REQUIRE(openVerbStr.empty());
    REQUIRE_NOTHROW(openAdjStr = localizationManager.TranslationCtx(SidTrDomainTests, "Open"_sid, "Adjective"_sid));
    REQUIRE(openAdjStr.empty());
    REQUIRE_NOTHROW(untranslatedStr = localizationManager.TranslationCtx(SidTrDomainTests, "Untranslated"_sid, "Some context"_sid));
    REQUIRE(untranslatedStr.empty());
    REQUIRE_NOTHROW(unknownStr = localizationManager.TranslationCtx(SidTrDomainTests, "Unknown"_sid, "Some context"_sid));
    REQUIRE(unknownStr.empty());

    // check TranslationCtxOrFallback with all entries valid but without domain
    REQUIRE_NOTHROW(localizationManager.RemoveMessagesDomain(KMP_TR_DOMAIN_TESTS));
    REQUIRE_NOTHROW(openVerbStr = localizationManager.TranslationCtxOrFallback(SidTrDomainTests, "Open"_sid, "Verb"_sid, "FALLBACK"));
    REQUIRE(openVerbStr == "FALLBACK");
    REQUIRE_NOTHROW(openAdjStr = localizationManager.TranslationCtxOrFallback(SidTrDomainTests, "Open"_sid, "Adjective"_sid, "FALLBACK"));
    REQUIRE(openAdjStr == "FALLBACK");
    REQUIRE_NOTHROW(untranslatedStr = localizationManager.TranslationCtxOrFallback(SidTrDomainTests, "Untranslated"_sid, "Some context"_sid, "FALLBACK"));
    REQUIRE(untranslatedStr == "FALLBACK");
}
//--------------------------------------------------------------------------

TEST_CASE("Localization manager dictionaries without context singular", "[localization][locale]")
{
    REQUIRE(Filesystem::Initialize());

    TranslationStr yesStr;
    TranslationStr noStr;
    TranslationStr untranslatedStr;
    TranslationStr unknownStr;

    LocalizationManager localizationManager;
    bool ok = false;
    REQUIRE_NOTHROW(ok = localizationManager.SetLocale(LocaleRuUTF8Keyword));
    REQUIRE(ok);
    REQUIRE_NOTHROW(ok = localizationManager.AddMessagesDomain(KMP_TR_DOMAIN_TESTS));
    REQUIRE(ok);

    // try add "tests" domain again
    REQUIRE_NOTHROW(ok = localizationManager.AddMessagesDomain(KMP_TR_DOMAIN_TESTS));
    REQUIRE_FALSE(ok);

    // check double remove
    REQUIRE_NOTHROW(ok = localizationManager.RemoveMessagesDomain(KMP_TR_DOMAIN_TESTS));
    REQUIRE(ok);
    REQUIRE_NOTHROW(ok = localizationManager.RemoveMessagesDomain(KMP_TR_DOMAIN_TESTS));
    REQUIRE_FALSE(ok);

    // add again
    REQUIRE_NOTHROW(ok = localizationManager.AddMessagesDomain(KMP_TR_DOMAIN_TESTS));
    REQUIRE(ok);

    // try add without messages path set
    REQUIRE_NOTHROW(localizationManager.Translate(KMP_TR_DOMAIN_TESTS, "Yes"));
    REQUIRE_NOTHROW(localizationManager.Translate(KMP_TR_DOMAIN_TESTS, "No"));
    REQUIRE_NOTHROW(yesStr = localizationManager.Translation(SidTrDomainTests, "Yes"_sid));
    REQUIRE_NOTHROW(noStr = localizationManager.Translation(SidTrDomainTests, "No"_sid));
    REQUIRE(yesStr == "Yes");
    REQUIRE(noStr == "No");

    // try add again after messages path set
    const auto defaultTranslationsPath = Filesystem::ToGenericU8String(Filesystem::GetApplicationPath().append(LocalesDirectory));
    REQUIRE_NOTHROW(localizationManager.AddMessagesPath(defaultTranslationsPath));
    REQUIRE_NOTHROW(ok = localizationManager.RemoveMessagesDomain(KMP_TR_DOMAIN_TESTS));
    REQUIRE(ok);
    REQUIRE_NOTHROW(ok = localizationManager.AddMessagesDomain(KMP_TR_DOMAIN_TESTS));
    REQUIRE(ok);
    REQUIRE_NOTHROW(localizationManager.Translate(KMP_TR_DOMAIN_TESTS, "Yes"));
    REQUIRE_NOTHROW(localizationManager.Translate(KMP_TR_DOMAIN_TESTS, "No"));
    REQUIRE_NOTHROW(localizationManager.Translate(KMP_TR_DOMAIN_TESTS, "Untranslated"));
    REQUIRE_NOTHROW(yesStr = localizationManager.Translation(SidTrDomainTests, "Yes"_sid));
    REQUIRE_NOTHROW(noStr = localizationManager.Translation(SidTrDomainTests, "No"_sid));
    REQUIRE_NOTHROW(untranslatedStr = localizationManager.Translation(SidTrDomainTests, "Untranslated"_sid));
    REQUIRE_NOTHROW(unknownStr = localizationManager.Translation(SidTrDomainTests, "Unknown"_sid));
    REQUIRE(Utils::Utf8ToNarrow(yesStr) == "Да");
    REQUIRE(Utils::Utf8ToNarrow(noStr) == "Нет");
    REQUIRE(untranslatedStr == "Untranslated");
    REQUIRE(unknownStr.empty());

    // try after remove "Tests" domain
    REQUIRE_NOTHROW(ok = localizationManager.RemoveMessagesDomain(KMP_TR_DOMAIN_TESTS));
    REQUIRE(ok);
    REQUIRE_NOTHROW(yesStr = localizationManager.Translation(SidTrDomainTests, "Yes"_sid));
    REQUIRE_NOTHROW(noStr = localizationManager.Translation(SidTrDomainTests, "No"_sid));
    REQUIRE(yesStr.empty());
    REQUIRE(noStr.empty());
}
//--------------------------------------------------------------------------

TEST_CASE("Localization manager dictionaries with context singular", "[localization][locale]")
{
    REQUIRE(Filesystem::Initialize());

    LocalizationManager localizationManager;
    REQUIRE_NOTHROW(localizationManager.SetLocale(LocaleRuUTF8Keyword));
    const auto defaultTranslationsPath = Filesystem::ToGenericU8String(Filesystem::GetApplicationPath().append(LocalesDirectory));
    REQUIRE_NOTHROW(localizationManager.AddMessagesPath(defaultTranslationsPath));
    bool ok = false;
    REQUIRE_NOTHROW(ok = localizationManager.AddMessagesDomain(KMP_TR_DOMAIN_TESTS));
    REQUIRE(ok);

    TranslationStr openVerbStr;
    TranslationStr openAdjStr;
    TranslationStr untranslatedStr;
    TranslationStr unknownStr;

    REQUIRE_NOTHROW(localizationManager.TranslateCtx(KMP_TR_DOMAIN_TESTS, "Open", "Verb"));
    REQUIRE_NOTHROW(localizationManager.TranslateCtx(KMP_TR_DOMAIN_TESTS, "Open", "Adjective"));
    REQUIRE_NOTHROW(localizationManager.TranslateCtx(KMP_TR_DOMAIN_TESTS, "Untranslated", "Some context"));

    // try TranslationCtx valid sources or invalid context
    REQUIRE_NOTHROW(openVerbStr = localizationManager.TranslationCtx(SidTrDomainTests, "Open"_sid, "VERB"_sid));
    REQUIRE(openVerbStr.empty());
    REQUIRE_NOTHROW(openVerbStr = localizationManager.TranslationCtx(SidTrDomainTests, "OPEN"_sid, "Verb"_sid));
    REQUIRE(openVerbStr.empty());

    // try TranslationCtx invalid sources and invalid context
    REQUIRE_NOTHROW(openVerbStr = localizationManager.TranslationCtx(SidTrDomainTests, "OPEN"_sid, "VERB"_sid));
    REQUIRE(openVerbStr.empty());

    // try TranslationCtx valid sources and context but invalid domain
    REQUIRE_NOTHROW(localizationManager.TranslateCtx("123", "Open", "Verb"));
    REQUIRE_NOTHROW(openVerbStr = localizationManager.TranslationCtx(123, "Open"_sid, "Verb"_sid));
    REQUIRE(openVerbStr.empty());

    // try TranslationCtx all valid
    REQUIRE_NOTHROW(openVerbStr = localizationManager.TranslationCtx(SidTrDomainTests, "Open"_sid, "Verb"_sid));
    REQUIRE_NOTHROW(openAdjStr = localizationManager.TranslationCtx(SidTrDomainTests, "Open"_sid, "Adjective"_sid));
    REQUIRE_NOTHROW(untranslatedStr = localizationManager.TranslationCtx(SidTrDomainTests, "Untranslated"_sid, "Some context"_sid));
    REQUIRE(Utils::Utf8ToNarrow(openVerbStr) == "Открыть");
    REQUIRE(Utils::Utf8ToNarrow(openAdjStr) == "Открытый");
    REQUIRE(untranslatedStr == "Untranslated");

    // try TranslationCtx after remove "Tests" domain
    REQUIRE_NOTHROW(ok = localizationManager.RemoveMessagesDomain(KMP_TR_DOMAIN_TESTS));
    REQUIRE(ok);
    REQUIRE_NOTHROW(openVerbStr = localizationManager.TranslationCtx(SidTrDomainTests, "Open"_sid, "Verb"_sid));
    REQUIRE_NOTHROW(openAdjStr = localizationManager.TranslationCtx(SidTrDomainTests, "Open"_sid, "Adjective"_sid));
    REQUIRE_NOTHROW(untranslatedStr = localizationManager.TranslationCtx(SidTrDomainTests, "Untranslated"_sid, "Some context"_sid));
    REQUIRE(openVerbStr.empty());
    REQUIRE(openAdjStr.empty());
    REQUIRE(untranslatedStr.empty());

    REQUIRE_NOTHROW(ok = localizationManager.AddMessagesDomain(KMP_TR_DOMAIN_TESTS));
    REQUIRE(ok);
    REQUIRE_NOTHROW(localizationManager.TranslateCtx(KMP_TR_DOMAIN_TESTS, "Open", "Verb"));
    REQUIRE_NOTHROW(localizationManager.TranslateCtx(KMP_TR_DOMAIN_TESTS, "Open", "Adjective"));
    REQUIRE_NOTHROW(localizationManager.TranslateCtx(KMP_TR_DOMAIN_TESTS, "Untranslated", "Some context"));

    // try TranslationCtxOrFallback invalid sources or invalid context
    REQUIRE_NOTHROW(openVerbStr = localizationManager.TranslationCtxOrFallback(SidTrDomainTests, "OPEN"_sid, "Verb"_sid, "FALLBACK"));
    REQUIRE(openVerbStr == "FALLBACK");

    // try TranslationCtxOrFallback invalid source and context
    REQUIRE_NOTHROW(openVerbStr = localizationManager.TranslationCtxOrFallback(SidTrDomainTests, "OPEN"_sid, "VERB"_sid, "FALLBACK"));
    REQUIRE(openVerbStr == "FALLBACK");

    // try TranslationCtxOrFallback invalid domain
    REQUIRE_NOTHROW(openVerbStr = localizationManager.TranslationCtxOrFallback(123, "Open"_sid, "Verb"_sid, "FALLBACK"));
    REQUIRE(openVerbStr == "FALLBACK");

    // try TranslationCtxOrFallback all valid
    REQUIRE_NOTHROW(openVerbStr = localizationManager.TranslationCtxOrFallback(SidTrDomainTests, "Open"_sid, "Verb"_sid, "FALLBACK"));
    REQUIRE_FALSE(openVerbStr == "FALLBACK");

    // try TranslationCtxOrFallback after remove "Tests" domain
    REQUIRE_NOTHROW(ok = localizationManager.RemoveMessagesDomain(KMP_TR_DOMAIN_TESTS));
    REQUIRE(ok);
    REQUIRE_NOTHROW(openVerbStr = localizationManager.TranslationCtxOrFallback(SidTrDomainTests, "Open"_sid, "Verb"_sid, "FALLBACK"));
    REQUIRE(openVerbStr == "FALLBACK");
}
//--------------------------------------------------------------------------

TEST_CASE("Localization translator without context plural", "[localization][locale]")
{
    REQUIRE(Filesystem::Initialize());

    LocalizationManager localizationManager;
    REQUIRE_NOTHROW(localizationManager.SetLocale(LocaleRuUTF8Keyword));
    const auto defaultTranslationsPath = Filesystem::ToGenericU8String(Filesystem::GetApplicationPath().append(LocalesDirectory));
    REQUIRE_NOTHROW(localizationManager.AddMessagesPath(defaultTranslationsPath));
    bool ok = false;
    REQUIRE_NOTHROW(ok = localizationManager.AddMessagesDomain(KMP_TR_DOMAIN_TESTS));
    REQUIRE(ok);

    TranslationStr appleStrUnformattedForm1;
    TranslationStr appleStrUnformattedForm2;
    TranslationStr appleStrUnformattedForm3;
    TranslationStr appleStr0;
    TranslationStr appleStr1;
    TranslationStr appleStr2;
    TranslationStr appleStr5;
    TranslationStr appleStr11;
    TranslationStr appleStr21;

    REQUIRE_NOTHROW(appleStrUnformattedForm1 = Translator::Translate(KMP_TR_DOMAIN_TESTS, "{1} apple", "{1} apples", 1));
    REQUIRE_NOTHROW(appleStrUnformattedForm2 = Translator::Translate(KMP_TR_DOMAIN_TESTS, "{1} apple", "{1} apples", 2));
    REQUIRE_NOTHROW(appleStrUnformattedForm3 = Translator::Translate(KMP_TR_DOMAIN_TESTS, "{1} apple", "{1} apples", 5));
    REQUIRE_FALSE(appleStrUnformattedForm1.empty());
    REQUIRE_FALSE(appleStrUnformattedForm2.empty());
    REQUIRE_FALSE(appleStrUnformattedForm3.empty());

    // existing domain Translation Russian
    REQUIRE_NOTHROW(appleStr1 = Translator::Format(appleStrUnformattedForm1, 1));
    REQUIRE(Utils::Utf8ToNarrow(appleStr1) == "1 яблоко");
    REQUIRE_NOTHROW(appleStr2 = Translator::Format(appleStrUnformattedForm2, 2));
    REQUIRE(Utils::Utf8ToNarrow(appleStr2) == "2 яблока");
    REQUIRE_NOTHROW(appleStr5 = Translator::Format(appleStrUnformattedForm3, 5));
    REQUIRE(Utils::Utf8ToNarrow(appleStr5) == "5 яблок");
    REQUIRE_NOTHROW(appleStr11 = Translator::Format(appleStrUnformattedForm3, 11));
    REQUIRE(Utils::Utf8ToNarrow(appleStr11) == "11 яблок");
    REQUIRE_NOTHROW(appleStr0 = Translator::Format(appleStrUnformattedForm3, 0));
    REQUIRE(Utils::Utf8ToNarrow(appleStr0) == "0 яблок");
    REQUIRE_NOTHROW(appleStr21 = Translator::Format(appleStrUnformattedForm1, 21));
    REQUIRE(Utils::Utf8ToNarrow(appleStr21) == "21 яблоко");

    // existing domain Translation English
    REQUIRE_NOTHROW(localizationManager.SetLocale(LocaleEnUTF8Keyword));
    REQUIRE_NOTHROW(appleStrUnformattedForm1 = Translator::Translate(KMP_TR_DOMAIN_TESTS, "{1} apple", "{1} apples", 1));
    REQUIRE_NOTHROW(appleStrUnformattedForm2 = Translator::Translate(KMP_TR_DOMAIN_TESTS, "{1} apple", "{1} apples", 2));
    REQUIRE_NOTHROW(appleStrUnformattedForm3 = Translator::Translate(KMP_TR_DOMAIN_TESTS, "{1} apple", "{1} apples", 5));
    REQUIRE_FALSE(appleStrUnformattedForm1.empty());
    REQUIRE_FALSE(appleStrUnformattedForm2.empty());
    REQUIRE_FALSE(appleStrUnformattedForm3.empty());
    REQUIRE_NOTHROW(appleStr1 = Translator::Format(appleStrUnformattedForm1, 1));
    REQUIRE(appleStr1 == "1 apple");
    REQUIRE_NOTHROW(appleStr2 = Translator::Format(appleStrUnformattedForm2, 2));
    REQUIRE(appleStr2 == "2 apples");
    REQUIRE_NOTHROW(appleStr5 = Translator::Format(appleStrUnformattedForm3, 5));
    REQUIRE(appleStr5 == "5 apples");
    REQUIRE_NOTHROW(appleStr11 = Translator::Format(appleStrUnformattedForm3, 11));
    REQUIRE(appleStr11 == "11 apples");
    REQUIRE_NOTHROW(appleStr0 = Translator::Format(appleStrUnformattedForm3, 0));
    REQUIRE(appleStr0 == "0 apples");
    REQUIRE_NOTHROW(appleStr21 = Translator::Format(appleStrUnformattedForm2, 21));
    REQUIRE(appleStr21 == "21 apples");
}
//--------------------------------------------------------------------------

TEST_CASE("Localization manager dictionaries without context plural", "[localization][locale]")
{
    REQUIRE(Filesystem::Initialize());

    LocalizationManager localizationManager;
    REQUIRE_NOTHROW(localizationManager.SetLocale(LocaleRuUTF8Keyword));
    const auto defaultTranslationsPath = Filesystem::ToGenericU8String(Filesystem::GetApplicationPath().append(LocalesDirectory));
    REQUIRE_NOTHROW(localizationManager.AddMessagesPath(defaultTranslationsPath));
    bool ok = false;
    REQUIRE_NOTHROW(ok = localizationManager.AddMessagesDomain(KMP_TR_DOMAIN_TESTS));
    REQUIRE(ok);

    TranslationStr appleStrUnformatted;
    TranslationStr appleStr0;
    TranslationStr appleStr1;
    TranslationStr appleStr2;
    TranslationStr appleStr5;
    TranslationStr appleStr11;
    TranslationStr appleStr21;

    // create only two forms for russian
    REQUIRE_NOTHROW(localizationManager.Translate(KMP_TR_DOMAIN_TESTS, "{1} apple", "{1} apples", 1));
    REQUIRE_NOTHROW(localizationManager.Translate(KMP_TR_DOMAIN_TESTS, "{1} apple", "{1} apples", 5));

    REQUIRE_NOTHROW(appleStr0 = localizationManager.TranslationFormatted(SidTrDomainTests, "{1} apple"_sid, "{1} apples"_sid, 0));
    REQUIRE(Utils::Utf8ToNarrow(appleStr0) == "0 яблок");
    REQUIRE_NOTHROW(appleStrUnformatted = localizationManager.Translation(SidTrDomainTests, "{1} apple"_sid, "{1} apples"_sid, 0));
    REQUIRE_NOTHROW(appleStr0 = Translator::Format(appleStrUnformatted, 0));
    REQUIRE(Utils::Utf8ToNarrow(appleStr0) == "0 яблок");

    REQUIRE_NOTHROW(appleStr5 = localizationManager.TranslationFormatted(SidTrDomainTests, "{1} apple"_sid, "{1} apples"_sid, 5));
    REQUIRE(Utils::Utf8ToNarrow(appleStr5) == "5 яблок");
    REQUIRE_NOTHROW(appleStrUnformatted = localizationManager.Translation(SidTrDomainTests, "{1} apple"_sid, "{1} apples"_sid, 5));
    REQUIRE_NOTHROW(appleStr5 = Translator::Format(appleStrUnformatted, 5));
    REQUIRE(Utils::Utf8ToNarrow(appleStr5) == "5 яблок");

    REQUIRE_NOTHROW(appleStr11 = localizationManager.TranslationFormatted(SidTrDomainTests, "{1} apple"_sid, "{1} apples"_sid, 11));
    REQUIRE(Utils::Utf8ToNarrow(appleStr11) == "11 яблок");
    REQUIRE_NOTHROW(appleStrUnformatted = localizationManager.Translation(SidTrDomainTests, "{1} apple"_sid, "{1} apples"_sid, 11));
    REQUIRE_NOTHROW(appleStr11 = Translator::Format(appleStrUnformatted, 11));
    REQUIRE(Utils::Utf8ToNarrow(appleStr11) == "11 яблок");

    REQUIRE_NOTHROW(appleStr1 = localizationManager.TranslationFormatted(SidTrDomainTests, "{1} apple"_sid, "{1} apples"_sid, 1));
    REQUIRE(Utils::Utf8ToNarrow(appleStr1) == "1 яблоко");
    REQUIRE_NOTHROW(appleStrUnformatted = localizationManager.Translation(SidTrDomainTests, "{1} apple"_sid, "{1} apples"_sid, 1));
    REQUIRE_NOTHROW(appleStr1 = Translator::Format(appleStrUnformatted, 1));
    REQUIRE(Utils::Utf8ToNarrow(appleStr1) == "1 яблоко");

    REQUIRE_NOTHROW(appleStr21 = localizationManager.TranslationFormatted(SidTrDomainTests, "{1} apple"_sid, "{1} apples"_sid, 21));
    REQUIRE(Utils::Utf8ToNarrow(appleStr21) == "21 яблоко");
    REQUIRE_NOTHROW(appleStrUnformatted = localizationManager.Translation(SidTrDomainTests, "{1} apple"_sid, "{1} apples"_sid, 21));
    REQUIRE_NOTHROW(appleStr21 = Translator::Format(appleStrUnformatted, 21));
    REQUIRE(Utils::Utf8ToNarrow(appleStr21) == "21 яблоко");

    // try first plural form that not added
    REQUIRE_NOTHROW(appleStr2 = localizationManager.TranslationFormatted(SidTrDomainTests, "{1} apple"_sid, "{1} apples"_sid, 2));
    REQUIRE(appleStr2.empty());
    REQUIRE_NOTHROW(appleStrUnformatted = localizationManager.Translation(SidTrDomainTests, "{1} apple"_sid, "{1} apples"_sid, 2));
    REQUIRE_NOTHROW(appleStr2 = Translator::Format(appleStrUnformatted, 2));
    REQUIRE(appleStr2.empty());

    // add first plural russian
    REQUIRE_NOTHROW(localizationManager.Translate(KMP_TR_DOMAIN_TESTS, "{1} apple", "{1} apples", 2));

    // try first plural russian form again
    REQUIRE_NOTHROW(appleStr2 = localizationManager.TranslationFormatted(SidTrDomainTests, "{1} apple"_sid, "{1} apples"_sid, 2));
    REQUIRE(Utils::Utf8ToNarrow(appleStr2) == "2 яблока");
    REQUIRE_NOTHROW(appleStrUnformatted = localizationManager.Translation(SidTrDomainTests, "{1} apple"_sid, "{1} apples"_sid, 2));
    REQUIRE_NOTHROW(appleStr2 = Translator::Format(appleStrUnformatted, 2));
    REQUIRE(Utils::Utf8ToNarrow(appleStr2) == "2 яблока");

    // try with english locale
    REQUIRE_NOTHROW(localizationManager.SetLocale(LocaleEnUTF8Keyword));
    REQUIRE_NOTHROW(localizationManager.Translate(KMP_TR_DOMAIN_TESTS, "{1} apple", "{1} apples", 1));
    REQUIRE_NOTHROW(localizationManager.Translate(KMP_TR_DOMAIN_TESTS, "{1} apple", "{1} apples", 5));

    REQUIRE_NOTHROW(appleStr0 = localizationManager.TranslationFormatted(SidTrDomainTests, "{1} apple"_sid, "{1} apples"_sid, 0));
    REQUIRE(appleStr0 == "0 apples");
    REQUIRE_NOTHROW(appleStrUnformatted = localizationManager.Translation(SidTrDomainTests, "{1} apple"_sid, "{1} apples"_sid, 0));
    REQUIRE_NOTHROW(appleStr0 = Translator::Format(appleStrUnformatted, 0));
    REQUIRE(appleStr0 == "0 apples");

    REQUIRE_NOTHROW(appleStr1 = localizationManager.TranslationFormatted(SidTrDomainTests, "{1} apple"_sid, "{1} apples"_sid, 1));
    REQUIRE(appleStr1 == "1 apple");
    REQUIRE_NOTHROW(appleStrUnformatted = localizationManager.Translation(SidTrDomainTests, "{1} apple"_sid, "{1} apples"_sid, 1));
    REQUIRE_NOTHROW(appleStr1 = Translator::Format(appleStrUnformatted, 1));
    REQUIRE(appleStr1 == "1 apple");

    REQUIRE_NOTHROW(appleStr11 = localizationManager.TranslationFormatted(SidTrDomainTests, "{1} apple"_sid, "{1} apples"_sid, 11));
    REQUIRE(appleStr11 == "11 apples");
    REQUIRE_NOTHROW(appleStrUnformatted = localizationManager.Translation(SidTrDomainTests, "{1} apple"_sid, "{1} apples"_sid, 11));
    REQUIRE_NOTHROW(appleStr11 = Translator::Format(appleStrUnformatted, 11));
    REQUIRE(appleStr11 == "11 apples");

    // test after remove "Tests" domain
    REQUIRE_NOTHROW(ok = localizationManager.RemoveMessagesDomain(KMP_TR_DOMAIN_TESTS));
    REQUIRE(ok);

    REQUIRE_NOTHROW(appleStr0 = localizationManager.TranslationFormatted(SidTrDomainTests, "{1} apple"_sid, "{1} apples"_sid, 0));
    REQUIRE(appleStr0.empty());
    REQUIRE_NOTHROW(appleStrUnformatted = localizationManager.Translation(SidTrDomainTests, "{1} apple"_sid, "{1} apples"_sid, 0));
    REQUIRE_NOTHROW(appleStr0 = Translator::Format(appleStrUnformatted, 0));
    REQUIRE(appleStr0.empty());
}
//--------------------------------------------------------------------------