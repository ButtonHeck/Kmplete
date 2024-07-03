#pragma once

#include "kmplete_export.h"

#include <sstream>

namespace Kmplete
{
    namespace Utils
    {
        template<typename... Ts>
        KMPLETE_API std::string Concatenate(Ts&&... args)
        {
            std::ostringstream oss;
            (oss << ... << std::forward<Ts>(args));
            return oss.str();
        }
        //--------------------------------------------------------------------------

        template<typename... Ts>
        KMPLETE_API std::ostringstream& ToSStream(std::ostringstream& oss, Ts&&... args)
        {
            (oss << ... << std::forward<Ts>(args));
            return oss;
        }
        //--------------------------------------------------------------------------

        KMPLETE_API std::wstring NarrowToWide(const std::string& str);
        //--------------------------------------------------------------------------

        KMPLETE_API std::string WideToNarrow(const std::wstring& wstr);
        //--------------------------------------------------------------------------

        KMPLETE_API std::string NarrowToU8(const std::string& str);
        //--------------------------------------------------------------------------

        KMPLETE_API std::string U8ToNarrow(const std::string& str);
        //--------------------------------------------------------------------------
    }
}