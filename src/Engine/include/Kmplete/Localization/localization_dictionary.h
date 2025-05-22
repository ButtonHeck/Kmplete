#pragma once

#include "Kmplete/Localization/localization_base.h"
#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Utils/string_id.h"

#include <unordered_map>
#include <array>

namespace Kmplete
{
    class LocalizationDictionary
    {
    public:
        KMP_DISABLE_COPY_MOVE(LocalizationDictionary)

        KMP_API explicit LocalizationDictionary(const DomainStrSID& domain, const LocaleStrSID& localeSid = SidTrInvalidLocale) noexcept;
        ~LocalizationDictionary() = default;

        KMP_NODISCARD KMP_API const DomainStrSID& GetDomain() const;
        KMP_API void SetLocale(const LocaleStrSID& localeSid);

        KMP_API void Add(const SourceStrSID& sourceSid, const TranslationStr& translation);
        KMP_API void Add(const SourceStrSID& sourceSidSingular, const SourceStrSID& sourceSidPlural, 
                         PluralityForm pluralityForm, const TranslationStr& translation);
        KMP_API void Add(const SourceStrSID& sourceSid, const ContextStrSID& contextSid, const TranslationStr& translation);
        KMP_API void Add(const SourceStrSID& sourceSidSingular, const SourceStrSID& sourceSidPlural, 
                         PluralityForm pluralityForm, const ContextStrSID& contextSid, const TranslationStr& translation);

        KMP_NODISCARD KMP_API const TranslationStr& Get(const SourceStrSID& sourceSid);
        KMP_NODISCARD KMP_API const TranslationStr& Get(const SourceStrSID& sourceSidSingular, const SourceStrSID& sourceSidPlural, PluralityForm pluralityForm);
        KMP_NODISCARD KMP_API const TranslationStr& Get(const SourceStrSID& sourceSid, const ContextStrSID& contextSid);
        KMP_NODISCARD KMP_API const TranslationStr& Get(const SourceStrSID& sourceSidSingular, const SourceStrSID& sourceSidPlural, 
                                                        PluralityForm pluralityForm, const ContextStrSID& contextSid);

    private:
        using TranslationMap = std::unordered_map<SourceStrSID, TranslationStr>;
        using ContextedTranslationMap = std::unordered_map<ContextedSource, TranslationStr, ContextedSourceHash>;
        using PluralTranslations = std::array<TranslationStr, PluralityFormCount>;
        using TranslationPluralMap = std::unordered_map<PluralSource, PluralTranslations, PluralSourceHash>;
        using ContextedTranslationPluralMap = std::unordered_map<ContextedPluralSource, PluralTranslations, ContextedPluralSourceHash>;

        using LocalizedTranslationMap = std::unordered_map<LocaleStrSID, TranslationMap>;
        using LocalizedContextedTranslationMap = std::unordered_map<LocaleStrSID, ContextedTranslationMap>;
        using LocalizedTranslationPluralMap = std::unordered_map<LocaleStrSID, TranslationPluralMap>;
        using LocalizedContextedTranslationPluralMap = std::unordered_map<LocaleStrSID, ContextedTranslationPluralMap>;

    private:
        const DomainStrSID _domain;
        LocaleStrSID _currentLocaleSid;
        LocalizedTranslationMap _translationMap;
        LocalizedContextedTranslationMap _translationCtxMap;
        LocalizedTranslationPluralMap _translationPluralMap;
        LocalizedContextedTranslationPluralMap _translationCtxPluralMap;
    };
    //--------------------------------------------------------------------------
}