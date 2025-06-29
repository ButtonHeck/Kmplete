#pragma once

#include "Kmplete/Base/types.h"
#include "Kmplete/Base/pointers.h"
#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Core/timer.h"

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
        std::chrono::microseconds elapsedTime;
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

#define KMP_PROFILE 1
#if KMP_PROFILE && !defined (KMP_PRODUCTION_BUILD)
    #if defined(__GNUC__) || (defined(__MWERKS__) && (__MWERKS__ >= 0x3000)) || (defined(__ICC) && (__ICC >= 600)) || defined(__ghs__)
        #define KMP_FUNC_SIG __PRETTY_FUNCTION__
    #elif defined(__DMC__) && (__DMC__ >= 0x810)
        #define KMP_FUNC_SIG __PRETTY_FUNCTION__
    #elif (defined(__FUNCSIG__) || (_MSC_VER))
        #define KMP_FUNC_SIG __FUNCSIG__
    #elif (defined(__INTEL_COMPILER) && (__INTEL_COMPILER >= 600)) || (defined(__IBMCPP__) && (__IBMCPP__ >= 500))
        #define KMP_FUNC_SIG __FUNCTION__
    #elif defined(__BORLANDC__) && (__BORLANDC__ >= 0x550)
        #define KMP_FUNC_SIG __FUNC__
    #elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901)
        #define KMP_FUNC_SIG __func__
    #elif defined(__cplusplus) && (__cplusplus >= 201103)
        #define KMP_FUNC_SIG __func__
    #else
        #define KMP_FUNC_SIG "KMP_FUNC_SIG unknown!"
    #endif

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