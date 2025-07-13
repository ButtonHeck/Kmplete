#pragma once

#include "Kmplete/Base/types.h"
#include "Kmplete/Base/pointers.h"
#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/macro.h"

#include <chrono>
#include <thread>
#include <mutex>
#include <fstream>

namespace Kmplete
{
    struct ProfileResult
    {
        String name;
        std::chrono::duration<double, std::micro> startTime;
        float elapsedTimeMicrosec;
        std::thread::id threadId;
    };
    //--------------------------------------------------------------------------

    struct ProfilingSession
    {
        String name;
    };
    //--------------------------------------------------------------------------

    class Profiler
    {
    public:
        KMP_DISABLE_COPY_MOVE(Profiler)

        KMP_NODISCARD KMP_API static Profiler& Get();

        KMP_API void BeginSession(const String& name, const Path& filepath);
        KMP_API void EndSession();
        KMP_API void WriteProfile(const ProfileResult& result);

    private:
        Profiler() noexcept;
        ~Profiler();

        void BeginSessionInternal(const String& name, const Path& filepath);
        void EndSessionInternal();
        void WriteHeader();
        void WriteFooter();

    private:
        std::mutex _mutex;
        UPtr<ProfilingSession> _currentSession;
        std::ofstream _outputFileStream;
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
        struct CleanupResult
        {
            char data[length];
        };

        template<size_t lengthSrc, size_t lengthRemove>
        consteval auto CleanupOutputString(const char(&src)[lengthSrc], const char(&remove)[lengthRemove])
        {
            CleanupResult<lengthSrc> result = {};

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
                    srcIndex += matchIndex;
                }

                result.data[dstIndex++] = (src[srcIndex] == '"' ? '\'' : src[srcIndex]);
                srcIndex++;
            }

            return result;
        }
    }
    //--------------------------------------------------------------------------
}

#define KMP_PROFILE true
#if (KMP_PROFILE && !defined (KMP_CONFIG_TYPE_PRODUCTION)) || defined (KMP_CONFIG_TYPE_RELWITHDEBINFO)
    #define _KMP_PROFILE_SCOPE_LINE2(name, line) \
        constexpr auto fixedNameCdecl##line = ::Kmplete::ProfilerUtils::CleanupOutputString(name, "__cdecl ");\
        constexpr auto fixedNameKmplete##line = ::Kmplete::ProfilerUtils::CleanupOutputString(fixedNameCdecl##line.data, "Kmplete::");\
        ::Kmplete::ProfilerTimer timer##line(fixedNameKmplete##line.data)
    #define _KMP_PROFILE_SCOPE_LINE(name, line) _KMP_PROFILE_SCOPE_LINE2(name, line)

    #define KMP_PROFILE_BEGIN_SESSION(name, filepath) ::Kmplete::Profiler::Get().BeginSession(name, filepath)
    #define KMP_PROFILE_END_SESSION() ::Kmplete::Profiler::Get().EndSession()
    #define KMP_PROFILE_SCOPE(name) _KMP_PROFILE_SCOPE_LINE(name, __LINE__)
    #define KMP_PROFILE_FUNCTION() KMP_PROFILE_SCOPE(KMP_FUNC_SIG)
#else
    #define KMP_PROFILE_BEGIN_SESSION(name, filepath)
    #define KMP_PROFILE_END_SESSION()
    #define KMP_PROFILE_SCOPE(name)
    #define KMP_PROFILE_FUNCTION()
#endif