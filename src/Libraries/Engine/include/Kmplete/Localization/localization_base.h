#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Base/type_traits.h"
#include "Kmplete/Base/string_id.h"


namespace Kmplete
{
    static constexpr auto LocalesDirectory = "locale";

    static constexpr auto TranslateKeyword = "Translate";
    static constexpr auto TranslateCtxKeyword = "TranslateCtx";
    static constexpr auto TranslateDeferKeyword = "TranslateDefer";
    static constexpr auto TranslateCtxDeferKeyword = "TranslateCtxDefer";

    static constexpr auto LocaleEnUTF8Keyword = "en_US.UTF-8";
    static constexpr auto LocaleEnName = "English";
    static constexpr auto LocaleRuUTF8Keyword = "ru_RU.UTF-8";
    static constexpr auto LocaleRuName = "Russian";

    static constexpr auto SidTrInvalidLocale = static_cast<StringID>(0);
    static constexpr auto SidTrLocaleEnName = ToStringID(LocaleEnName);
    static constexpr auto SidTrLocaleRuName = ToStringID(LocaleRuName);
    static constexpr auto SidTrDomainEngine = ToStringID(KMP_TR_DOMAIN_ENGINE);


    using LocaleStr = String;
    using DomainStr = String;
    using ContextStr = String;
    using SourceStr = String;
    using TranslationStr = String;

    using LocaleStrSID = StringID;
    using DomainStrSID = StringID;
    using ContextStrSID = StringID;
    using SourceStrSID = StringID;
    using TranslationStrSID = StringID;


    //! This defines what form of a word is used (plural or singular),
    //! moreover some languages may have multiple plural form, thus the engine
    //! includes at least two of them.
    enum PluralityForm
    {
        PluralityFormSingular,
        PluralityFormPlural1,
        PluralityFormPlural2,
        PluralityFormCount
    };
    //--------------------------------------------------------------------------

    //! Base class of plurality dispatcher that is responsible for checking what form a word should
    //! have based on the numeral used for this word 
    struct PluralityFormDispatcher
    {
        KMP_NODISCARD KMP_API virtual PluralityForm GetPluralityForm(int count) const noexcept = 0;
    };
    //--------------------------------------------------------------------------

    //! Dispatcher for English translations
    struct PluralityFormDispatcherEn : public PluralityFormDispatcher
    {
        KMP_NODISCARD KMP_API PluralityForm GetPluralityForm(int count) const noexcept override;
    };
    //--------------------------------------------------------------------------

    //! Dispatcher for Russian translations
    struct PluralityFormDispatcherRu : public PluralityFormDispatcher
    {
        KMP_NODISCARD KMP_API PluralityForm GetPluralityForm(int count) const noexcept override;
    };
    //--------------------------------------------------------------------------

    KMP_NODISCARD KMP_API const PluralityFormDispatcher& GetPluralityDispatcher(const LocaleStr& localeStr) noexcept;

    static constexpr PluralityFormDispatcherEn pluralFormIndexGeneratorEn;
    static constexpr PluralityFormDispatcherRu pluralFormIndexGeneratorRu;


    //! Source with a context, e.g. word "quit" translation depends on whether it is a noun or a verb,
    //! or a word may have different translations based on what context it is used with
    struct ContextedSource
    {
        SourceStrSID sourceSid;
        ContextStrSID contextSid;

        bool operator==(const ContextedSource& other) const
        {
            return sourceSid == other.sourceSid &&
                   contextSid == other.contextSid;
        }
    };
    static_assert(IsTriviallyCopyable<ContextedSource>::value);
    static_assert(IsTriviallyMoveConstructible<ContextedSource>::value);
    static_assert(IsTriviallyMoveAssignable<ContextedSource>::value);
    //--------------------------------------------------------------------------

    //! Utility hasher struct to enable storing ContextedSource in a hash map
    struct ContextedSourceHash
    {
        std::size_t operator()(const ContextedSource& source) const
        {
            std::size_t hash = source.sourceSid;
            hash ^= source.contextSid + 0x9e3779b9 + (hash << 6) + (hash >> 2);
            return hash;
        }
    };
    //--------------------------------------------------------------------------


    //! Source that is used with a numeral, all translations are initially created from English
    //! strings that are distinguished by a simple rule "1 or many" - thus there is only a pair
    //! of string for each form, but for other languages the actual translation form depends
    //! on the what PluralityFormDispatcher returns.
    //! @see PluralityFormDispatcher
    struct PluralSource
    {
        SourceStrSID sourceSidSingular;
        SourceStrSID sourceSidPlural;

        bool operator==(const PluralSource& other) const
        {
            return sourceSidSingular == other.sourceSidSingular &&
                   sourceSidPlural == other.sourceSidPlural;
        }
    };
    static_assert(IsTriviallyCopyable<PluralSource>::value);
    static_assert(IsTriviallyMoveConstructible<PluralSource>::value);
    static_assert(IsTriviallyMoveAssignable<PluralSource>::value);
    //--------------------------------------------------------------------------

    //! Utility hasher struct to enable storing PluralSource in a hash map
    struct PluralSourceHash
    {
        std::size_t operator()(const PluralSource& source) const
        {
            std::size_t hash = source.sourceSidSingular;
            hash ^= source.sourceSidPlural + 0x9e3779b9 + (hash << 6) + (hash >> 2);
            return hash;
        }
    };
    //--------------------------------------------------------------------------


    //! Combination of ContextedSource and PluralSource structures
    //! @see ContextedSource
    //! @see PluralSource
    struct ContextedPluralSource
    {
        SourceStrSID sourceSidSingular;
        SourceStrSID sourceSidPlural;
        ContextStrSID contextSid;

        bool operator==(const ContextedPluralSource& other) const
        {
            return sourceSidSingular == other.sourceSidSingular &&
                   sourceSidPlural == other.sourceSidPlural &&
                   contextSid == other.contextSid;
        }
    };
    static_assert(IsTriviallyCopyable<ContextedPluralSource>::value);
    static_assert(IsTriviallyMoveConstructible<ContextedPluralSource>::value);
    static_assert(IsTriviallyMoveAssignable<ContextedPluralSource>::value);
    //--------------------------------------------------------------------------

    //! Utility hasher struct to enable storing PluralSource in a hash map
    struct ContextedPluralSourceHash
    {
        std::size_t operator()(const ContextedPluralSource& source) const
        {
            std::size_t hash = source.sourceSidSingular;
            hash ^= source.sourceSidPlural + 0x9e3779b9 + (hash << 6) + (hash >> 2);
            hash ^= source.contextSid + 0x9e3779b9 + (hash << 6) + (hash >> 2);
            return hash;
        }
    };
    //--------------------------------------------------------------------------
}