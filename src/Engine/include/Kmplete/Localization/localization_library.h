#pragma once

#include "Kmplete/Localization/localization_base.h"
#include "Kmplete/Localization/localization_dictionary.h"
#include "Kmplete/Core/kmplete_api.h"
#include "Kmplete/Core/pointers.h"

#include <unordered_map>

namespace Kmplete
{
    class LocalizationLibrary
    {
    public:
        KMP_DISABLE_COPY_MOVE(LocalizationLibrary)

        KMP_API LocalizationLibrary() noexcept;

        KMP_API void SetLocale(const LocaleStrSID& localeSid) noexcept;
        KMP_API void SetLocale(const LocaleStr& locale) noexcept;

        KMP_NODISCARD KMP_API bool AddDictionary(const DomainStrSID& domainSid);
        KMP_NODISCARD KMP_API bool AddDictionary(const DomainStr& domain);
        KMP_NODISCARD KMP_API bool RemoveDictionary(const DomainStrSID& domainSid) noexcept;
        KMP_NODISCARD KMP_API bool RemoveDictionary(const DomainStr& domain) noexcept;

        KMP_API void Add(const DomainStrSID& domainSid, const SourceStrSID& sourceSid, const TranslationStr& translation);
        KMP_API void Add(const DomainStrSID& domainSid, const SourceStrSID& sourceSidSingular, const SourceStrSID& sourceSidPlural, PluralityForm pluralityForm, const TranslationStr& translation);
        KMP_API void Add(const DomainStrSID& domainSid, const SourceStrSID& sourceSid, const ContextStrSID& contextSid, const TranslationStr& translation);
        KMP_NODISCARD KMP_API const TranslationStr& Get(const DomainStrSID& domainSid, const SourceStrSID& sourceSid) const;
        KMP_NODISCARD KMP_API const TranslationStr& Get(const DomainStrSID& domainSid, const SourceStrSID& sourceSidSingular, const SourceStrSID& sourceSidPlural, PluralityForm pluralityForm) const;
        KMP_NODISCARD KMP_API const TranslationStr& Get(const DomainStrSID& domainSid, const SourceStrSID& sourceSid, const ContextStrSID& contextSid) const;

    private:
        LocaleStrSID _currentLocaleSid;
        std::unordered_map<DomainStrSID, UPtr<LocalizationDictionary>> _dictionaryMap;
    };
    //--------------------------------------------------------------------------
}