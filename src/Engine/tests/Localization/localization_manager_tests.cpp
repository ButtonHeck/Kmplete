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

    localizationManager.AddLocaleChangedCallback([&](){ testValue++; });

    // en_EN.UTF-8
    REQUIRE_NOTHROW(localizationManager.SetLocale(LocaleEnUTF8Keyword));
    REQUIRE(testValue == 1);

    int anotherTestValue = 100;
    localizationManager.AddLocaleChangedCallback([&](){ anotherTestValue++; });

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

    // Without domain "Tests" and without "locale" path
    auto yesStr = Translator::Translate(KMP_TR_DOMAIN_TESTS, "Yes");
    REQUIRE(yesStr == "Yes"); // without translation

    // add path
    const auto defaultTranslationsPath = Filesystem::ToGenericU8String(Filesystem::GetApplicationPath().append(LocalesDirectory));
    REQUIRE_NOTHROW(localizationManager.AddMessagesPath(defaultTranslationsPath));
    yesStr = Translator::Translate(KMP_TR_DOMAIN_TESTS, "Yes");
    REQUIRE(yesStr == "Yes"); // without translation

    // add domain (translations must be ready)
    REQUIRE_NOTHROW(localizationManager.AddMessagesDomain(KMP_TR_DOMAIN_TESTS));
    yesStr = Translator::Translate(KMP_TR_DOMAIN_TESTS, "Yes");
    REQUIRE(Utils::Utf8ToNarrow(yesStr) == "Да");
    auto noStr = Translator::Translate(KMP_TR_DOMAIN_TESTS, "No");
    REQUIRE(Utils::Utf8ToNarrow(noStr) == "Нет");
    auto untranslatedStr = Translator::Translate(KMP_TR_DOMAIN_TESTS, "Untranslated");
    REQUIRE(untranslatedStr == "Untranslated");

    // switch to en_EN
    REQUIRE(localizationManager.SetLocale(LocaleEnUTF8Keyword));
    yesStr = Translator::Translate(KMP_TR_DOMAIN_TESTS, "Yes");
    REQUIRE(yesStr == "Yes");
    noStr = Translator::Translate(KMP_TR_DOMAIN_TESTS, "No");
    REQUIRE(noStr == "No");
    untranslatedStr = Translator::Translate(KMP_TR_DOMAIN_TESTS, "Untranslated");
    REQUIRE(untranslatedStr == "Untranslated");

    // switch to ru_RU again
    REQUIRE_NOTHROW(localizationManager.SetLocale(LocaleRuUTF8Keyword));
    yesStr = Translator::Translate(KMP_TR_DOMAIN_TESTS, "Yes");
    REQUIRE(Utils::Utf8ToNarrow(yesStr) == "Да");
    noStr = Translator::Translate(KMP_TR_DOMAIN_TESTS, "No");
    REQUIRE(Utils::Utf8ToNarrow(noStr) == "Нет");
    untranslatedStr = Translator::Translate(KMP_TR_DOMAIN_TESTS, "Untranslated");
    REQUIRE(untranslatedStr == "Untranslated");
}
//--------------------------------------------------------------------------

