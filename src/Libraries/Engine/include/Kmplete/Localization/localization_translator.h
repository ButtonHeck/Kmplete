#pragma once

#include "Kmplete/Localization/localization_base.h"
#include "Kmplete/Base/kmplete_api.h"

#include <boost/locale.hpp>


namespace Kmplete
{
    //! Families of free functions that delegates direct translation routines to appropriate Boost.Locale function calls.
    //! "Translate[...]" functions names SHOULD MATCH parameters of gettext utility
    //! programs in order to successfully update and compile translations - thus if any renaming occurs, make sure
    //! to rename those parameters as well.
    namespace Localization
    {
        KMP_NODISCARD KMP_API TranslationStr Translate(const DomainStr& domain, const SourceStr& source, const std::locale& locale = std::locale());
        KMP_NODISCARD KMP_API TranslationStr Translate(const DomainStr& domain, const SourceStr& sourceSingular, const SourceStr& sourcePlural, int count, const std::locale& locale = std::locale());
        KMP_NODISCARD KMP_API TranslationStr TranslateCtx(const DomainStr& domain, const SourceStr& source, const ContextStr& context, const std::locale& locale = std::locale());
        KMP_NODISCARD KMP_API TranslationStr TranslateCtx(const DomainStr& domain, const SourceStr& sourceSingular, const SourceStr& sourcePlural, int count, const ContextStr& context, const std::locale& locale = std::locale());

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