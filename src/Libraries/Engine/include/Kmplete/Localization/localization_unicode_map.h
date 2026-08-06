#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Localization/localization_base.h"
#include "Kmplete/Log/log_class_macro.h"


namespace Kmplete
{
    namespace Localization
    {
        //TODO: comments
        class KMP_API UnicodeMap
        {
            KMP_LOG_CLASSNAME(UnicodeMap)
            KMP_DISABLE_COPY_MOVE(UnicodeMap)

        public:
            KMP_NODISCARD static bool Initialize();

            KMP_NODISCARD static const CodepointVector& GetCodepointVector(const LocaleStr& locale);

        private:
            KMP_NODISCARD static bool _InitializeEn();
            KMP_NODISCARD static bool _InitializeRu();
            
        private:
            static bool _initialized;
            static HashMap<LocaleStr, CodepointVector> _localeCodepointRanges;
        };
        //--------------------------------------------------------------------------
    }
}