TEST_CASE("Localization translator with context singular", "[localization][locale]")
{
    REQUIRE(Filesystem::Initialize());

    LocalizationManager localizationManager;
    REQUIRE_NOTHROW(localizationManager.SetLocale(LocaleRuUTF8Keyword));

    std::string openVerbStr;
    std::string openAdjStr;
    std::string untranslatedStr;

    // Without domain "Tests" and without messages path
    openVerbStr = Translator::TranslateCtx(KMP_TR_DOMAIN_TESTS, "Open", "Verb");
    REQUIRE(openVerbStr == "Open");

    // add path
    const auto defaultTranslationsPath = Filesystem::ToGenericU8String(Filesystem::GetApplicationPath().append(LocalesDirectory));
    REQUIRE_NOTHROW(localizationManager.AddMessagesPath(defaultTranslationsPath));
    openVerbStr = Translator::TranslateCtx(KMP_TR_DOMAIN_TESTS, "Open", "Verb");
    REQUIRE(openVerbStr == "Open");

    // add domain "Tests"
    REQUIRE_NOTHROW(localizationManager.AddMessagesDomain(KMP_TR_DOMAIN_TESTS));
    openVerbStr = Translator::TranslateCtx(KMP_TR_DOMAIN_TESTS, "Open", "Verb");
    REQUIRE(Utils::Utf8ToNarrow(openVerbStr) == "Открыть");
    openAdjStr = Translator::TranslateCtx(KMP_TR_DOMAIN_TESTS, "Open", "Adjective");
    REQUIRE(Utils::Utf8ToNarrow(openAdjStr) == "Открытый");
    untranslatedStr = Translator::TranslateCtx(KMP_TR_DOMAIN_TESTS, "Untranslated", "Some context");
    REQUIRE(untranslatedStr == "Untranslated");

    // switch to en_EN
    REQUIRE(localizationManager.SetLocale(LocaleEnUTF8Keyword));
    openVerbStr = Translator::TranslateCtx(KMP_TR_DOMAIN_TESTS, "Open", "Verb");
    REQUIRE(openVerbStr == "Open");
    openAdjStr = Translator::TranslateCtx(KMP_TR_DOMAIN_TESTS, "Open", "Adjective");
    REQUIRE(openAdjStr == "Open");
    untranslatedStr = Translator::TranslateCtx(KMP_TR_DOMAIN_TESTS, "Untranslated", "Some context");
    REQUIRE(untranslatedStr == "Untranslated");

    // switch to ru_RU again
    REQUIRE(localizationManager.SetLocale(LocaleRuUTF8Keyword));
    openVerbStr = Translator::TranslateCtx(KMP_TR_DOMAIN_TESTS, "Open", "Verb");
    REQUIRE(Utils::Utf8ToNarrow(openVerbStr) == "Открыть");
    openAdjStr = Translator::TranslateCtx(KMP_TR_DOMAIN_TESTS, "Open", "Adjective");
    REQUIRE(Utils::Utf8ToNarrow(openAdjStr) == "Открытый");
    untranslatedStr = Translator::TranslateCtx(KMP_TR_DOMAIN_TESTS, "Untranslated", "Some context");
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
    Ptr<LocalizationDictionary> dict;
    REQUIRE_NOTHROW(dict = localizationManager.AddMessagesDomain(KMP_TR_DOMAIN_TESTS));
    REQUIRE(dict);

    REQUIRE_NOTHROW(dict->Add("Yes"_sid, Translator::Translate(KMP_TR_DOMAIN_TESTS, "Yes")));
    REQUIRE_NOTHROW(dict->Add("No"_sid, Translator::Translate(KMP_TR_DOMAIN_TESTS, "No")));
    REQUIRE_NOTHROW(dict->Add("Untranslated"_sid, Translator::Translate(KMP_TR_DOMAIN_TESTS, "Untranslated")));

    std::string yesStr;
    std::string noStr;
    std::string untranslatedStr;
    std::string unknownStr;

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

    std::string openVerbStr;
    std::string openAdjStr;
    std::string untranslatedStr;
    std::string unknownStr;

    LocalizationManager localizationManager;
    REQUIRE_NOTHROW(localizationManager.SetLocale(LocaleRuUTF8Keyword));
    const auto defaultTranslationsPath = Filesystem::ToGenericU8String(Filesystem::GetApplicationPath().append(LocalesDirectory));
    REQUIRE_NOTHROW(localizationManager.AddMessagesPath(defaultTranslationsPath));
    Ptr<LocalizationDictionary> dict;
    REQUIRE_NOTHROW(dict = localizationManager.AddMessagesDomain(KMP_TR_DOMAIN_TESTS));
    REQUIRE(dict);

    // add existing translations with existing context
    REQUIRE_NOTHROW(dict->Add("Open"_sid, "Verb"_sid, Translator::TranslateCtx(KMP_TR_DOMAIN_TESTS, "Open", "Verb")));
    REQUIRE_NOTHROW(dict->Add("Open"_sid, "Adjective"_sid, Translator::TranslateCtx(KMP_TR_DOMAIN_TESTS, "Open", "Adjective")));
    REQUIRE_NOTHROW(dict->Add("Untranslated"_sid, "Some context"_sid, Translator::TranslateCtx(KMP_TR_DOMAIN_TESTS, "Untranslated", "Some context")));

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

TEST_CASE("Localization manager dictionaries without context", "[localization][locale]")
{
    REQUIRE(Filesystem::Initialize());

    std::string yesStr;
    std::string noStr;
    std::string untranslatedStr;
    std::string unknownStr;

    LocalizationManager localizationManager;
    REQUIRE_NOTHROW(localizationManager.SetLocale(LocaleRuUTF8Keyword));
    auto dict = localizationManager.GetDictionary(KMP_TR_DOMAIN_TESTS);
    REQUIRE(dict == nullptr);

    REQUIRE_NOTHROW(dict = localizationManager.AddMessagesDomain(KMP_TR_DOMAIN_TESTS));
    REQUIRE(dict);
    REQUIRE(dict->GetDomain() != 0);

    // try add "tests" domain again
    Ptr<LocalizationDictionary> anotherDict = nullptr;
    REQUIRE_NOTHROW(anotherDict = localizationManager.AddMessagesDomain(KMP_TR_DOMAIN_TESTS));
    REQUIRE(anotherDict == dict);

    REQUIRE_NOTHROW(dict = localizationManager.GetDictionary(KMP_TR_DOMAIN_TESTS));
    REQUIRE(dict);

    // check double remove
    REQUIRE_NOTHROW(localizationManager.RemoveMessagesDomain(KMP_TR_DOMAIN_TESTS));
    REQUIRE_NOTHROW(localizationManager.RemoveMessagesDomain(KMP_TR_DOMAIN_TESTS));

    REQUIRE_NOTHROW(dict = localizationManager.GetDictionary(KMP_TR_DOMAIN_TESTS));
    REQUIRE(dict == nullptr);

    // add again
    REQUIRE_NOTHROW(dict = localizationManager.AddMessagesDomain(KMP_TR_DOMAIN_TESTS));
    REQUIRE(dict);

    // try add without messages path set
    REQUIRE_NOTHROW(dict->Add("Yes"_sid, Translator::Translate(KMP_TR_DOMAIN_TESTS, "Yes")));
    REQUIRE_NOTHROW(dict->Add("No"_sid, Translator::Translate(KMP_TR_DOMAIN_TESTS, "No")));
    REQUIRE_NOTHROW(yesStr = dict->Get("Yes"_sid));
    REQUIRE_NOTHROW(noStr = dict->Get("No"_sid));
    REQUIRE(yesStr == "Yes");
    REQUIRE(noStr == "No");

    // try add again after messages path set
    const auto defaultTranslationsPath = Filesystem::ToGenericU8String(Filesystem::GetApplicationPath().append(LocalesDirectory));
    REQUIRE_NOTHROW(localizationManager.AddMessagesPath(defaultTranslationsPath));
    REQUIRE_NOTHROW(localizationManager.RemoveMessagesDomain(KMP_TR_DOMAIN_TESTS));
    REQUIRE_NOTHROW(dict = localizationManager.AddMessagesDomain(KMP_TR_DOMAIN_TESTS));
    REQUIRE_NOTHROW(dict->Add("Yes"_sid, Translator::Translate(KMP_TR_DOMAIN_TESTS, "Yes")));
    REQUIRE_NOTHROW(dict->Add("No"_sid, Translator::Translate(KMP_TR_DOMAIN_TESTS, "No")));
    REQUIRE_NOTHROW(dict->Add("Untranslated"_sid, Translator::Translate(KMP_TR_DOMAIN_TESTS, "Untranslated")));
    REQUIRE_NOTHROW(dict->Add("Unknown"_sid, Translator::Translate(KMP_TR_DOMAIN_TESTS, "Unknown")));
    REQUIRE_NOTHROW(yesStr = dict->Get("Yes"_sid));
    REQUIRE_NOTHROW(noStr = dict->Get("No"_sid));
    REQUIRE_NOTHROW(untranslatedStr = dict->Get("Untranslated"_sid));
    REQUIRE_NOTHROW(unknownStr = dict->Get("Unknown"_sid));
    REQUIRE(Utils::Utf8ToNarrow(yesStr) == "Да");
    REQUIRE(Utils::Utf8ToNarrow(noStr) == "Нет");
    REQUIRE(untranslatedStr == "Untranslated");
    REQUIRE(unknownStr == "Unknown");

    // try get after remove "Tests" domain (via existing ptr to dictionary)
    REQUIRE_NOTHROW(localizationManager.RemoveMessagesDomain(KMP_TR_DOMAIN_TESTS));
    REQUIRE_NOTHROW(yesStr = dict->Get("Yes"_sid));
    REQUIRE_NOTHROW(noStr = dict->Get("No"_sid));
    REQUIRE(yesStr.empty());
    REQUIRE(noStr.empty());
}
//--------------------------------------------------------------------------

TEST_CASE("Localization manager dictionaries with context", "[localization][locale]")
{
    REQUIRE(Filesystem::Initialize());

    LocalizationManager localizationManager;
    REQUIRE_NOTHROW(localizationManager.SetLocale(LocaleRuUTF8Keyword));
    const auto defaultTranslationsPath = Filesystem::ToGenericU8String(Filesystem::GetApplicationPath().append(LocalesDirectory));
    REQUIRE_NOTHROW(localizationManager.AddMessagesPath(defaultTranslationsPath));
    Ptr<LocalizationDictionary> dict = nullptr;
    REQUIRE_NOTHROW(dict = localizationManager.AddMessagesDomain(KMP_TR_DOMAIN_TESTS));
    REQUIRE(dict);

    std::string openVerbStr;
    std::string openAdjStr;
    std::string untranslatedStr;
    std::string unknownStr;

    // try add valid sources but invalid context
    REQUIRE_NOTHROW(dict->Add("Open"_sid, "VERB"_sid, Translator::TranslateCtx(KMP_TR_DOMAIN_TESTS, "Open", "VERB")));
    REQUIRE_NOTHROW(openVerbStr = dict->Get("Open"_sid, "VERB"_sid));
    REQUIRE(openVerbStr == "Open");
    REQUIRE_NOTHROW(openVerbStr = dict->Get("Open"_sid, "Verb"_sid));
    REQUIRE(openVerbStr.empty());
    REQUIRE_NOTHROW(openVerbStr = dict->Get("OPEN"_sid, "Verb"_sid));
    REQUIRE(openVerbStr.empty());
    REQUIRE_NOTHROW(dict->Clear());

    // try add invalid sources and invalid context
    REQUIRE_NOTHROW(dict->Add("OPEN"_sid, "VERB"_sid, Translator::TranslateCtx(KMP_TR_DOMAIN_TESTS, "OPEN", "VERB")));
    REQUIRE_NOTHROW(openVerbStr = dict->Get("OPEN"_sid, "VERB"_sid));
    REQUIRE(openVerbStr == "OPEN");
}
//--------------------------------------------------------------------------