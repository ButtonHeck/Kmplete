#include "Kmplete/Localization/localization_library.h"
#include "Kmplete/Localization/localization_dictionary.h"
#include "Kmplete/Log/log.h"

namespace Kmplete
{
    static const auto NoTranslation = TranslationStr("");

    LocalizationLibrary::LocalizationLibrary() noexcept
        : KMP_PROFILE_CONSTRUCTOR_START_BASE_CLASS("LocalizationLibrary::LocalizationLibrary() noexcept")
          _currentLocaleSid(SidTrInvalidLocale)
    {
        KMP_PROFILE_CONSTRUCTOR_END()
    }
    //--------------------------------------------------------------------------

    void LocalizationLibrary::SetLocale(const LocaleStrSID& localeSid) noexcept
    {
        KMP_PROFILE_FUNCTION();

        if (_currentLocaleSid != localeSid)
        {
            _currentLocaleSid = localeSid;
            for (auto& [domain, dictionary] : _dictionaryMap)
            {
                dictionary->SetLocale(localeSid);
            }
        }
    }
    //--------------------------------------------------------------------------

    void LocalizationLibrary::SetLocale(const LocaleStr& locale) noexcept
    {
        KMP_PROFILE_FUNCTION();

        SetLocale(Utils::ToStringID(KMP_SID_PARAM(locale)));
    }
    //--------------------------------------------------------------------------

    bool LocalizationLibrary::AddDictionary(const DomainStrSID& domainSid)
    {
        KMP_PROFILE_FUNCTION();

        if (_dictionaryMap.contains(domainSid))
        {
            KMP_LOG_WARN("already contains domainSID '{}'", domainSid);
            return false;
        }

        _dictionaryMap.insert(std::make_pair(domainSid, CreateUPtr<LocalizationDictionary>(domainSid, _currentLocaleSid)));
        return true;
    }
    //--------------------------------------------------------------------------

    bool LocalizationLibrary::AddDictionary(const DomainStr& domain)
    {
        KMP_PROFILE_FUNCTION();

        const auto domainSid = Utils::ToStringID(KMP_SID_PARAM(domain));
        if (_dictionaryMap.contains(domainSid))
        {
            KMP_LOG_WARN("already contains domain '{}'", domain);
            return false;
        }

        return AddDictionary(domainSid);
    }
    //--------------------------------------------------------------------------

    bool LocalizationLibrary::RemoveDictionary(const DomainStrSID& domainSid) noexcept
    {
        KMP_PROFILE_FUNCTION();

        if (_dictionaryMap.contains(domainSid))
        {
            _dictionaryMap.erase(domainSid);
            return true;
        }
        
        return false;
    }
    //--------------------------------------------------------------------------

    bool LocalizationLibrary::RemoveDictionary(const DomainStr& domain) noexcept
    {
        KMP_PROFILE_FUNCTION();

        return RemoveDictionary(Utils::ToStringID(KMP_SID_PARAM(domain)));
    }
    //--------------------------------------------------------------------------

    void LocalizationLibrary::Add(const DomainStrSID& domainSid, const SourceStrSID& sourceSid, const TranslationStr& translation)
    {
        KMP_PROFILE_FUNCTION();

        if (_dictionaryMap.contains(domainSid))
        {
            _dictionaryMap.at(domainSid)->Add(sourceSid, translation);
        }
    }
    //--------------------------------------------------------------------------

    void LocalizationLibrary::Add(const DomainStrSID& domainSid, const SourceStrSID& sourceSidSingular, const SourceStrSID& sourceSidPlural, 
                                  PluralityForm pluralityForm, const TranslationStr& translation)
    {
        KMP_PROFILE_FUNCTION();

        if (_dictionaryMap.contains(domainSid))
        {
            _dictionaryMap.at(domainSid)->Add(sourceSidSingular, sourceSidPlural, pluralityForm, translation);
        }
    }
    //--------------------------------------------------------------------------

    void LocalizationLibrary::Add(const DomainStrSID& domainSid, const SourceStrSID& sourceSid, const ContextStrSID& contextSid, const TranslationStr& translation)
    {
        KMP_PROFILE_FUNCTION();

        if (_dictionaryMap.contains(domainSid))
        {
            _dictionaryMap.at(domainSid)->Add(sourceSid, contextSid, translation);
        }
    }
    //--------------------------------------------------------------------------

    void LocalizationLibrary::Add(const DomainStrSID& domainSid, const SourceStrSID& sourceSidSingular, const SourceStrSID& sourceSidPlural, 
                                  PluralityForm pluralityForm, const ContextStrSID& contextSid, const TranslationStr& translation)
    {
        KMP_PROFILE_FUNCTION();

        if (_dictionaryMap.contains(domainSid))
        {
            _dictionaryMap.at(domainSid)->Add(sourceSidSingular, sourceSidPlural, pluralityForm, contextSid, translation);
        }
    }
    //--------------------------------------------------------------------------

    const TranslationStr& LocalizationLibrary::Get(const DomainStrSID& domainSid, const SourceStrSID& sourceSid) const
    {
        KMP_PROFILE_FUNCTION();

        if (_dictionaryMap.contains(domainSid))
        {
            return _dictionaryMap.at(domainSid)->Get(sourceSid);
        }

        return NoTranslation;
    }
    //--------------------------------------------------------------------------

    const TranslationStr& LocalizationLibrary::Get(const DomainStrSID& domainSid, const SourceStrSID& sourceSidSingular, 
                                                   const SourceStrSID& sourceSidPlural, PluralityForm pluralityForm) const
    {
        KMP_PROFILE_FUNCTION();

        if (_dictionaryMap.contains(domainSid))
        {
            return _dictionaryMap.at(domainSid)->Get(sourceSidSingular, sourceSidPlural, pluralityForm);
        }

        return NoTranslation;
    }
    //--------------------------------------------------------------------------

    const TranslationStr& LocalizationLibrary::Get(const DomainStrSID& domainSid, const SourceStrSID& sourceSid, const ContextStrSID& contextSid) const
    {
        KMP_PROFILE_FUNCTION();

        if (_dictionaryMap.contains(domainSid))
        {
            return _dictionaryMap.at(domainSid)->Get(sourceSid, contextSid);
        }

        return NoTranslation;
    }
    //--------------------------------------------------------------------------

    const TranslationStr& LocalizationLibrary::Get(const DomainStrSID& domainSid, const SourceStrSID& sourceSidSingular, const SourceStrSID& sourceSidPlural, 
                                                   PluralityForm pluralityForm, const ContextStrSID& contextSid) const
    {
        KMP_PROFILE_FUNCTION();

        if (_dictionaryMap.contains(domainSid))
        {
            return _dictionaryMap.at(domainSid)->Get(sourceSidSingular, sourceSidPlural, pluralityForm, contextSid);
        }

        return NoTranslation;
    }
    //--------------------------------------------------------------------------
}