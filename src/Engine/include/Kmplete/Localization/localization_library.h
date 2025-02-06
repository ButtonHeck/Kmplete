#pragma once

#include "Kmplete/Localization/localization_base.h"
#include "Kmplete/Core/kmplete_api.h"
#include "Kmplete/Core/pointers.h"
#include "Kmplete/Core/nullability.h"

#include <unordered_map>

namespace Kmplete
{
    class LocalizationDictionary;

    class LocalizationLibrary
    {
    public:
        KMP_DISABLE_COPY_MOVE(LocalizationLibrary)

        KMP_API LocalizationLibrary() KMP_NOEXCEPT;

        KMP_API void SetLocale(const LocaleStrSID& localeSid) KMP_NOEXCEPT;
        KMP_API void SetLocale(const LocaleStr& locale) KMP_NOEXCEPT;

        KMP_NODISCARD KMP_API NonNull<Ptr<LocalizationDictionary>> AddDictionary(const DomainStrSID& domainSid);
        KMP_NODISCARD KMP_API NonNull<Ptr<LocalizationDictionary>> AddDictionary(const DomainStr& domain);
        KMP_NODISCARD KMP_API Nullable<Ptr<LocalizationDictionary>> GetDictionary(const DomainStrSID& domainSid) const;
        KMP_NODISCARD KMP_API Nullable<Ptr<LocalizationDictionary>> GetDictionary(const DomainStr& domain) const;
        KMP_API void RemoveDictionary(const DomainStrSID& domainSid) KMP_NOEXCEPT;
        KMP_API void RemoveDictionary(const DomainStr& domain) KMP_NOEXCEPT;

        KMP_API void Add(const DomainStrSID& domainSid, const SourceStrSID& sourceSid, const TranslationStr& translation);
        KMP_API void Add(const DomainStrSID& domainSid, const SourceStrSID& sourceSid, const ContextStrSID& contextSid, const TranslationStr& translation);
        KMP_NODISCARD KMP_API const TranslationStr& Get(const DomainStrSID& domainSid, const SourceStrSID& sourceSid);
        KMP_NODISCARD KMP_API const TranslationStr& Get(const DomainStrSID& domainSid, const SourceStrSID& sourceSid, const ContextStrSID& contextSid);

    private:
        LocaleStrSID _currentLocaleSid;
        std::unordered_map<DomainStrSID, Ptr<LocalizationDictionary>> _dictionaryMap;
    };
    //--------------------------------------------------------------------------
}