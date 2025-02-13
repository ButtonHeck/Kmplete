#pragma once

#include "Kmplete/Utils/string_id.h"

#include <string>

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

    using LocaleStr = std::string;
    using DomainStr = std::string;
    using ContextStr = std::string;
    using SourceStr = std::string;
    using TranslationStr = std::string;

    using LocaleStrSID = Utils::StringID;
    using DomainStrSID = Utils::StringID;
    using ContextStrSID = Utils::StringID;
    using SourceStrSID = Utils::StringID;
    using TranslationStrSID = Utils::StringID;

    struct ContextedSource
    {
        SourceStrSID sourceSid;
        ContextStrSID contextSid;

        bool operator==(const ContextedSource& other) const
        {
            return sourceSid == other.sourceSid && contextSid == other.contextSid;
        }
    };
    //--------------------------------------------------------------------------

    struct ContextedSourceHash
    {
        std::size_t operator()(const ContextedSource& p) const
        {
            return p.sourceSid ^ p.contextSid;
        }
    };
    //--------------------------------------------------------------------------
}