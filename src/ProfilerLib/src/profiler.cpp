#include "Kmplete/Profile/profiler.h"

#if defined(KMP_PROFILE)

#include "Kmplete/Log/log.h"

#include <iomanip>


namespace Kmplete
{
    Profiler::Profiler() noexcept
        : _level(0)
        , _active(true)
        , _currentSession(nullptr)
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

    void Profiler::SetLevel(unsigned int level)
    {
        _level = level;
    }
    //--------------------------------------------------------------------------

    unsigned int Profiler::GetLevel() const
    {
        return _level;
    }
    //--------------------------------------------------------------------------

    void Profiler::SetActive(bool active)
    {
        _active = active;
        KMP_LOG_INFO("activated: {}", _active);
    }
    //--------------------------------------------------------------------------

    bool Profiler::IsActive() const
    {
        return _active;
    }
    //--------------------------------------------------------------------------

    void Profiler::BeginSession(const String& name, const Filepath& filepath, int storageSize)
    {
        std::lock_guard lock(_mutex);
        _BeginSessionInternal(name, filepath, storageSize);
    }
    //--------------------------------------------------------------------------

    void Profiler::EndSession()
    {
        std::lock_guard lock(_mutex);
        _EndSessionInternal();
    }
    //--------------------------------------------------------------------------

    void Profiler::_BeginSessionInternal(const String& name, const Filepath& filepath, int storageSize)
    {
        if (_currentSession)
        {
            KMP_LOG_ERROR("cannot start session '{}' when session '{}' already started", name, _currentSession->name);
            _EndSessionInternal();
        }

        _storageSize = storageSize;
        _storeCycles = 0;
        _profileResults.reserve(storageSize);
        _outputFilepath = filepath;
        _currentSession.reset(new ProfilingSession({ name, 0 }));
    }
    //--------------------------------------------------------------------------

    void Profiler::_EndSessionInternal()
    {
        if (_currentSession)
        {
            std::ofstream outputFileStream(_outputFilepath);
            if (outputFileStream.is_open())
            {
                _WriteProfileHeader(outputFileStream);
                if (_storeCycles > 0)
                {
                    _WriteProfileResultsFromIntermediate(outputFileStream);
                }
                _WriteProfileResults(outputFileStream);
                _WriteProfileFooter(outputFileStream);

                outputFileStream.flush();
                outputFileStream.close();
            }
            else
            {
                KMP_LOG_ERROR("failed to open profiling session '{}' file '{}'", _currentSession->name, _outputFilepath);
            }            
        }

        _currentSession.reset(nullptr);
        _profileResults.clear();
    }
    //--------------------------------------------------------------------------

    void Profiler::_WriteProfileHeader(std::ofstream& outputFileStream) const
    {
        outputFileStream
            << R"rjs({"otherData":{"profileCount":")rjs"
            << _currentSession->profilesCount
            << R"rjs("},"traceEvents":[{})rjs";
    }
    //--------------------------------------------------------------------------

    void Profiler::_WriteProfileResults(std::ofstream& outputFileStream) const
    {
        for (const auto& profileResult : _profileResults)
        {
            const auto elapsedMicroseconds = std::chrono::duration<float, std::micro>(profileResult.end - profileResult.start).count();
            const auto start = std::chrono::duration<double, std::micro>(profileResult.start.time_since_epoch()).count();

            outputFileStream
                << std::setprecision(3) << std::fixed
                << R"rjs(,{"cat":"function","dur":)rjs"
                << elapsedMicroseconds
                << R"rjs(,"name":")rjs"
                << profileResult.name
                << R"rjs(","ph":"X","pid":0,"tid":)rjs"
                << profileResult.threadId
                << R"rjs(,"ts":)rjs"
                << start
                << "}";
        }
    }
    //--------------------------------------------------------------------------

    void Profiler::_WriteProfileResultsToIntermediate() const
    {
        const auto intermediateStoragePath = _CreateIntermediateFilepath(_storeCycles);

        std::ofstream intermediateFileStream(intermediateStoragePath);
        if (intermediateFileStream.is_open())
        {
            _WriteProfileResults(intermediateFileStream);

            intermediateFileStream.flush();
            intermediateFileStream.close();

            KMP_LOG_INFO("write to intermediate file '{}'", intermediateStoragePath);
        }
        else
        {
            KMP_LOG_ERROR("failed to write to intermediate file '{}'", intermediateStoragePath);
        }
    }
    //--------------------------------------------------------------------------

    void Profiler::_WriteProfileResultsFromIntermediate(std::ofstream& outputFileStream) const
    {
        int storeCycle = 0;
        while (storeCycle < _storeCycles)
        {
            const auto intermediateStoragePath = _CreateIntermediateFilepath(storeCycle);

            std::ifstream intermediateFileInputStream(intermediateStoragePath);
            if (intermediateFileInputStream.is_open())
            {
                std::stringstream intermediateBuffer;
                intermediateBuffer << intermediateFileInputStream.rdbuf();
                intermediateFileInputStream.close();

                outputFileStream << intermediateBuffer.str();

                if (!std::filesystem::remove(intermediateStoragePath))
                {
                    KMP_LOG_WARN("failed to remove temporary intermediate file '{}'", intermediateStoragePath);
                }
            }
            else
            {
                KMP_LOG_ERROR("failed to write from intermediate file '{}'", intermediateStoragePath);
            }

            outputFileStream.flush();

            KMP_LOG_INFO("write from intermediate file '{}'", intermediateStoragePath);

            ++storeCycle;
        }
    }
    //--------------------------------------------------------------------------

    void Profiler::_WriteProfileFooter(std::ofstream& outputFileStream) const
    {
        outputFileStream << "]}";
    }
    //--------------------------------------------------------------------------

    Filepath Profiler::_CreateIntermediateFilepath(int intermediateCount) const
    {
        const auto intermediateStorageFilename = _outputFilepath.stem().generic_u8string().append(std::to_string(intermediateCount));
        return _outputFilepath.parent_path() / intermediateStorageFilename;
    }
    //--------------------------------------------------------------------------

    void Profiler::_BeginNewCycle()
    {
        ++_storeCycles;
        _profileResults.clear();
        _profileResults.reserve(_storageSize);
    }
    //--------------------------------------------------------------------------


    ProfilerTimer::ProfilerTimer(const char* name, unsigned int level /*= 0*/)
        : _name(name)
        , _skip(Profiler::Get().GetLevel() < level || !Profiler::Get().IsActive())
        , _start(_skip ? std::chrono::high_resolution_clock::time_point() : std::chrono::high_resolution_clock::now())
    {}
    //--------------------------------------------------------------------------

    ProfilerTimer::~ProfilerTimer()
    {
        if (_skip)
        {
            return;
        }

        const auto end = std::chrono::high_resolution_clock::now();

        auto& profiler = Profiler::Get();

        if (!profiler._currentSession)
        {
            return;
        }

        {
            std::lock_guard lock(profiler._mutex);
            ++profiler._currentSession->profilesCount;
            profiler._profileResults.emplace_back(_name, _start, end, std::this_thread::get_id());

            if (profiler._profileResults.size() < static_cast<size_t>(profiler._storageSize))
            {
                return;
            }

            profiler._WriteProfileResultsToIntermediate();
            profiler._BeginNewCycle();
        }
    }
    //--------------------------------------------------------------------------

    void ProfilerTimer::SetName(const char* name)
    {
        _name = name;
    }
    //--------------------------------------------------------------------------
}
#endif