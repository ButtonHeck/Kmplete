#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Base/type_traits.h"
#include "Kmplete/Utils/string_id.h"


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

    static constexpr auto SidTrInvalidLocale = static_cast<Utils::StringID>(0);
    static constexpr auto SidTrLocaleEnName = Utils::ToStringID(LocaleEnName);
    static constexpr auto SidTrLocaleRuName = Utils::ToStringID(LocaleRuName);
    static constexpr auto SidTrDomainEngine = Utils::ToStringID(KMP_TR_DOMAIN_ENGINE);


    using LocaleStr = String;
    using DomainStr = String;
    using ContextStr = String;
    using SourceStr = String;
    using TranslationStr = String;

    using LocaleStrSID = Utils::StringID;
    using DomainStrSID = Utils::StringID;
    using ContextStrSID = Utils::StringID;
    using SourceStrSID = Utils::StringID;
    using TranslationStrSID = Utils::StringID;


    enum PluralityForm
    {
        PluralityFormSingular,
        PluralityFormPlural1,
        PluralityFormPlural2,
        PluralityFormCount
    };
    //--------------------------------------------------------------------------

    struct PluralityFormDispatcher
    {
        KMP_NODISCARD KMP_API virtual PluralityForm GetPluralityForm(int count) const noexcept = 0;
    };
    //--------------------------------------------------------------------------

    struct PluralityFormDispatcherEn : public PluralityFormDispatcher
    {
        KMP_NODISCARD KMP_API PluralityForm GetPluralityForm(int count) const noexcept override;
    };
    //--------------------------------------------------------------------------

    struct PluralityFormDispatcherRu : public PluralityFormDispatcher
    {
        KMP_NODISCARD KMP_API PluralityForm GetPluralityForm(int count) const noexcept override;
    };
    //--------------------------------------------------------------------------

    KMP_NODISCARD KMP_API const PluralityFormDispatcher& GetPluralityDispatcher(const LocaleStr& localeStr) noexcept;
    //--------------------------------------------------------------------------

    static constexpr PluralityFormDispatcherEn pluralFormIndexGeneratorEn;
    static constexpr PluralityFormDispatcherRu pluralFormIndexGeneratorRu;


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

    struct ContextedSourceHash
    {
        std::size_t operator()(const ContextedSource& source) const
        {
            return source.sourceSid ^ source.contextSid;
        }
    };
    //--------------------------------------------------------------------------


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

    struct PluralSourceHash
    {
        std::size_t operator()(const PluralSource& source) const
        {
            return source.sourceSidSingular ^ source.sourceSidPlural;
        }
    };
    //--------------------------------------------------------------------------


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

    struct ContextedPluralSourceHash
    {
        std::size_t operator()(const ContextedPluralSource& source) const
        {
            return source.sourceSidSingular ^ source.sourceSidPlural ^ source.contextSid;
        }
    };
    //--------------------------------------------------------------------------
}