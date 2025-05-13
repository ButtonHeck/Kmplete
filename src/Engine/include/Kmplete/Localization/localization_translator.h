#pragma once

#include "Kmplete/Localization/localization_base.h"
#include "Kmplete/Core/kmplete_api.h"

#include <boost/locale.hpp>

namespace Kmplete
{
    namespace Translator
    {
        KMP_NODISCARD KMP_API TranslationStr Translate(const DomainStr& domain, const SourceStr& source);
        KMP_NODISCARD KMP_API TranslationStr Translate(const DomainStr& domain, const SourceStr& sourceSingular, const SourceStr& sourcePlural, int count);
        KMP_NODISCARD KMP_API TranslationStr TranslateCtx(const DomainStr& domain, const SourceStr& source, const ContextStr& context);
        KMP_NODISCARD KMP_API TranslationStr TranslateCtx(const DomainStr& domain, const SourceStr& sourceSingular, const SourceStr& sourcePlural, int count, const ContextStr& context);

        inline void TranslateDefer(const DomainStr&, const SourceStr&) {};
        inline void TranslateDefer(const DomainStr&, const SourceStr&, const SourceStr&, int) {};
        inline void TranslateCtxDefer(const DomainStr&, const SourceStr&, const ContextStr&) {};
        inline void TranslateCtxDefer(const DomainStr&, const SourceStr&, const SourceStr&, int, const ContextStr&) {};

        template<typename... Types>
        KMP_NODISCARD TranslationStr Format(const SourceStr& source, Types&&... args)
        {
            return (boost::locale::format(source) % ... % std::forward<Types>(args)).str();
        }
        //--------------------------------------------------------------------------
    }
}