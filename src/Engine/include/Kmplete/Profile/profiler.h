#pragma once

#include "Kmplete/Base/types.h"
#include "Kmplete/Base/pointers.h"
#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Core/timer.h"
#include "Kmplete/Utils/function_utils.h"

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

        void InternalBeginSession(const String& name, const Path& filepath);
        void InternalEndSession();
        void WriteHeader();
        void WriteFooter();

    private:
        std::mutex _mutex;
        UPtr<ProfilingSession> _currentSession;
        std::ofstream _outputFileStream;
    };
    //--------------------------------------------------------------------------

    class ProfilerTimer : public Timer
    {
    public:
        KMP_DISABLE_COPY_MOVE(ProfilerTimer)

        KMP_API explicit ProfilerTimer(const char* name);
        KMP_API ~ProfilerTimer();

    private:
        const char* _name;
    };
    //--------------------------------------------------------------------------
}

#define KMP_PROFILE true
#if KMP_PROFILE && !defined (KMP_PRODUCTION_BUILD)
    #define KMP_PROFILE_BEGIN_SESSION(name, filepath) ::Kmplete::Profiler::Get().BeginSession(name, filepath)
    #define KMP_PROFILE_END_SESSION() ::Kmplete::Profiler::Get().EndSession()
    #define KMP_PROFILE_SCOPE_LINE2(name, line) ::Kmplete::ProfilerTimer timer##line(name)
    #define KMP_PROFILE_SCOPE_LINE(name, line) KMP_PROFILE_SCOPE_LINE2(name, line)
    #define KMP_PROFILE_SCOPE(name) KMP_PROFILE_SCOPE_LINE(name, __LINE__)
    #define KMP_PROFILE_FUNCTION() KMP_PROFILE_SCOPE(KMP_FUNC_SIG)
#else
    #define KMP_PROFILE_BEGIN_SESSION(name, filepath)
    #define KMP_PROFILE_END_SESSION()
    #define KMP_PROFILE_SCOPE(name)
    #define KMP_PROFILE_FUNCTION()
#endif