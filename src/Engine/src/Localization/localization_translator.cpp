#include "Kmplete/Localization/localization_translator.h"

namespace Kmplete
{
    namespace Translator
    {
        TranslationStr Translate(const DomainStr& domain, const SourceStr& source)
        {
            return boost::locale::translate(source).str(domain);
        }
        //--------------------------------------------------------------------------

        TranslationStr Translate(const DomainStr& domain, const SourceStr& sourceSingular, const SourceStr& sourcePlural, int count)
        {
            return boost::locale::translate(sourceSingular, sourcePlural, count).str(domain);
        }
        //--------------------------------------------------------------------------

        TranslationStr TranslateCtx(const DomainStr& domain, const SourceStr& source, const ContextStr& context)
        {
            return boost::locale::translate(context, source).str(domain);
        }
        //--------------------------------------------------------------------------

        TranslationStr TranslateCtx(const DomainStr& domain, const SourceStr& sourceSingular, const SourceStr& sourcePlural, int count, const ContextStr& context)
        {
            return boost::locale::translate(context, sourceSingular, sourcePlural, count).str(domain);
        }
        //--------------------------------------------------------------------------
    }
}