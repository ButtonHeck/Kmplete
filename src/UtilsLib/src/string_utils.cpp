#include "Kmplete/Utils/string_utils.h"
#include "Kmplete/Base/platform.h"
#include "Kmplete/Log/log.h"

#include <numeric>
#include <regex>
#if defined (KMP_PLATFORM_WINDOWS)
    #include <Windows.h>
#else
    #include <cstdlib>
#endif

namespace Kmplete
{
    namespace Utils
    {
        String RegexReplace(const String& source, const char* regexp, const char* replacement)
        {
            KMP_PROFILE_FUNCTION();

            if (regexp == nullptr || replacement == nullptr)
            {
                KMP_LOG_WARN_FN("Utils: RegexReplace - either regexp or replacement string is nullptr");
                return source;
            }

            try
            {
                const std::regex regexObject(regexp, std::regex_constants::ECMAScript);
                return std::regex_replace(source, regexObject, replacement);
            }
            catch (KMP_MB_UNUSED const std::regex_error& re)
            {
                KMP_LOG_ERROR_FN("Utils: regex '{}' error for string '{}': '{}'", regexp, source, re.what());
                return source;
            }
        }
        //--------------------------------------------------------------------------

        String StringVectorToString(const StringVector& stringVector, char delimiter)
        {
            return StringVectorToString(stringVector, String({delimiter}));
        }
        //--------------------------------------------------------------------------

        String StringVectorToString(const StringVector& stringVector, const String& delimiter)
        {
            KMP_PROFILE_FUNCTION();

            if (stringVector.empty())
            {
                KMP_LOG_WARN_FN("Utils: StringVectorToString - stringVector is empty");
                return "";
            }

            return std::accumulate(stringVector.begin(), stringVector.end(), String(),
                [&delimiter](const String& a, const String& b) {
                    return b.empty() ? a : (a + delimiter + b);
                });
        }
        //--------------------------------------------------------------------------

        WString NarrowToWide(const String& str)
        {
            KMP_PROFILE_FUNCTION();

#if defined (KMP_PLATFORM_WINDOWS)
            const auto length = MultiByteToWideChar(CP_ACP, 0, str.c_str(), static_cast<int>(str.size()), 0, 0);
            WString wide(length, L'\0');
            MultiByteToWideChar(CP_ACP, 0, str.c_str(), static_cast<int>(str.size()), &wide[0], length);
            return wide;
#else
            mbstate_t st = {};
            const unsigned int len = str.length();
            const char* ptr = str.c_str();
            const ssize_t bufSize = len * sizeof(String::value_type) + 1;
            Vector<wchar_t> buf(bufSize, 0);
            const ssize_t res = std::mbsrtowcs(&buf[0], &ptr, bufSize, &st);
            return (res >= 0) ? WString(buf.begin(), buf.begin() + res) : L"?";
#endif
        }
        //--------------------------------------------------------------------------

        String WideToNarrow(const WString& wstr)
        {
            KMP_PROFILE_FUNCTION();

#if defined (KMP_PLATFORM_WINDOWS)
            const auto length = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), static_cast<int>(wstr.size()), nullptr, 0, nullptr, nullptr);
            String narrow(length, '\0');
            WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), static_cast<int>(wstr.size()), &narrow[0], length, nullptr, nullptr);
            return narrow;
#else
            mbstate_t st = {};
            const unsigned int wlen = wstr.length();
            const wchar_t* wptr = wstr.c_str();
            const ssize_t bufSize = wlen * sizeof(WString::value_type) + 1;
            Vector<char> buf(bufSize, 0);
            const ssize_t res = std::wcsrtombs(&buf[0], &wptr, bufSize, &st);
            return (res >= 0) ? String(buf.begin(), buf.begin() + res) : "?";
#endif
        }
        //--------------------------------------------------------------------------

        String NarrowToUtf8(const String& str)
        {
            KMP_PROFILE_FUNCTION();

#if defined (KMP_PLATFORM_WINDOWS)
            const auto size = MultiByteToWideChar(CP_ACP, MB_COMPOSITE, str.c_str(), static_cast<int>(str.length()), nullptr, 0);
            WString u16Str(size, L'\0');
            MultiByteToWideChar(CP_ACP, MB_COMPOSITE, str.c_str(), static_cast<int>(str.length()), &u16Str[0], size);

            const auto u8Size = WideCharToMultiByte(CP_UTF8, 0, u16Str.c_str(), static_cast<int>(u16Str.length()), nullptr, 0, nullptr, nullptr);
            String u8Str(u8Size, '\0');
            WideCharToMultiByte(CP_UTF8, 0, u16Str.c_str(), static_cast<int>(u16Str.length()), &u8Str[0], u8Size, nullptr, nullptr);
            return u8Str;
#else
            return str;
#endif
        }
        //--------------------------------------------------------------------------

        String Utf8ToNarrow(const String& str)
        {
            KMP_PROFILE_FUNCTION();

#if defined (KMP_PLATFORM_WINDOWS)
            const auto u16Size = MultiByteToWideChar(CP_UTF8, 0, &str[0], static_cast<int>(str.size()), nullptr, 0);
            if (u16Size > 0)
            {
                WString u16Str(u16Size, L'\0');
                MultiByteToWideChar(CP_UTF8, 0, &str[0], static_cast<int>(str.size()), &u16Str[0], u16Size);
                return WideToNarrow(u16Str);
            }

            return String();
#else
            return str;
#endif
        }
        //--------------------------------------------------------------------------

        Filepath::string_type NarrowToFilepath(const String& str)
        {
#if defined (KMP_PLATFORM_WINDOWS)
            return NarrowToWide(str);
#else
            return str;
#endif
        }
        //--------------------------------------------------------------------------
    }
}
