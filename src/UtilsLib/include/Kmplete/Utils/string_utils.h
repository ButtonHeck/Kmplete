#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Profile/profiler.h"

#include <sstream>

namespace Kmplete
{
    namespace Utils
    {
        template<typename... Ts>
        KMP_NODISCARD String Concatenate(Ts&&... args)
        {
            KMP_PROFILE_FUNCTION();

            std::ostringstream oss;
            (oss << ... << std::forward<Ts>(args));
            return oss.str();
        }
        //--------------------------------------------------------------------------

        template<typename... Ts>
        std::ostringstream& ToSStream(std::ostringstream& oss, Ts&&... args)
        {
            KMP_PROFILE_FUNCTION();

            (oss << ... << std::forward<Ts>(args));
            return oss;
        }
        //--------------------------------------------------------------------------

        KMP_NODISCARD KMP_API String RegexReplace(const String& source, const char* regexp, const char* replacement);
        KMP_NODISCARD KMP_API String StringVectorToString(const StringVector& stringVector, char delimiter);
        KMP_NODISCARD KMP_API String StringVectorToString(const StringVector& stringVector, const String& delimiter);
        KMP_NODISCARD KMP_API WString NarrowToWide(const String& str);
        KMP_NODISCARD KMP_API String WideToNarrow(const WString& wstr);
        KMP_NODISCARD KMP_API String NarrowToUtf8(const String& str);
        KMP_NODISCARD KMP_API String Utf8ToNarrow(const String& str);
    }
}