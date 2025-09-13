#include "Kmplete/Localization/localization_base.h"

#include <catch2/catch_test_macros.hpp>


TEST_CASE("Plurality form english", "[localization][locale]")
{
    using namespace Kmplete;
    const auto& pluralityFormDispatcherEn = GetPluralityDispatcher(LocaleEnUTF8Keyword);

    REQUIRE(pluralityFormDispatcherEn.GetPluralityForm(1) == PluralityFormSingular);
    REQUIRE(pluralityFormDispatcherEn.GetPluralityForm(0) == PluralityFormPlural1);
    REQUIRE(pluralityFormDispatcherEn.GetPluralityForm(11) == PluralityFormPlural1);
    REQUIRE(pluralityFormDispatcherEn.GetPluralityForm(5) == PluralityFormPlural1);
    REQUIRE(pluralityFormDispatcherEn.GetPluralityForm(33) == PluralityFormPlural1);
    REQUIRE(pluralityFormDispatcherEn.GetPluralityForm(50) == PluralityFormPlural1);
    REQUIRE(pluralityFormDispatcherEn.GetPluralityForm(100) == PluralityFormPlural1);
    REQUIRE(pluralityFormDispatcherEn.GetPluralityForm(101) == PluralityFormPlural1);
}
//--------------------------------------------------------------------------

TEST_CASE("Plurality form russian", "[localization][locale]")
{
    using namespace Kmplete;
    const auto& pluralityFormDispatcherRu = GetPluralityDispatcher(LocaleRuUTF8Keyword);

    REQUIRE(pluralityFormDispatcherRu.GetPluralityForm(1) == PluralityFormSingular);
    REQUIRE(pluralityFormDispatcherRu.GetPluralityForm(0) == PluralityFormPlural2);
    REQUIRE(pluralityFormDispatcherRu.GetPluralityForm(2) == PluralityFormPlural1);
    REQUIRE(pluralityFormDispatcherRu.GetPluralityForm(3) == PluralityFormPlural1);
    REQUIRE(pluralityFormDispatcherRu.GetPluralityForm(4) == PluralityFormPlural1);
    REQUIRE(pluralityFormDispatcherRu.GetPluralityForm(5) == PluralityFormPlural2);
    REQUIRE(pluralityFormDispatcherRu.GetPluralityForm(10) == PluralityFormPlural2);
    REQUIRE(pluralityFormDispatcherRu.GetPluralityForm(11) == PluralityFormPlural2);
    REQUIRE(pluralityFormDispatcherRu.GetPluralityForm(15) == PluralityFormPlural2);
    REQUIRE(pluralityFormDispatcherRu.GetPluralityForm(20) == PluralityFormPlural2);
    REQUIRE(pluralityFormDispatcherRu.GetPluralityForm(21) == PluralityFormSingular);
    REQUIRE(pluralityFormDispatcherRu.GetPluralityForm(22) == PluralityFormPlural1);
    REQUIRE(pluralityFormDispatcherRu.GetPluralityForm(25) == PluralityFormPlural2);
    REQUIRE(pluralityFormDispatcherRu.GetPluralityForm(100) == PluralityFormPlural2);
    REQUIRE(pluralityFormDispatcherRu.GetPluralityForm(101) == PluralityFormSingular);
}
//--------------------------------------------------------------------------