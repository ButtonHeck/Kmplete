#include "Kmplete/Localization/localization_dictionary.h"
#include "Kmplete/Log/log.h"
#include "Kmplete/Core/assertion.h"

namespace Kmplete
{
    LocalizationDictionary::LocalizationDictionary(const DomainStrSID& domain, const LocaleStrSID& localeSid /*= SidTrInvalidLocale*/) noexcept
        :
#if defined(KMP_PROFILE)
        _constructorProfilerTimer(CreateUPtr<ProfilerTimer>("LocalizationDictionary::LocalizationDictionary(const DomainStrSID&, const LocaleStrSID&)")),
#endif
        _domain(domain)
        , _currentLocaleSid(localeSid)
    {
#if defined(KMP_PROFILE)
        _constructorProfilerTimer.reset(nullptr);
#endif
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
        KMP_PROFILE_FUNCTION();

        KMP_ASSERT(_currentLocaleSid != SidTrInvalidLocale);
        if (_translationMap[_currentLocaleSid].contains(sourceSid))
        {
            if (_translationMap[_currentLocaleSid][sourceSid] != translation)
            {
                KMP_LOG_WARN("LocalizationDictionary: \"{}\" possible duplicate \"{}\"", _domain, sourceSid);
            }
        }
        else
        {
            _translationMap[_currentLocaleSid].insert(std::make_pair(sourceSid, translation));
        }
    }
    //--------------------------------------------------------------------------

    void LocalizationDictionary::Add(const SourceStrSID& sourceSidSingular, const SourceStrSID& sourceSidPlural, PluralityForm pluralityForm, const TranslationStr& translation)
    {
        KMP_PROFILE_FUNCTION();

        KMP_ASSERT(_currentLocaleSid != SidTrInvalidLocale);
        const PluralSource&& pluralSource = PluralSource{ sourceSidSingular, sourceSidPlural };
        if (_translationPluralMap[_currentLocaleSid].contains(pluralSource) && 
            !_translationPluralMap[_currentLocaleSid][pluralSource][pluralityForm].empty() && 
            _translationPluralMap[_currentLocaleSid][pluralSource][pluralityForm] != translation)
        {
            KMP_LOG_WARN("LocalizationDictionary: \"{}\" possible duplicate \"{}\"/\"{}\" (plural form \"{}\")",
                          _domain, sourceSidSingular, sourceSidPlural, static_cast<int>(pluralityForm));
            return;
        }

        if (!_translationPluralMap[_currentLocaleSid].contains(pluralSource))
        {
            PluralTranslations pluralTranslations;
            pluralTranslations[pluralityForm] = translation;
            _translationPluralMap[_currentLocaleSid].insert(std::make_pair(pluralSource, pluralTranslations));
        }
        else
        {
            _translationPluralMap[_currentLocaleSid][pluralSource][pluralityForm] = translation;
        }
    }
    //--------------------------------------------------------------------------

    void LocalizationDictionary::Add(const SourceStrSID& sourceSid, const ContextStrSID& contextSid, const TranslationStr& translation)
    {
        KMP_PROFILE_FUNCTION();

        KMP_ASSERT(_currentLocaleSid != SidTrInvalidLocale);
        const ContextedSource&& contextedSource = ContextedSource{ sourceSid, contextSid };
        if (_translationCtxMap[_currentLocaleSid].contains(contextedSource))
        {
            if (_translationCtxMap[_currentLocaleSid][contextedSource] != translation)
            {
                KMP_LOG_WARN("LocalizationDictionary: \"{}\" possible duplicate \"{}\" (context \"{}\")", _domain, sourceSid, contextSid);
            }
        }
        else
        {
            _translationCtxMap[_currentLocaleSid].insert(std::make_pair(contextedSource, translation));
        }
    }
    //--------------------------------------------------------------------------

    void LocalizationDictionary::Add(const SourceStrSID& sourceSidSingular, const SourceStrSID& sourceSidPlural, PluralityForm pluralityForm,
                                     const ContextStrSID& contextSid, const TranslationStr& translation)
    {
        KMP_PROFILE_FUNCTION();

        KMP_ASSERT(_currentLocaleSid != SidTrInvalidLocale);
        const ContextedPluralSource&& contextedPluralSource = ContextedPluralSource{ sourceSidSingular, sourceSidPlural, contextSid };
        if (_translationCtxPluralMap[_currentLocaleSid].contains(contextedPluralSource) &&
            !_translationCtxPluralMap[_currentLocaleSid][contextedPluralSource][pluralityForm].empty() &&
            _translationCtxPluralMap[_currentLocaleSid][contextedPluralSource][pluralityForm] != translation)
        {
            KMP_LOG_WARN("LocalizationDictionary: \"{}\" possible duplicate \"{}\"/\"{}\" (context \"{}\" plural form \"{}\")",
                          _domain, sourceSidSingular, sourceSidPlural, contextSid, static_cast<int>(pluralityForm));
            return;
        }

        if (!_translationCtxPluralMap[_currentLocaleSid].contains(contextedPluralSource))
        {
            PluralTranslations pluralTranslations;
            pluralTranslations[pluralityForm] = translation;
            _translationCtxPluralMap[_currentLocaleSid].insert(std::make_pair(contextedPluralSource, pluralTranslations));
        }
        else
        {
            _translationCtxPluralMap[_currentLocaleSid][contextedPluralSource][pluralityForm] = translation;
        }
    }
    //--------------------------------------------------------------------------

    const TranslationStr& LocalizationDictionary::Get(const SourceStrSID& sourceSid)
    {
        KMP_PROFILE_FUNCTION();

        return _translationMap[_currentLocaleSid][sourceSid];
    }
    //--------------------------------------------------------------------------

    const TranslationStr& LocalizationDictionary::Get(const SourceStrSID& sourceSidSingular, const SourceStrSID& sourceSidPlural, PluralityForm pluralityForm)
    {
        KMP_PROFILE_FUNCTION();

        return _translationPluralMap[_currentLocaleSid][PluralSource{sourceSidSingular, sourceSidPlural}][pluralityForm];
    }
    //--------------------------------------------------------------------------

    const TranslationStr& LocalizationDictionary::Get(const SourceStrSID& sourceSid, const ContextStrSID& contextSid)
    {
        KMP_PROFILE_FUNCTION();

        return _translationCtxMap[_currentLocaleSid][ContextedSource{sourceSid, contextSid}];
    }
    //--------------------------------------------------------------------------

    const TranslationStr& LocalizationDictionary::Get(const SourceStrSID& sourceSidSingular, const SourceStrSID& sourceSidPlural,
                                                      PluralityForm pluralityForm, const ContextStrSID& contextSid)
    {
        KMP_PROFILE_FUNCTION();

        return _translationCtxPluralMap[_currentLocaleSid][ContextedPluralSource{sourceSidSingular, sourceSidPlural, contextSid}][pluralityForm];
    }
    //--------------------------------------------------------------------------
}