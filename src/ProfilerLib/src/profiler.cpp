#include "Kmplete/Profile/profiler.h"
#include "Kmplete/Log/log.h"

#include <fstream>

namespace Kmplete
{
    Profiler::Profiler() noexcept
        : _currentSession(nullptr)
        , _storageSize(0)
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

    void Profiler::BeginSession(const String& name, const Path& filepath, int storageSize)
    {
        KMP_MB_UNUSED std::lock_guard lock(_mutex);
        BeginSessionInternal(name, filepath, storageSize);
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
            KMP_MB_UNUSED std::lock_guard lock(_mutex);
            ++_currentSession->profilesCount;
            _profileResults.push_back(result);
        }
    }
    //--------------------------------------------------------------------------

    void Profiler::BeginSessionInternal(const String& name, const Path& filepath, int storageSize)
    {
        if (_currentSession)
        {
            KMP_LOG_ERROR("Profiler: cannot start session '{}' when session '{}' already started", name, _currentSession->name);
            EndSessionInternal();
        }

        _storageSize = storageSize;
        _profileResults.reserve(storageSize);
        _outputFilePath = filepath;
        _currentSession.reset(new ProfilingSession({ name, 0 }));
    }
    //--------------------------------------------------------------------------

    void Profiler::EndSessionInternal()
    {
        if (_currentSession)
        {
            std::ofstream _outputFileStream(_outputFilePath);
            if (_outputFileStream.is_open())
            {
                _outputFileStream 
                    << R"rjs({"otherData":{"profileCount":")rjs" 
                    << _currentSession->profilesCount 
                    << R"rjs("},"traceEvents":[{})rjs";

                for (const auto& profileResult : _profileResults)
                {
                    _outputFileStream 
                        << std::setprecision(3) << std::fixed
                        << R"rjs(,{"cat":"function","dur":)rjs"
                        << profileResult.elapsedTimeMicrosec
                        << R"rjs(,"name":")rjs"
                        << profileResult.name
                        << R"rjs(","ph":"X","pid":0,"tid":)rjs"
                        << profileResult.threadId
                        << R"rjs(,"ts":)rjs"
                        << profileResult.startTime.count()
                        << "}";
                }

                _outputFileStream << "]}";

                _outputFileStream.flush();
                _outputFileStream.close();
            }
            else
            {
                KMP_LOG_ERROR("Profiler: failed to open profiling session '{}' file '{}'", _currentSession->name, _outputFilePath);
            }            
        }

        _currentSession.reset(nullptr);
        _profileResults.clear();
    }
    //--------------------------------------------------------------------------


    ProfilerTimer::ProfilerTimer(const char* name)
        : _name(name)
        , _last(std::chrono::high_resolution_clock::now())
    {}
    //--------------------------------------------------------------------------

    ProfilerTimer::~ProfilerTimer()
    {
        const auto elapsedTimeMicroseconds = std::chrono::duration<float, std::micro>(std::chrono::high_resolution_clock::now() - _last).count();
        const auto start = std::chrono::duration<double, std::micro>(_last.time_since_epoch());

        Profiler::Get().WriteProfile(ProfileResult{_name, start, elapsedTimeMicroseconds, std::this_thread::get_id()});
    }
    //--------------------------------------------------------------------------
}