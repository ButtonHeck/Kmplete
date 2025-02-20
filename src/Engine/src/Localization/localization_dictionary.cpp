#include "Kmplete/Localization/localization_dictionary.h"
#include "Kmplete/Core/log.h"
#include "Kmplete/Core/assertion.h"

namespace Kmplete
{
    LocalizationDictionary::LocalizationDictionary(const DomainStrSID& domain, const LocaleStrSID& localeSid) noexcept
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
        KMP_ASSERT(_currentLocaleSid != SidTrInvalidLocale);
        if (_translationMap[_currentLocaleSid].contains(sourceSid) && _translationMap[_currentLocaleSid][sourceSid] != translation)
        {
            KMP_LOG_CORE_WARN("LocalizationDictionary: \"{}\" possible duplicate \"{}\"", _domain, sourceSid);
            return;
        }

        _translationMap[_currentLocaleSid].insert(std::make_pair(sourceSid, translation));
    }
    //--------------------------------------------------------------------------

    void LocalizationDictionary::Add(const SourceStrSID& sourceSid, const ContextStrSID& contextSid, const TranslationStr& translation)
    {
        KMP_ASSERT(_currentLocaleSid != SidTrInvalidLocale);
        const ContextedSource&& contextedSource = ContextedSource{ sourceSid, contextSid };
        if (_translationCtxMap[_currentLocaleSid].contains(contextedSource) && _translationCtxMap[_currentLocaleSid][contextedSource] != translation)
        {
            KMP_LOG_CORE_WARN("LocalizationDictionary: \"{}\" possible duplicate \"{}\" (context \"{}\")", _domain, sourceSid, contextSid);
            return;
        }

        _translationCtxMap[_currentLocaleSid].insert(std::make_pair(contextedSource, translation));
    }
    //--------------------------------------------------------------------------

    const TranslationStr& LocalizationDictionary::Get(const SourceStrSID& sourceSid)
    {
        return _translationMap[_currentLocaleSid][sourceSid];
    }
    //--------------------------------------------------------------------------

    const TranslationStr& LocalizationDictionary::Get(const SourceStrSID& sourceSid, const ContextStrSID& contextSid)
    {
        return _translationCtxMap[_currentLocaleSid][ContextedSource{sourceSid, contextSid}];
    }
    //--------------------------------------------------------------------------

    void LocalizationDictionary::Clear() noexcept
    {
        _translationMap.clear();
        _translationCtxMap.clear();
    }
    //--------------------------------------------------------------------------
}