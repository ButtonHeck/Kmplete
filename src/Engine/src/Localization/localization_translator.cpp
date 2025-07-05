#include "Kmplete/Localization/localization_translator.h"
#include "Kmplete/Profile/profiler.h"

namespace Kmplete
{
    namespace Localization
    {
        TranslationStr Translate(const DomainStr& domain, const SourceStr& source, const std::locale& locale /*= std::locale()*/)
        {
            KMP_PROFILE_FUNCTION();

            return boost::locale::translate(source).str(locale, domain);
        }
        //--------------------------------------------------------------------------

        TranslationStr Translate(const DomainStr& domain, const SourceStr& sourceSingular, const SourceStr& sourcePlural, int count, const std::locale& locale /*= std::locale()*/)
        {
            KMP_PROFILE_FUNCTION();

            return boost::locale::translate(sourceSingular, sourcePlural, count).str(locale, domain);
        }
        //--------------------------------------------------------------------------

        TranslationStr TranslateCtx(const DomainStr& domain, const SourceStr& source, const ContextStr& context, const std::locale& locale /*= std::locale()*/)
        {
            KMP_PROFILE_FUNCTION();

            return boost::locale::translate(context, source).str(locale, domain);
        }
        //--------------------------------------------------------------------------

        TranslationStr TranslateCtx(const DomainStr& domain, const SourceStr& sourceSingular, const SourceStr& sourcePlural, int count, const ContextStr& context, const std::locale& locale /*= std::locale()*/)
        {
            KMP_PROFILE_FUNCTION();

            return boost::locale::translate(context, sourceSingular, sourcePlural, count).str(locale, domain);
        }
        //--------------------------------------------------------------------------
    }
}