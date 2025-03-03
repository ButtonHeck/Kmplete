#include "Kmplete/Localization/localization_base.h"

namespace Kmplete
{
    PluralityForm PluralityFormDispatcherEn::GetPluralityForm(int count) const noexcept
    {
        return count == 1 ? PluralityFormSingular : PluralityFormPlural1;
    }
    //--------------------------------------------------------------------------

    PluralityForm PluralityFormDispatcherRu::GetPluralityForm(int count) const noexcept
    {
        if ((count % 10 == 1) && (count % 100 != 11))
        {
            return PluralityFormSingular;
        }
        else if ((count % 10 >= 2) && (count % 10 <= 4) && ((count % 100 < 10) || (count % 100 >= 20)))
        {
            return PluralityFormPlural1;
        }
        else
        {
            return PluralityFormPlural2;
        }
    }
    //--------------------------------------------------------------------------

    const PluralityFormDispatcher& GetPluralityDispatcher(const LocaleStr& localeStr) noexcept
    {
        if (localeStr == LocaleEnUTF8Keyword)
        {
            return pluralFormIndexGeneratorEn;
        }
        else if (localeStr == LocaleRuUTF8Keyword)
        {
            return pluralFormIndexGeneratorRu;
        }

        return pluralFormIndexGeneratorEn;
    }
    //--------------------------------------------------------------------------
}