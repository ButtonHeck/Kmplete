#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"

#include <cstdint>

namespace Kmplete
{
    namespace Utils
    {
        using StringID = uint64_t;

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
    }

    KMP_NODISCARD consteval Utils::StringID operator ""_sid(const char* cstr, std::size_t)
    {
        return Utils::ToStringID(cstr);
    }
    //--------------------------------------------------------------------------
}
