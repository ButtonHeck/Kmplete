#pragma once

#include "Kmplete/Localization/localization_base.h"
#include "Kmplete/Core/kmplete_api.h"
#include "Kmplete/Utils/string_id.h"

#include <unordered_map>

namespace Kmplete
{
    class LocalizationDictionary
    {
    public:
        KMP_DISABLE_COPY_MOVE(LocalizationDictionary)

        KMP_API explicit LocalizationDictionary(const DomainStrSID& domain, const LocaleStrSID& localeSid = SidTrInvalidLocale) KMP_NOEXCEPT;

        KMP_NODISCARD KMP_API const DomainStrSID& GetDomain() const;
        KMP_API void SetLocale(const LocaleStrSID& localeSid);

        KMP_API void Add(const SourceStrSID& sourceSid, const TranslationStr& translation);
        KMP_API void Add(const SourceStrSID& sourceSid, const ContextStrSID& contextSid, const TranslationStr& translation);
        KMP_NODISCARD KMP_API const TranslationStr& Get(const SourceStrSID& sourceSid);
        KMP_NODISCARD KMP_API const TranslationStr& Get(const SourceStrSID& sourceSid, const ContextStrSID& contextSid);

    private:
        using TranslationMap = std::unordered_map<SourceStrSID, TranslationStr>;
        using ContextedTranslationMap = std::unordered_map<ContextedSource, TranslationStr, ContextedSourceHash>;
        using LocalizedTranslationMap = std::unordered_map<LocaleStrSID, TranslationMap>;
        using LocalizedContextedTranslationMap = std::unordered_map<LocaleStrSID, ContextedTranslationMap>;

    private:
        const DomainStrSID _domain;
        LocaleStrSID _currentLocaleSid;
        LocalizedTranslationMap _translationMap;
        LocalizedContextedTranslationMap _translationCtxMap;
    };
    //--------------------------------------------------------------------------
}