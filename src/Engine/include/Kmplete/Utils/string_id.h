#pragma once

#include "Kmplete/Core/kmplete_api.h"

#include <cstdint>
#include <string>

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

        KMP_NODISCARD constexpr StringID ToStringID(const std::string& str)
        {
            return ToStringID(str.c_str());
        }
        //--------------------------------------------------------------------------
    }

    KMP_NODISCARD consteval Utils::StringID operator ""_sid(const char* cstr, std::size_t)
    {
        return Utils::ToStringID(cstr);
    }
    //--------------------------------------------------------------------------
}
