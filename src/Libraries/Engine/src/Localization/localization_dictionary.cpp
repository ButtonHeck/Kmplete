#include "Kmplete/Localization/localization_dictionary.h"
#include "Kmplete/Log/log.h"
#include "Kmplete/Core/assertion.h"


namespace Kmplete
{
    LocalizationDictionary::LocalizationDictionary(const DomainStrSID& domain, const LocaleStrSID& localeSid /*= SidTrInvalidLocale*/) noexcept
        : KMP_PROFILE_CONSTRUCTOR_START_BASE_CLASS()
          _domain(domain)
        , _currentLocaleSid(localeSid)
    {
        KMP_PROFILE_CONSTRUCTOR_END()
    }
    //--------------------------------------------------------------------------

    const DomainStrSID& LocalizationDictionary::GetDomain() const
    {
        return _domain;
    }
    //--------------------------------------------------------------------------

    void LocalizationDictionary::SetLocale(const LocaleStrSID& localeSid)
    {
        _currentLocaleSid = localeSid;
    }
    //--------------------------------------------------------------------------

    void LocalizationDictionary::Add(const SourceStrSID& sourceSid, const TranslationStr& translation)
    {
        KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

        KMP_ASSERT(_currentLocaleSid != SidTrInvalidLocale);
        auto& currentLocaleTranslations = _translationMap[_currentLocaleSid];
        if (currentLocaleTranslations.contains(sourceSid))
        {
            if (currentLocaleTranslations[sourceSid] != translation)
            {
                KMP_LOG_WARN("\"{}\" possible duplicate \"{}\"", _domain, sourceSid);
            }
        }
        else
        {
            currentLocaleTranslations.insert(std::make_pair(sourceSid, translation));
        }
    }
    //--------------------------------------------------------------------------

    void LocalizationDictionary::Add(const SourceStrSID& sourceSidSingular, const SourceStrSID& sourceSidPlural, PluralityForm pluralityForm, const TranslationStr& translation)
    {
        KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

        KMP_ASSERT(_currentLocaleSid != SidTrInvalidLocale);
        const PluralSource&& pluralSource = PluralSource{ sourceSidSingular, sourceSidPlural };
        auto& currentLocaleTranslations = _translationPluralMap[_currentLocaleSid]; 
        const auto isPluralSourceAdded = currentLocaleTranslations.contains(pluralSource);
        if (isPluralSourceAdded && 
            !currentLocaleTranslations[pluralSource][pluralityForm].empty() && 
            currentLocaleTranslations[pluralSource][pluralityForm] != translation)
        {
            KMP_LOG_WARN("\"{}\" possible duplicate \"{}\"/\"{}\" (plural form \"{}\")",
                          _domain, sourceSidSingular, sourceSidPlural, static_cast<int>(pluralityForm));
            return;
        }

        if (!isPluralSourceAdded)
        {
            PluralTranslations pluralTranslations;
            pluralTranslations[pluralityForm] = translation;
            currentLocaleTranslations.insert(std::make_pair(pluralSource, pluralTranslations));
        }
        else
        {
            currentLocaleTranslations[pluralSource][pluralityForm] = translation;
        }
    }
    //--------------------------------------------------------------------------

    void LocalizationDictionary::Add(const SourceStrSID& sourceSid, const ContextStrSID& contextSid, const TranslationStr& translation)
    {
        KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

        KMP_ASSERT(_currentLocaleSid != SidTrInvalidLocale);
        const ContextedSource&& contextedSource = ContextedSource{ sourceSid, contextSid };
        auto& currentLocaleTranslations = _translationCtxMap[_currentLocaleSid]; 
        if (currentLocaleTranslations.contains(contextedSource))
        {
            if (currentLocaleTranslations[contextedSource] != translation)
            {
                KMP_LOG_WARN("\"{}\" possible duplicate \"{}\" (context \"{}\")", _domain, sourceSid, contextSid);
            }
        }
        else
        {
            currentLocaleTranslations.insert(std::make_pair(contextedSource, translation));
        }
    }
    //--------------------------------------------------------------------------

