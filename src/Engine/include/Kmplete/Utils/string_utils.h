#pragma once

#include "Kmplete/Core/kmplete_api.h"

#include <sstream>

namespace Kmplete
{
    namespace Utils
    {
        template<typename... Ts>
        std::string Concatenate(Ts&&... args)
        {
            std::ostringstream oss;
            (oss << ... << std::forward<Ts>(args));
            return oss.str();
        }
        //--------------------------------------------------------------------------

        template<typename... Ts>
        std::ostringstream& ToSStream(std::ostringstream& oss, Ts&&... args)
        {
            (oss << ... << std::forward<Ts>(args));
            return oss;
        }
        //--------------------------------------------------------------------------

        KMP_API std::wstring NarrowToWide(const std::string& str);
        //--------------------------------------------------------------------------

        KMP_API std::string WideToNarrow(const std::wstring& wstr);
        //--------------------------------------------------------------------------

        KMP_API std::string NarrowToUtf8(const std::string& str);
        //--------------------------------------------------------------------------

        KMP_API std::string Utf8ToNarrow(const std::string& str);
        //--------------------------------------------------------------------------
    }
}