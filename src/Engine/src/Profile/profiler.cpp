#include "Kmplete/Profile/profiler.h"
#include "Kmplete/Log/log.h"
#include "Kmplete/Filesystem/filesystem.h"

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
        InternalBeginSession(name, filepath);
    }
    //--------------------------------------------------------------------------

    void Profiler::EndSession()
    {
        KMP_MB_UNUSED std::lock_guard lock(_mutex);
        InternalEndSession();
    }
    //--------------------------------------------------------------------------

    void Profiler::InternalBeginSession(const String& name, const Path& filepath)
    {
        if (_currentSession)
        {
            KMP_LOG_ERROR("Profiler: cannot start session '{}' when session '{}' already started", name, _currentSession->name);
            InternalEndSession();
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

    void Profiler::InternalEndSession()
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
}