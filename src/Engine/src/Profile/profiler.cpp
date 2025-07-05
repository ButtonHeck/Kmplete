#include "Kmplete/Profile/profiler.h"
#include "Kmplete/Log/log.h"
#include "Kmplete/Filesystem/filesystem.h"
#include "Kmplete/Utils/string_utils.h"

namespace Kmplete
{
    Profiler::Profiler() noexcept
        : _currentSession(nullptr)
    {}
    //--------------------------------------------------------------------------

    Profiler::~Profiler()
    {
        EndSession();
    }
    //--------------------------------------------------------------------------

    Profiler& Profiler::Get()
    {
        static Profiler profilerInstance;
        return profilerInstance;
    }
    //--------------------------------------------------------------------------

    void Profiler::BeginSession(const String& name, const Path& filepath)
    {
        KMP_MB_UNUSED std::lock_guard lock(_mutex);
        BeginSessionInternal(name, filepath);
    }
    //--------------------------------------------------------------------------

    void Profiler::EndSession()
    {
        KMP_MB_UNUSED std::lock_guard lock(_mutex);
        EndSessionInternal();
    }
    //--------------------------------------------------------------------------

    void Profiler::WriteProfile(const ProfileResult& result)
    {
        if (_currentSession)
        {
            std::ostringstream json;
            Utils::ToSStream(json, 
                std::setprecision(3), std::fixed,
                ",{\"cat\":\"function\",\"dur\":", 
                result.elapsedTimeMicrosec,
                ",\"name\":\"",
                result.name,
                "\",\"ph\":\"X\",\"pid\":0,\"tid\":",
                result.threadId,
                ",\"ts\":",
                result.startTime.count(), "}"
            );

            KMP_MB_UNUSED std::lock_guard lock(_mutex);
            _outputFileStream << json.str();
        }
    }
    //--------------------------------------------------------------------------

    void Profiler::BeginSessionInternal(const String& name, const Path& filepath)
    {
        if (_currentSession)
        {
            KMP_LOG_ERROR("Profiler: cannot start session '{}' when session '{}' already started", name, _currentSession->name);
            EndSessionInternal();
        }

        _outputFileStream.open(filepath);
        if (_outputFileStream.is_open())
        {
            _currentSession.reset(new ProfilingSession({name}));
            WriteHeader();
        }
        else
        {
            KMP_LOG_ERROR("Profiler: failed to open profiling session '{}' file '{}'", name, filepath);
        }
    }
    //--------------------------------------------------------------------------

    void Profiler::EndSessionInternal()
    {
        if (_currentSession)
        {
            WriteFooter();
            _outputFileStream.close();
            _currentSession.reset(nullptr);
        }
    }
    //--------------------------------------------------------------------------

    void Profiler::WriteHeader()
    {
        _outputFileStream << "{\"otherData\": {},\"traceEvents\":[{}";
        _outputFileStream.flush();
    }
    //--------------------------------------------------------------------------

    void Profiler::WriteFooter()
    {
        _outputFileStream << "]}";
        _outputFileStream.flush();
    }
    //--------------------------------------------------------------------------


    ProfilerTimer::ProfilerTimer(const char* name)
        : _name(name)
    {}
    //--------------------------------------------------------------------------

    ProfilerTimer::~ProfilerTimer()
    {
        const auto elapsedTimeMicroseconds = Peek() * 1000.0f;
        const auto start = std::chrono::duration<double, std::micro>(_last.time_since_epoch());

        Profiler::Get().WriteProfile(ProfileResult{_name, start, elapsedTimeMicroseconds, std::this_thread::get_id()});
    }
    //--------------------------------------------------------------------------
}