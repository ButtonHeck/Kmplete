#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"


namespace Kmplete
{
    //! An alternative for UUID that may be created either from a string literal ("_sid" literal)
    //! or from a C-style/std string ("ToStringID" function)
    //! StringID provides lower memory usage (compare to strings), fast indexing (as StringID is merely a number)
    //! and better readability in the code - e.g.:
    //! 1) auto playerID = 1929839594ULL;
    //! 2) auto playerID = "player"_sid;
    using StringID = UInt64;


    KMP_NODISCARD constexpr StringID ToStringID(const char* cstr)
    {
        if (cstr == nullptr)
        {
            return 0;
        }

        StringID sid = 0;
        size_t index = 0;
        while (cstr[index] != 0)
        {
            sid = 37 * sid + static_cast<StringID>(17) * static_cast<char>(cstr[index]);
            index++;
        }

        return sid;
    }
    //--------------------------------------------------------------------------

//! This compiler feature enables usage of an std::string in the constexpr context.
//! See https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2019/p0980r1.pdf for details
//! Otherwise use c_str() as an argument for ToStringID function
#if defined (__cpp_lib_constexpr_string) && __cpp_lib_constexpr_string >= 201907L
#define KMP_SID_PARAM(str) str

    KMP_NODISCARD constexpr StringID ToStringID(const String& str)
    {
        return ToStringID(str.c_str());
    }
    //--------------------------------------------------------------------------
#else
#define KMP_SID_PARAM(str) str.c_str()
#endif

    KMP_NODISCARD consteval StringID operator ""_sid(const char* cstr, std::size_t)
    {
        return ToStringID(cstr);
    }
    //--------------------------------------------------------------------------
}
