#include "Kmplete/Localization/localization_dictionary.h"

namespace Kmplete
{
    LocalizationDictionary::LocalizationDictionary(const DomainStrSID& domain, const LocaleStrSID& localeSid) KMP_NOEXCEPT
        : _domain(domain)
        , _currentLocaleSid(localeSid)
    {}
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
        _translationMap[_currentLocaleSid].insert(std::make_pair(sourceSid, translation));
    }
    //--------------------------------------------------------------------------

    void LocalizationDictionary::Add(const SourceStrSID& sourceSid, const ContextStrSID& contextSid, const TranslationStr& translation)
    {
        _translationCtxMap[_currentLocaleSid].insert(std::make_pair(ContextedSource{ sourceSid, contextSid }, translation));
    }
    //--------------------------------------------------------------------------

    const TranslationStr& LocalizationDictionary::Get(const SourceStrSID& sourceSid)
    {
        return _translationMap[_currentLocaleSid][sourceSid];
    }
    //--------------------------------------------------------------------------

    const TranslationStr& LocalizationDictionary::Get(const SourceStrSID& sourceSid, const ContextStrSID& contextSid)
    {
        return _translationCtxMap[_currentLocaleSid][{sourceSid, contextSid}];
    }
    //--------------------------------------------------------------------------
}