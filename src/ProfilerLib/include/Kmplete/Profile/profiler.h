#pragma once

#define KMP_PROFILE true
#if (KMP_PROFILE && !defined (KMP_CONFIG_TYPE_PRODUCTION)) || defined (KMP_CONFIG_TYPE_RELWITHDEBINFO)

#include "Kmplete/Base/types.h"
#include "Kmplete/Base/pointers.h"
#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/macro.h"

#include <fstream>
#include <chrono>
#include <thread>
#include <mutex>
#include <type_traits>

namespace Kmplete
{
    struct ProfileResult
    {
        String name;
        std::chrono::duration<double, std::micro> startTime;
        float elapsedTimeMicrosec;
        std::thread::id threadId;
    };
    static_assert(std::is_move_constructible_v<ProfileResult> == true);
    //--------------------------------------------------------------------------

    struct ProfilingSession
    {
        String name;
        int profilesCount;
    };
    //--------------------------------------------------------------------------

    class Profiler
    {
    public:
        KMP_DISABLE_COPY_MOVE(Profiler)

        KMP_NODISCARD KMP_API static Profiler& Get();

        KMP_API void BeginSession(const String& name, const Path& filepath, int storageSize);
        KMP_API void EndSession();

    private:
        Profiler() noexcept;
        ~Profiler();

        void BeginSessionInternal(const String& name, const Path& filepath, int storageSize);
        void EndSessionInternal();

        void WriteProfileHeader(std::ofstream& outputFileStream) const;
        void WriteProfileResults(std::ofstream& outputFileStream) const;
        void WriteProfileResultsToIntermediate() const;
        void WriteProfileResultsFromIntermediate(std::ofstream& outputFileStream) const;
        void WriteProfileFooter(std::ofstream& outputFileStream) const;
        Path CreateIntermediateFilePath(int intermediateCount) const;
        void BeginNewCycle();

    private:
        friend class ProfilerTimer;

    private:
        std::mutex _mutex;
        UPtr<ProfilingSession> _currentSession;
        Path _outputFilePath;
        std::vector<ProfileResult> _profileResults;
        int _storageSize;
        int _storeCycles;
    };
    //--------------------------------------------------------------------------

    class ProfilerTimer
    {
    public:
        KMP_DISABLE_COPY_MOVE(ProfilerTimer)

        KMP_API explicit ProfilerTimer(const char* name);
        KMP_API ~ProfilerTimer();

    private:
        const char* _name;
        std::chrono::high_resolution_clock::time_point _last;
    };
    //--------------------------------------------------------------------------

    namespace ProfilerUtils
    {
        template<size_t length>
        struct ReplaceResult
        {
            char data[length];
        };

        template<size_t lengthSrc, size_t lengthRemove, size_t lengthReplace>
        consteval auto ReplaceString(const char(&src)[lengthSrc], const char(&remove)[lengthRemove], const char(&replace)[lengthReplace])
        {
            ReplaceResult<lengthSrc> result = {};

            size_t srcIndex = 0;
            size_t dstIndex = 0;
            while (srcIndex < lengthSrc)
            {
                size_t matchIndex = 0;
                while ((matchIndex < lengthRemove - 1) && (srcIndex + matchIndex < lengthSrc - 1) && (src[srcIndex + matchIndex] == remove[matchIndex]))
                {
                    matchIndex++;
                }

                if (matchIndex == lengthRemove - 1)
                {
                    size_t replaceIndex = 0;
                    while (replaceIndex < lengthReplace - 1)
                    {
                        result.data[dstIndex++] = replace[replaceIndex++];
                    }

                    srcIndex += matchIndex;
                }
                else
                {
                    result.data[dstIndex++] = src[srcIndex];
                    srcIndex++;
                }
            }

            return result;
        }
    }
    //--------------------------------------------------------------------------
}

#define _KMP_PROFILE_SCOPE_LINE2(name, line) \
    constexpr auto fixedNameCdecl##line      = ::Kmplete::ProfilerUtils::ReplaceString(name, "__cdecl ", "");\
    constexpr auto fixedNameKmplete##line    = ::Kmplete::ProfilerUtils::ReplaceString(fixedNameCdecl##line.data, "Kmplete::", "");\
    constexpr auto fixedNameUPtr##line       = ::Kmplete::ProfilerUtils::ReplaceString(fixedNameKmplete##line.data, "std::unique_ptr", "UPtr");\
    constexpr auto fixedNamePtr##line        = ::Kmplete::ProfilerUtils::ReplaceString(fixedNameUPtr##line.data, "std::shared_ptr", "Ptr");\
    constexpr auto fixedNameString##line     = ::Kmplete::ProfilerUtils::ReplaceString(fixedNamePtr##line.data, "std::basic_string<char,struct std::char_traits<char>,class std::allocator<char> >", "String");\
    constexpr auto fixedNameWString##line    = ::Kmplete::ProfilerUtils::ReplaceString(fixedNameString##line.data, "std::basic_string<wchar_t,struct std::char_traits<wchar_t>,class std::allocator<wchar_t> >", "WString");\
    constexpr auto fixedNameInt64##line      = ::Kmplete::ProfilerUtils::ReplaceString(fixedNameWString##line.data, "__int64", "int64");\
    constexpr auto fixedNamePath##line       = ::Kmplete::ProfilerUtils::ReplaceString(fixedNameInt64##line.data, "std::filesystem::path", "Path");\
    constexpr auto fixedNameClass##line      = ::Kmplete::ProfilerUtils::ReplaceString(fixedNamePath##line.data, "class ", "");\
    constexpr auto fixedNameStruct##line     = ::Kmplete::ProfilerUtils::ReplaceString(fixedNameClass##line.data, "struct ", "");\
    constexpr auto fixedNameVector##line     = ::Kmplete::ProfilerUtils::ReplaceString(fixedNameStruct##line.data, "std::vector", "Vector");\
    constexpr auto fixedNameVoidParams##line = ::Kmplete::ProfilerUtils::ReplaceString(fixedNameVector##line.data, "(void)", "()");\
    ::Kmplete::ProfilerTimer timer##line(fixedNameVoidParams##line.data)
#define _KMP_PROFILE_SCOPE_LINE(name, line) _KMP_PROFILE_SCOPE_LINE2(name, line)

#define KMP_PROFILE_BEGIN_SESSION(name, filepath, storageSize) ::Kmplete::Profiler::Get().BeginSession(name, filepath, storageSize)
#define KMP_PROFILE_END_SESSION() ::Kmplete::Profiler::Get().EndSession()
#define KMP_PROFILE_SCOPE(name) _KMP_PROFILE_SCOPE_LINE(name, __LINE__)
#define KMP_PROFILE_FUNCTION() KMP_PROFILE_SCOPE(KMP_FUNC_SIG)

#else
#define KMP_PROFILE_BEGIN_SESSION(name, filepath, storageSize)
#define KMP_PROFILE_END_SESSION()
#define KMP_PROFILE_SCOPE(name)
#define KMP_PROFILE_FUNCTION()
#endif