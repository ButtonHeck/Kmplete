#include "Kmplete/Profile/profiler.h"

#if (KMP_PROFILE && !defined (KMP_CONFIG_TYPE_PRODUCTION)) || defined (KMP_CONFIG_TYPE_RELWITHDEBINFO)

#include "Kmplete/Log/log.h"

namespace Kmplete
{
    Profiler::Profiler() noexcept
        : _currentSession(nullptr)
        , _storageSize(0)
        , _storeCycles(0)
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

    void Profiler::BeginSessionInternal(const String& name, const Path& filepath, int storageSize)
    {
        if (_currentSession)
        {
            KMP_LOG_ERROR("Profiler: cannot start session '{}' when session '{}' already started", name, _currentSession->name);
            EndSessionInternal();
        }

        _storageSize = storageSize;
        _storeCycles = 0;
        _profileResults.reserve(storageSize);
        _outputFilePath = filepath;
        _currentSession.reset(new ProfilingSession({ name, 0 }));
    }
    //--------------------------------------------------------------------------

    void Profiler::EndSessionInternal()
    {
        if (_currentSession)
        {
            std::ofstream outputFileStream(_outputFilePath);
            if (outputFileStream.is_open())
            {
                outputFileStream
                    << R"rjs({"otherData":{"profileCount":")rjs" 
                    << _currentSession->profilesCount 
                    << R"rjs("},"traceEvents":[{})rjs";

                if (_storeCycles == 0)
                {
                    WriteProfileResults(outputFileStream);
                }
                else
                {
                    int storeCycle = 0;
                    while (storeCycle <= _storeCycles)
                    {
                        const auto intermediateStorageFilename = _outputFilePath.stem().generic_u8string().append("_").append(std::to_string(storeCycle));
                        const auto intermediateStoragePath = _outputFilePath.parent_path() / intermediateStorageFilename;
                        std::ifstream intermediateFileInputStream(intermediateStoragePath);
                        if (intermediateFileInputStream.is_open())
                        {
                            std::stringstream ss;
                            ss << intermediateFileInputStream.rdbuf();
                            intermediateFileInputStream.close();

                            const String intermediateProfiles = ss.str();
                            outputFileStream << intermediateProfiles;
                            outputFileStream.flush();
                            
                            std::filesystem::remove(intermediateStoragePath);
                        }

                        ++storeCycle;
                    }

                    WriteProfileResults(outputFileStream);
                }

                outputFileStream << "]}";
                outputFileStream.flush();
                outputFileStream.close();
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

    void Profiler::WriteProfileResults(std::ofstream& outputFileStream)
    {
        for (const auto& profileResult : _profileResults)
        {
            outputFileStream
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

        auto& profiler = Profiler::Get();

        if (!profiler._currentSession)
        {
            return;
        }

        KMP_MB_UNUSED std::lock_guard lock(profiler._mutex);
        ++profiler._currentSession->profilesCount;
        profiler._profileResults.emplace_back(_name, start, elapsedTimeMicroseconds, std::this_thread::get_id());

        if (profiler._profileResults.size() < profiler._storageSize)
        {
            return;
        }

        const auto intermediateStorageFilename = profiler._outputFilePath.stem().generic_u8string().append("_").append(std::to_string(profiler._storeCycles));
        const auto intermediateStoragePath = profiler._outputFilePath.parent_path() / intermediateStorageFilename;

        std::ofstream intermediateFileStream(intermediateStoragePath);
        if (intermediateFileStream.is_open())
        {
            profiler.WriteProfileResults(intermediateFileStream);
            intermediateFileStream.flush();
            intermediateFileStream.close();
        }

        ++profiler._storeCycles;
        profiler._profileResults.clear();
        profiler._profileResults.reserve(profiler._storageSize);
    }
    //--------------------------------------------------------------------------
}
#endif