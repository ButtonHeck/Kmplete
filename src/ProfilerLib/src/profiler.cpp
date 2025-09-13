#include "Kmplete/Profile/profiler.h"

#if defined(KMP_PROFILE)

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

    void Profiler::BeginSession(const String& name, const Filepath& filepath, int storageSize)
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

    void Profiler::BeginSessionInternal(const String& name, const Filepath& filepath, int storageSize)
    {
        if (_currentSession)
        {
            KMP_LOG_ERROR("cannot start session '{}' when session '{}' already started", name, _currentSession->name);
            EndSessionInternal();
        }

        _storageSize = storageSize;
        _storeCycles = 0;
        _profileResults.reserve(storageSize);
        _outputFilepath = filepath;
        _currentSession.reset(new ProfilingSession({ name, 0 }));
    }
    //--------------------------------------------------------------------------

    void Profiler::EndSessionInternal()
    {
        if (_currentSession)
        {
            std::ofstream outputFileStream(_outputFilepath);
            if (outputFileStream.is_open())
            {
                WriteProfileHeader(outputFileStream);
                if (_storeCycles > 0)
                {
                    WriteProfileResultsFromIntermediate(outputFileStream);
                }
                WriteProfileResults(outputFileStream);
                WriteProfileFooter(outputFileStream);

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

    void Profiler::WriteProfileHeader(std::ofstream& outputFileStream) const
    {
        outputFileStream
            << R"rjs({"otherData":{"profileCount":")rjs"
            << _currentSession->profilesCount
            << R"rjs("},"traceEvents":[{})rjs";
    }
    //--------------------------------------------------------------------------

    void Profiler::WriteProfileResults(std::ofstream& outputFileStream) const
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

    void Profiler::WriteProfileResultsToIntermediate() const
    {
        const auto intermediateStoragePath = CreateIntermediateFilepath(_storeCycles);

        std::ofstream intermediateFileStream(intermediateStoragePath);
        if (intermediateFileStream.is_open())
        {
            WriteProfileResults(intermediateFileStream);

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

    void Profiler::WriteProfileResultsFromIntermediate(std::ofstream& outputFileStream) const
    {
        int storeCycle = 0;
        while (storeCycle < _storeCycles)
        {
            const auto intermediateStoragePath = CreateIntermediateFilepath(storeCycle);

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

    void Profiler::WriteProfileFooter(std::ofstream& outputFileStream) const
    {
        outputFileStream << "]}";
    }
    //--------------------------------------------------------------------------

    Filepath Profiler::CreateIntermediateFilepath(int intermediateCount) const
    {
        const auto intermediateStorageFilename = _outputFilepath.stem().generic_u8string().append(std::to_string(intermediateCount));
        return _outputFilepath.parent_path() / intermediateStorageFilename;
    }
    //--------------------------------------------------------------------------

    void Profiler::BeginNewCycle()
    {
        ++_storeCycles;
        _profileResults.clear();
        _profileResults.reserve(_storageSize);
    }
    //--------------------------------------------------------------------------


    ProfilerTimer::ProfilerTimer(const char* name)
        : _name(name)
        , _start(std::chrono::high_resolution_clock::now())
    {}
    //--------------------------------------------------------------------------

    ProfilerTimer::~ProfilerTimer()
    {
        const auto end = std::chrono::high_resolution_clock::now();

        auto& profiler = Profiler::Get();

        if (!profiler._currentSession)
        {
            return;
        }

        KMP_MB_UNUSED std::lock_guard lock(profiler._mutex);
        ++profiler._currentSession->profilesCount;
        profiler._profileResults.emplace_back(_name, _start, end, std::this_thread::get_id());

        if (profiler._profileResults.size() < static_cast<size_t>(profiler._storageSize))
        {
            return;
        }

        profiler.WriteProfileResultsToIntermediate();
        profiler.BeginNewCycle();
    }
    //--------------------------------------------------------------------------
}
#endif