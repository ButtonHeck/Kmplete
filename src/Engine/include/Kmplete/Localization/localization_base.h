#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types.h"
#include "Kmplete/Utils/string_id.h"

#include <type_traits>

namespace Kmplete
{
    static constexpr auto LocalesDirectory = "locale";

    static constexpr auto TranslateKeyword = "Translate";
    static constexpr auto TranslateCtxKeyword = "TranslateCtx";
    static constexpr auto TranslateDeferKeyword = "TranslateDefer";
    static constexpr auto TranslateCtxDeferKeyword = "TranslateCtxDefer";

    static constexpr auto LocaleEnUTF8Keyword = "en_EN.UTF-8";
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
    static_assert(std::is_trivially_copyable_v<ContextedSource> == true);
    static_assert(std::is_trivially_move_constructible_v<ContextedSource> == true);
    static_assert(std::is_trivially_move_assignable_v<ContextedSource> == true);
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
    static_assert(std::is_trivially_copyable_v<PluralSource> == true);
    static_assert(std::is_trivially_move_constructible_v<PluralSource> == true);
    static_assert(std::is_trivially_move_assignable_v<PluralSource> == true);
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
    static_assert(std::is_trivially_copyable_v<ContextedPluralSource> == true);
    static_assert(std::is_trivially_move_constructible_v<ContextedPluralSource> == true);
    static_assert(std::is_trivially_move_assignable_v<ContextedPluralSource> == true);
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