#pragma once

#include "Kmplete/Localization/localization_base.h"
#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Base/string_id.h"
#include "Kmplete/Profile/profiler.h"
#include "Kmplete/Log/log_class_macro.h"


namespace Kmplete
{
    //! Localization storage class, responsible for storing all kind of translations for
    //! a dedicated domain. Storage structures for translations are implemented as a 
    //! map (locale identifier as key) of maps ([contexted|plural]source identifier as a key) of 
    //! actual translations. Although it is not prohibited to use this class as is, dictionary is supposed
    //! to be used as a delegate for LocalizationLibrary class
    //! @see LocalizationLibrary
    class LocalizationDictionary
    {
        KMP_LOG_CLASSNAME(LocalizationDictionary)
        KMP_PROFILE_CONSTRUCTOR_DECLARE()
        KMP_DISABLE_COPY_MOVE(LocalizationDictionary)

    public:
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
        using TranslationMap = HashMap<SourceStrSID, TranslationStr>;
        using ContextedTranslationMap = HashMap<ContextedSource, TranslationStr, ContextedSourceHash>;
        using PluralTranslations = Array<TranslationStr, PluralityFormCount>;
        using TranslationPluralMap = HashMap<PluralSource, PluralTranslations, PluralSourceHash>;
        using ContextedTranslationPluralMap = HashMap<ContextedPluralSource, PluralTranslations, ContextedPluralSourceHash>;

        using LocalizedTranslationMap = HashMap<LocaleStrSID, TranslationMap>;
        using LocalizedContextedTranslationMap = HashMap<LocaleStrSID, ContextedTranslationMap>;
        using LocalizedTranslationPluralMap = HashMap<LocaleStrSID, TranslationPluralMap>;
        using LocalizedContextedTranslationPluralMap = HashMap<LocaleStrSID, ContextedTranslationPluralMap>;

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