    void LocalizationDictionary::Add(const SourceStrSID& sourceSidSingular, const SourceStrSID& sourceSidPlural, PluralityForm pluralityForm,
                                     const ContextStrSID& contextSid, const TranslationStr& translation)
    {
        KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

        KMP_ASSERT(_currentLocaleSid != SidTrInvalidLocale);
        const ContextedPluralSource&& contextedPluralSource = ContextedPluralSource{ sourceSidSingular, sourceSidPlural, contextSid };
        auto& currentLocaleTranslations = _translationCtxPluralMap[_currentLocaleSid];
        const auto isContextedPluralSourceAdded = currentLocaleTranslations.contains(contextedPluralSource); 
        if (isContextedPluralSourceAdded &&
            !currentLocaleTranslations[contextedPluralSource][pluralityForm].empty() &&
            currentLocaleTranslations[contextedPluralSource][pluralityForm] != translation)
        {
            KMP_LOG_WARN("\"{}\" possible duplicate \"{}\"/\"{}\" (context \"{}\" plural form \"{}\")",
                          _domain, sourceSidSingular, sourceSidPlural, contextSid, static_cast<int>(pluralityForm));
            return;
        }

        if (!isContextedPluralSourceAdded)
        {
            PluralTranslations pluralTranslations;
            pluralTranslations[pluralityForm] = translation;
            currentLocaleTranslations.insert(std::make_pair(contextedPluralSource, pluralTranslations));
        }
        else
        {
            currentLocaleTranslations[contextedPluralSource][pluralityForm] = translation;
        }
    }
    //--------------------------------------------------------------------------

    const TranslationStr& LocalizationDictionary::Get(const SourceStrSID& sourceSid)
    {
        KMP_PROFILE_FUNCTION(ProfileLevelMinorFunctionsVerbose);

        KMP_ASSERT(_currentLocaleSid != SidTrInvalidLocale);
        return _translationMap[_currentLocaleSid][sourceSid];
    }
    //--------------------------------------------------------------------------

    const TranslationStr& LocalizationDictionary::Get(const SourceStrSID& sourceSidSingular, const SourceStrSID& sourceSidPlural, PluralityForm pluralityForm)
    {
        KMP_PROFILE_FUNCTION(ProfileLevelMinorFunctionsVerbose);

        KMP_ASSERT(_currentLocaleSid != SidTrInvalidLocale);
        return _translationPluralMap[_currentLocaleSid][PluralSource{sourceSidSingular, sourceSidPlural}][pluralityForm];
    }
    //--------------------------------------------------------------------------

    const TranslationStr& LocalizationDictionary::Get(const SourceStrSID& sourceSid, const ContextStrSID& contextSid)
    {
        KMP_PROFILE_FUNCTION(ProfileLevelMinorFunctionsVerbose);

        KMP_ASSERT(_currentLocaleSid != SidTrInvalidLocale);
        return _translationCtxMap[_currentLocaleSid][ContextedSource{sourceSid, contextSid}];
    }
    //--------------------------------------------------------------------------

    const TranslationStr& LocalizationDictionary::Get(const SourceStrSID& sourceSidSingular, const SourceStrSID& sourceSidPlural,
                                                      PluralityForm pluralityForm, const ContextStrSID& contextSid)
    {
        KMP_PROFILE_FUNCTION(ProfileLevelMinorFunctionsVerbose);

        KMP_ASSERT(_currentLocaleSid != SidTrInvalidLocale);
        return _translationCtxPluralMap[_currentLocaleSid][ContextedPluralSource{sourceSidSingular, sourceSidPlural, contextSid}][pluralityForm];
    }
    //--------------------------------------------------------------------------
}