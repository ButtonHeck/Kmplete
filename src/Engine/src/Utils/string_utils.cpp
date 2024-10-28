#include "Kmplete/Utils/string_utils.h"
#include "Kmplete/Core/platform.h"

#include <string>
#include <numeric>
#if defined KMP_PLATFORM_WINDOWS
#include <Windows.h>
#else
#include <cstdlib>
#endif

namespace Kmplete
{
    namespace Utils
    {
        std::string StringVectorToString(const StringVector& stringVector, char delimiter)
        {
            return StringVectorToString(stringVector, std::string({delimiter}));
        }
        //--------------------------------------------------------------------------

        std::string StringVectorToString(const StringVector& stringVector, const std::string& delimiter)
        {
            if (stringVector.empty())
            {
                return "";
            }

            return std::accumulate(stringVector.begin(), stringVector.end(), std::string(),
                [&delimiter](const std::string& a, const std::string& b) {
                    return b.empty() ? a : (a + delimiter + b);
                });
        }
        //--------------------------------------------------------------------------

        std::wstring NarrowToWide(const std::string& str)
        {
#if defined KMP_PLATFORM_WINDOWS
            const auto length = MultiByteToWideChar(CP_ACP, 0, str.c_str(), static_cast<int>(str.size()), 0, 0);
            std::wstring wide(length, L'\0');
            MultiByteToWideChar(CP_ACP, 0, str.c_str(), static_cast<int>(str.size()), &wide[0], length);
            return wide;
#else
            mbstate_t st = {};
            const unsigned int len = str.length();
            const char* ptr = str.c_str();
            const ssize_t bufSize = len * sizeof(std::string::value_type) + 1;
            std::vector<wchar_t> buf(bufSize, 0);
            const ssize_t res = std::mbsrtowcs(&buf[0], &ptr, bufSize, &st);
            return (res >= 0) ? std::wstring(buf.begin(), buf.begin() + res) : L"?";
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
            mbstate_t st = {};
            const unsigned int wlen = wstr.length();
            const wchar_t* wptr = wstr.c_str();
            const ssize_t bufSize = wlen * sizeof(std::wstring::value_type) + 1;
            std::vector<char> buf(bufSize, 0);
            const ssize_t res = std::wcsrtombs(&buf[0], &wptr, bufSize, &st);
            return (res >= 0) ? std::string(buf.begin(), buf.begin() + res) : "?";
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
