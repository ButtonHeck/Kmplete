#include "Kmplete/Localization/localization_library.h"
#include "Kmplete/Localization/localization_dictionary.h"

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

    NonNull<Ptr<LocalizationDictionary>> LocalizationLibrary::AddDictionary(const DomainStrSID& domainSid)
    {
        auto dictionary = CreatePtr<LocalizationDictionary>(domainSid, _currentLocaleSid);
        _dictionaryMap.insert(std::make_pair(domainSid, dictionary));
        return dictionary;
    }
    //--------------------------------------------------------------------------

    NonNull<Ptr<LocalizationDictionary>> LocalizationLibrary::AddDictionary(const DomainStr& domain)
    {
        return AddDictionary(Utils::ToStringID(domain));
    }
    //--------------------------------------------------------------------------

    Nullable<Ptr<LocalizationDictionary>> LocalizationLibrary::GetDictionary(const DomainStrSID& domainSid) const
    {
        if (_dictionaryMap.contains(domainSid))
        {
            return _dictionaryMap.at(domainSid);
        }

        return nullptr;
    }
    //--------------------------------------------------------------------------

    Nullable<Ptr<LocalizationDictionary>> LocalizationLibrary::GetDictionary(const DomainStr& domain) const
    {
        return GetDictionary(Utils::ToStringID(domain));
    }
    //--------------------------------------------------------------------------

    void LocalizationLibrary::RemoveDictionary(const DomainStrSID& domainSid) noexcept
    {
        _dictionaryMap.erase(domainSid);
    }
    //--------------------------------------------------------------------------

    void LocalizationLibrary::RemoveDictionary(const DomainStr& domain) noexcept
    {
        RemoveDictionary(Utils::ToStringID(domain));
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