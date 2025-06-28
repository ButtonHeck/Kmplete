#pragma once

#include "Kmplete/Base/types.h"
#include "Kmplete/Base/pointers.h"
#include "Kmplete/Base/kmplete_api.h"

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
}