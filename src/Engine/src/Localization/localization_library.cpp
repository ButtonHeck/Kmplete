#include "Kmplete/Localization/localization_library.h"
#include "Kmplete/Localization/localization_dictionary.h"
#include "Kmplete/Core/log.h"

namespace Kmplete
{
    static const auto NoTranslation = TranslationStr("");

    LocalizationLibrary::LocalizationLibrary() noexcept
        : _currentLocaleSid(SidTrInvalidLocale)
    {}
    //--------------------------------------------------------------------------

    void LocalizationLibrary::SetLocale(const LocaleStrSID& localeSid) noexcept
    {
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
        SetLocale(Utils::ToStringID(locale));
    }
    //--------------------------------------------------------------------------

    bool LocalizationLibrary::AddDictionary(const DomainStrSID& domainSid)
    {
        if (_dictionaryMap.contains(domainSid))
        {
            KMP_LOG_CORE_WARN("LocalizationLibrary: already contains domainSID '{}'", domainSid);
            return false;
        }

        _dictionaryMap.insert(std::make_pair(domainSid, CreateUPtr<LocalizationDictionary>(domainSid, _currentLocaleSid)));
        return true;
    }
    //--------------------------------------------------------------------------

    bool LocalizationLibrary::AddDictionary(const DomainStr& domain)
    {
        const auto domainSid = Utils::ToStringID(domain);
        if (_dictionaryMap.contains(domainSid))
        {
            KMP_LOG_CORE_WARN("LocalizationLibrary: already contains domain '{}'", domain);
            return false;
        }

        return AddDictionary(domainSid);
    }
    //--------------------------------------------------------------------------

    bool LocalizationLibrary::RemoveDictionary(const DomainStrSID& domainSid) noexcept
    {
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
        return RemoveDictionary(Utils::ToStringID(domain));
    }
    //--------------------------------------------------------------------------

    void LocalizationLibrary::Add(const DomainStrSID& domainSid, const SourceStrSID& sourceSid, const TranslationStr& translation)
    {
        if (_dictionaryMap.contains(domainSid))
        {
            _dictionaryMap.at(domainSid)->Add(sourceSid, translation);
        }
    }
    //--------------------------------------------------------------------------

    void LocalizationLibrary::Add(const DomainStrSID& domainSid, const SourceStrSID& sourceSid, const ContextStrSID& contextSid, const TranslationStr& translation)
    {
        if (_dictionaryMap.contains(domainSid))
        {
            _dictionaryMap.at(domainSid)->Add(sourceSid, contextSid, translation);
        }
    }
    //--------------------------------------------------------------------------

    const TranslationStr& LocalizationLibrary::Get(const DomainStrSID& domainSid, const SourceStrSID& sourceSid) const
    {
        if (_dictionaryMap.contains(domainSid))
        {
            return _dictionaryMap.at(domainSid)->Get(sourceSid);
        }

        return NoTranslation;
    }
    //--------------------------------------------------------------------------

    const TranslationStr& LocalizationLibrary::Get(const DomainStrSID& domainSid, const SourceStrSID& sourceSid, const ContextStrSID& contextSid) const
    {
        if (_dictionaryMap.contains(domainSid))
        {
            return _dictionaryMap.at(domainSid)->Get(sourceSid, contextSid);
        }

        return NoTranslation;
    }
    //--------------------------------------------------------------------------
}