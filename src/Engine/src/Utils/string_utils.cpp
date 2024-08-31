#include "Kmplete/Utils/string_utils.h"
#include "Kmplete/Core/platform.h"

#include <string>
#if defined KMP_PLATFORM_WINDOWS
#include <Windows.h>
#else
#include <cstdlib>
#endif

namespace Kmplete
{
    namespace Utils
    {
        std::wstring NarrowToWide(const std::string& str)
        {
#if defined KMP_PLATFORM_WINDOWS
            const auto length = MultiByteToWideChar(CP_ACP, 0, str.c_str(), static_cast<int>(str.size()), 0, 0);
            std::wstring wide(length, L'\0');
            MultiByteToWideChar(CP_ACP, 0, str.c_str(), static_cast<int>(str.size()), &wide[0], length);
            return wide;
#else
            std::wstring wide(str.length() * 4, L'\0');
            std::mbstowcs(&wide[0], str.c_str(), str.length());
            return std::wstring(wide.c_str());
#endif
        }
        //--------------------------------------------------------------------------

        std::string WideToNarrow(const std::wstring& wstr)
        {
#if defined KMP_PLATFORM_WINDOWS
            const auto length = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), static_cast<int>(wstr.size()), nullptr, 0, nullptr, nullptr);
            std::string narrow(length, '\0');
            WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), static_cast<int>(wstr.size()), &narrow[0], length, nullptr, nullptr);
            return narrow;
#else
            std::string narrow(wstr.length() * 4, '\0');
            std::wcstombs(&narrow[0], wstr.c_str(), wstr.length());
            return std::string(narrow.c_str());
#endif
        }
        //--------------------------------------------------------------------------

        std::string NarrowToUtf8(const std::string& str)
        {
#if defined KMP_PLATFORM_WINDOWS
            const auto size = MultiByteToWideChar(CP_ACP, MB_COMPOSITE, str.c_str(), static_cast<int>(str.length()), nullptr, 0);
            std::wstring u16Str(size, L'\0');
            MultiByteToWideChar(CP_ACP, MB_COMPOSITE, str.c_str(), static_cast<int>(str.length()), &u16Str[0], size);

            const auto u8Size = WideCharToMultiByte(CP_UTF8, 0, u16Str.c_str(), static_cast<int>(u16Str.length()), nullptr, 0, nullptr, nullptr);
            std::string u8Str(u8Size, '\0');
            WideCharToMultiByte(CP_UTF8, 0, u16Str.c_str(), static_cast<int>(u16Str.length()), &u8Str[0], u8Size, nullptr, nullptr);
            return u8Str;
#else
            return str;
#endif
        }
        //--------------------------------------------------------------------------

        std::string Utf8ToNarrow(const std::string& str)
        {
#if defined KMP_PLATFORM_WINDOWS
            const auto u16Size = MultiByteToWideChar(CP_UTF8, 0, &str[0], static_cast<int>(str.size()), nullptr, 0);
            if (u16Size > 0)
            {
                std::wstring u16Str(u16Size, L'\0');
                MultiByteToWideChar(CP_UTF8, 0, &str[0], static_cast<int>(str.size()), &u16Str[0], u16Size);
                return WideToNarrow(u16Str);
            }

            return std::string();
#else
            return str;
#endif
        }
        //--------------------------------------------------------------------------
    }
}
