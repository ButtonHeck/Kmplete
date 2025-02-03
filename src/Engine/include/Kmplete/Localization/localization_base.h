#pragma once

#include "Kmplete/Utils/string_id.h"

#include <string>

namespace Kmplete
{
    static constexpr auto TranslateKeyword = "Translate";
    static constexpr auto TranslateCtxKeyword = "TranslateCtx";
    static constexpr auto TranslateDeferKeyword = "TranslateDefer";
    static constexpr auto TranslateCtxDeferKeyword = "TranslateCtxDefer";

    static constexpr auto LocaleEnUTF8Keyword = "en_EN.UTF-8";
    static constexpr auto LocaleEnName = "English";
    static constexpr auto LocaleRuUTF8Keyword = "ru_RU.UTF-8";
    static constexpr auto LocaleRuName = "Russian";

    static constexpr auto SidTrLocaleEnUTF8 = Utils::ToStringID(LocaleEnUTF8Keyword);
    static constexpr auto SidTrLocaleRuUTF8 = Utils::ToStringID(LocaleRuUTF8Keyword);
    static constexpr auto SidTrDomainEngine = Utils::ToStringID(KMP_TR_DOMAIN_ENGINE);

    using LocaleStr = std::string;
    using DomainStr = std::string;
    using ContextStr = std::string;
    using SourceStr = std::string;
    using TranslationStr = std::string;

    struct ContextedSource
    {
        SourceStr source;
        ContextStr context;

        bool operator==(const ContextedSource& other) const
        {
            return source == other.source && context == other.context;
        }
    };
    //--------------------------------------------------------------------------

    struct ContextedSourceHash
    {
        std::size_t operator()(const ContextedSource& p) const
        {
            const auto hs = std::hash<SourceStr>{}(p.source);
            const auto hc = std::hash<ContextStr>{}(p.context);

            return hs ^ hc;
        }
    };
    //--------------------------------------------------------------------------
}