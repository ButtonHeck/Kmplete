#include "Kmplete/Core/system_metrics_manager.h"
#include "Kmplete/Core/assertion.h"
#include "Kmplete/Log/log.h"

#if defined (KMP_PLATFORM_WINDOWS)
    #include <Windows.h>
    #include <Psapi.h>
    #include <TlHelp32.h>
#else
    #include <sys/types.h>
    #include <sys/sysinfo.h>
    #include <sys/times.h>
    #include <unistd.h>
    #include <cstring>
    #include <cstdio>
    #include <cstdlib>
#endif


namespace Kmplete
{
    SystemMetricsManager::SystemMetricsManager()
        : KMP_PROFILE_CONSTRUCTOR_START_BASE_CLASS()
          _lastCPUTimestamp(0ULL)
        , _lastSysCPUTimestamp(0ULL)
        , _lastUserCPUTimestamp(0ULL)
        , _currentProcessId(0)
#if defined (KMP_PLATFORM_WINDOWS)
        , _windowsProcessHandle(nullptr)
#endif
    {
        _Initialize();

        KMP_PROFILE_CONSTRUCTOR_END()
    }
    //--------------------------------------------------------------------------

    bool SystemMetricsManager::Update(SystemMetricsUpdateMode updateMode) KMP_PROFILING(ProfileLevelImportant)
    {
#if defined (KMP_PLATFORM_WINDOWS)
        if (!_windowsProcessHandle)
        {
            _windowsProcessHandle = GetCurrentProcess();
            if (!_windowsProcessHandle)
            {
                KMP_LOG_ERROR("update failed on GetCurrentProcess handle");
                return false;
            }
        }
#endif

        if (updateMode & SystemMetricsManager::SystemMetricsUpdateMode::NumThreads)
        {
            if (!_UpdateNumThreads())
            {
                return false;
            }
        }

        if (updateMode & SystemMetricsManager::SystemMetricsUpdateMode::MemoryUsed)
        {
            if (!_UpdateMemoryUsed())
            {
                return false;
            }
        }

        if (updateMode & SystemMetricsManager::SystemMetricsUpdateMode::CPUUsed)
        {
            if (!_UpdateCPUUsed())
            {
                return false;
            }
        }

        if (updateMode & SystemMetricsManager::SystemMetricsUpdateMode::StackUsed)
        {
            if (!_UpdateCurrentThreadStackUsed())
            {
                return false;
            }
        }

        return true;
    }}
    //--------------------------------------------------------------------------

    const SystemMetricsManager::SystemMetrics& SystemMetricsManager::GetMetrics() const noexcept
    {
        return _systemMetrics;
    }
    //--------------------------------------------------------------------------

    void SystemMetricsManager::_Initialize() KMP_PROFILING(ProfileLevelAlways)
    {
#if defined (KMP_PLATFORM_WINDOWS)
        static_assert(sizeof(ULARGE_INTEGER) == sizeof(decltype(_lastCPUTimestamp)));
#endif

        if (!_InitializeProcessId())
        {
            return;
        }

        if (!_InitializeTotalMemory())
        {
            return;
        }

        if (!_InitializeNumProcessors())
        {
            return;
        }

        if (!_InitializeWindowsProcessHandle())
        {
            return;
        }

        if (!_InitializeCPUTimestamps())
        {
            return;
        }

        KMP_ASSERT(_systemMetrics.numProcessors);
        KMP_ASSERT(_systemMetrics.totalVirtualMemoryMib);
        KMP_ASSERT(_systemMetrics.totalPhysicalMemoryMib);
    }}
    //--------------------------------------------------------------------------

    bool SystemMetricsManager::_InitializeProcessId() KMP_PROFILING(ProfileLevelImportant)
    {
#if defined (KMP_PLATFORM_WINDOWS)
        _currentProcessId = GetCurrentProcessId();
#else
        _currentProcessId = getpid();
#endif

        return true;
    }}
    //--------------------------------------------------------------------------

    bool SystemMetricsManager::_InitializeTotalMemory() KMP_PROFILING(ProfileLevelImportant)
    {
        static constexpr auto MibDivisor = 1024.0 * 1024.0;
        
#if defined (KMP_PLATFORM_WINDOWS)
        MEMORYSTATUSEX memoryInfo;
        memoryInfo.dwLength = sizeof(MEMORYSTATUSEX);
        if (!static_cast<bool>(GlobalMemoryStatusEx(&memoryInfo)))
        {
            KMP_LOG_ERROR("initialization failed on GlobalMemoryStatusEx, error {}", GetLastError());
            return false;
        }

        _systemMetrics.totalVirtualMemoryMib = static_cast<float>(static_cast<double>(memoryInfo.ullTotalPageFile) / MibDivisor);
        _systemMetrics.totalPhysicalMemoryMib = static_cast<float>(static_cast<double>(memoryInfo.ullTotalPhys) / MibDivisor);
#else
        struct sysinfo memoryInfo;
        const auto error = sysinfo(&memoryInfo);
        if (error != 0)
        {
            KMP_LOG_ERROR("initialization failed on sysinfo");
            return false;
        }
    
        unsigned long totalVirtualMemory = memoryInfo.totalram;
        totalVirtualMemory += memoryInfo.totalswap;
        totalVirtualMemory *= memoryInfo.mem_unit;
    
        unsigned long totalPhysicalMemory = memoryInfo.totalram;
        totalPhysicalMemory *= memoryInfo.mem_unit;
    
        _systemMetrics.totalVirtualMemoryMib = static_cast<float>(static_cast<double>(totalVirtualMemory) / MibDivisor);
        _systemMetrics.totalPhysicalMemoryMib = static_cast<float>(static_cast<double>(totalPhysicalMemory) / MibDivisor);
#endif

        return true;
    }}
    //--------------------------------------------------------------------------

    bool SystemMetricsManager::_InitializeNumProcessors() KMP_PROFILING(ProfileLevelImportant)
    {
#if defined (KMP_PLATFORM_WINDOWS)
        SYSTEM_INFO systemInfo;
        GetSystemInfo(&systemInfo);
        _systemMetrics.numProcessors = systemInfo.dwNumberOfProcessors;
#elif defined (KMP_PLATFORM_MACOSX)
#else
        _systemMetrics.numProcessors = sysconf(_SC_NPROCESSORS_ONLN);
#endif

        return true;
    }}
    //--------------------------------------------------------------------------

    bool SystemMetricsManager::_InitializeWindowsProcessHandle() KMP_PROFILING(ProfileLevelImportant)
    {
#if defined (KMP_PLATFORM_WINDOWS)
        _windowsProcessHandle = GetCurrentProcess();
        if (!_windowsProcessHandle)
        {
            KMP_LOG_ERROR("initialization failed on GetCurrentProcess handle");
            return false;
        }
#else
#endif

        return true;
    }}
    //--------------------------------------------------------------------------

    bool SystemMetricsManager::_InitializeCPUTimestamps() KMP_PROFILING(ProfileLevelImportant)
    {
#if defined (KMP_PLATFORM_WINDOWS)
        FILETIME fTime, fSystem, fUser;
        const auto filetimeSize = sizeof(FILETIME);

        GetSystemTimeAsFileTime(&fTime);
        memcpy(&_lastCPUTimestamp, &fTime, filetimeSize);

        if (!static_cast<bool>(GetProcessTimes(reinterpret_cast<HANDLE>(_windowsProcessHandle), &fTime, &fTime, &fSystem, &fUser)))
        {
            KMP_LOG_ERROR("initialization failed on GetProcessTimes, error {}", GetLastError());
            return false;
        }

        memcpy(&_lastSysCPUTimestamp, &fSystem, filetimeSize);
        memcpy(&_lastUserCPUTimestamp, &fUser, filetimeSize);
#else
        struct tms timeSample;
        _lastCPUTimestamp = static_cast<decltype(_lastCPUTimestamp)>(times(&timeSample));
        _lastSysCPUTimestamp = static_cast<decltype(_lastSysCPUTimestamp)>(timeSample.tms_stime);
        _lastUserCPUTimestamp = static_cast<decltype(_lastUserCPUTimestamp)>(timeSample.tms_utime);        
#endif

        return true;
    }}
    //--------------------------------------------------------------------------

    bool SystemMetricsManager::_UpdateNumThreads() KMP_PROFILING(ProfileLevelImportantVerbose)
    {
#if defined (KMP_PLATFORM_WINDOWS)
        HANDLE threadSnapHandle = INVALID_HANDLE_VALUE;
        unsigned int numThreads = 0;
        threadSnapHandle = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
        if (threadSnapHandle == INVALID_HANDLE_VALUE)
        {
            KMP_LOG_WARN("failed to take snapshot of all running threads");
            _systemMetrics.numThreads = numThreads;
            return false;
        }
        else
        {
            THREADENTRY32 threadEntry; 
            threadEntry.dwSize = sizeof(THREADENTRY32);

            if (!Thread32First(threadSnapHandle, &threadEntry))
            {
                KMP_LOG_WARN("failed to retrieve information about the first thread");
                CloseHandle(threadSnapHandle);
            }
            else
            {
                do
                {
                    if (threadEntry.th32OwnerProcessID == _currentProcessId)
                    {
                        numThreads++;
                    }
                } while (Thread32Next(threadSnapHandle, &threadEntry));

                CloseHandle(threadSnapHandle);
            }
        }

        _systemMetrics.numThreads = numThreads;
#else
        const auto parseThreadsStatusLine = [](char* lineBuffer) {
            const int lineLength = strlen(lineBuffer);
            if (lineLength < 3) return 0;

            const char* ch = lineBuffer;
            while (*ch < '0' || *ch > '9') ch++;
            lineBuffer[lineLength - 3] = '\0';
            const auto number = atoi(ch);
            return number;
        };
        
        FILE* file = fopen("/proc/self/status", "r");
        int numThreads = 0;
        char lineBuffer[128] = {0};
        
        while (fgets(lineBuffer, 128, file) != NULL)
        {
            if (strncmp(lineBuffer, "Threads:", 8) == 0)
            {
                numThreads = parseThreadsStatusLine(lineBuffer);
                break;
            }
        }
        
        fclose(file);
        
        _systemMetrics.numThreads = numThreads;
#endif

        return true;
    }}
    //--------------------------------------------------------------------------

    bool SystemMetricsManager::_UpdateMemoryUsed() KMP_PROFILING(ProfileLevelImportantVerbose)
    {
#if defined (KMP_PLATFORM_WINDOWS)
        static constexpr auto MibDivisor = 1024.0 * 1024.0;
        PROCESS_MEMORY_COUNTERS_EX pmc;
        if (!static_cast<bool>(GetProcessMemoryInfo(reinterpret_cast<HANDLE>(_windowsProcessHandle), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc))))
        {
            KMP_LOG_ERROR("update failed on GetProcessMemoryInfo, error {}", GetLastError());
            return false;
        }

        _systemMetrics.virtualMemoryUsedMib = static_cast<float>(static_cast<double>(pmc.PrivateUsage) / MibDivisor);
        _systemMetrics.physicalMemoryUsedMib = static_cast<float>(static_cast<double>(pmc.WorkingSetSize) / MibDivisor);
#else
        const auto parseStatusLine = [](char* lineBuffer) {
            const int lineLength = strlen(lineBuffer);
            if (lineLength < 3) return 0;

            const char* ch = lineBuffer;
            while (*ch < '0' || *ch > '9') ch++;
            lineBuffer[lineLength - 3] = '\0';
            const auto number = atoi(ch);
            return number;
        };
        
        FILE* file = fopen("/proc/self/status", "r");
        int resultVirtual = -1;
        int resultPhysical = -1;
        char line[128] = {0};
        
        while (fgets(line, 128, file) != NULL)
        {
            if (strncmp(line, "VmSize:", 7) == 0)
            {
                resultVirtual = parseStatusLine(line);
            }
            if (strncmp(line, "VmRSS:", 6) == 0)
            {
                resultPhysical = parseStatusLine(line);
            }
            if (resultVirtual != -1 && resultPhysical != -1)
            {
                break;
            }
        }
        
        fclose(file);
        
        _systemMetrics.virtualMemoryUsedMib = static_cast<float>(static_cast<double>(resultVirtual) / 1024.0);
        _systemMetrics.physicalMemoryUsedMib = static_cast<float>(static_cast<double>(resultPhysical) / 1024.0);
#endif

        return true;
    }}
    //--------------------------------------------------------------------------

    bool SystemMetricsManager::_UpdateCPUUsed() KMP_PROFILING(ProfileLevelImportantVerbose)
    {
        if (_systemMetrics.numProcessors == 0)
        {
            return false;
        }

        double cpuPercent = 0.0;
        
#if defined (KMP_PLATFORM_WINDOWS)
        FILETIME fTime, fSystem, fUser;
        ULARGE_INTEGER now, system, user;
        const auto filetimeSize = sizeof(FILETIME);

        GetSystemTimeAsFileTime(&fTime);
        memcpy(&now, &fTime, filetimeSize);

        if (now.QuadPart - _lastCPUTimestamp == 0)
        {
            return false;
        }

        if (!static_cast<bool>(GetProcessTimes(reinterpret_cast<HANDLE>(_windowsProcessHandle), &fTime, &fTime, &fSystem, &fUser)))
        {
            KMP_LOG_ERROR("update failed on GetProcessTimes, error {}", GetLastError());
            return false;
        }

        memcpy(&system, &fSystem, filetimeSize);
        memcpy(&user, &fUser, filetimeSize);
        cpuPercent = static_cast<double>(system.QuadPart - _lastSysCPUTimestamp) + static_cast<double>(user.QuadPart - _lastUserCPUTimestamp);
        cpuPercent /= (now.QuadPart - _lastCPUTimestamp);
        cpuPercent /= _systemMetrics.numProcessors;
        cpuPercent *= 100;

        _lastCPUTimestamp = static_cast<unsigned long long>(now.QuadPart);
        _lastUserCPUTimestamp = static_cast<unsigned long long>(user.QuadPart);
        _lastSysCPUTimestamp = static_cast<unsigned long long>(system.QuadPart);
#else
        struct tms timeSample;
        const clock_t now = times(&timeSample);

        if (now - static_cast<clock_t>(_lastCPUTimestamp) == 0)
        {
            return false;
        }

        if (now <= static_cast<clock_t>(_lastCPUTimestamp) || 
            timeSample.tms_stime < static_cast<clock_t>(_lastSysCPUTimestamp) ||
            timeSample.tms_utime < static_cast<clock_t>(_lastUserCPUTimestamp))
        {
            cpuPercent = -1.0;
        }
        else
        {
            cpuPercent = (timeSample.tms_stime - static_cast<clock_t>(_lastSysCPUTimestamp)) + (timeSample.tms_utime - static_cast<clock_t>(_lastUserCPUTimestamp));
            cpuPercent /= (now - static_cast<clock_t>(_lastCPUTimestamp));
            cpuPercent /= _systemMetrics.numProcessors;
            cpuPercent *= 100;
        }
        
        _lastCPUTimestamp = static_cast<decltype(_lastCPUTimestamp)>(now);
        _lastSysCPUTimestamp = static_cast<decltype(_lastSysCPUTimestamp)>(timeSample.tms_stime);
        _lastUserCPUTimestamp = static_cast<decltype(_lastUserCPUTimestamp)>(timeSample.tms_utime);   
#endif

        _systemMetrics.cpuUsagePercent = static_cast<float>(cpuPercent);
        return true;
    }}
    //--------------------------------------------------------------------------

    bool SystemMetricsManager::_UpdateCurrentThreadStackUsed() KMP_PROFILING(ProfileLevelImportantVerbose)
    {
#if defined (KMP_PLATFORM_WINDOWS)
        static constexpr auto KibDivisor = 1024.0f;

        int localVariable = 0;
        auto* currentEspRegisterValueApproximate = &localVariable;

        NT_TIB* threadInformationBlockPtr = (NT_TIB*)NtCurrentTeb();
        void* stackBase = threadInformationBlockPtr->StackBase;
        void* stackLimit = threadInformationBlockPtr->StackLimit;

        const SIZE_T overallUsed = SIZE_T(stackBase) - SIZE_T(stackLimit);
        const SIZE_T currentlyApproximatelyUsed = SIZE_T(stackBase) - SIZE_T(currentEspRegisterValueApproximate);

        ULONG_PTR threadStackLowLimit = 0ULL;
        ULONG_PTR threadStackHighLimit = 0ULL;
        GetCurrentThreadStackLimits(&threadStackLowLimit, &threadStackHighLimit);

        _systemMetrics.currentThreadStackTotal = static_cast<float>(threadStackHighLimit - threadStackLowLimit) / KibDivisor;
        _systemMetrics.currentThreadStackOverallUsed = static_cast<float>(overallUsed) / KibDivisor;
        _systemMetrics.currentThreadStackOverallUsagePercent = _systemMetrics.currentThreadStackOverallUsed / _systemMetrics.currentThreadStackTotal * 100;
        _systemMetrics.currentThreadStackUsed = static_cast<float>(currentlyApproximatelyUsed) / KibDivisor;
        _systemMetrics.currentThreadStackUsagePercent = _systemMetrics.currentThreadStackUsed / _systemMetrics.currentThreadStackTotal * 100;
#endif

        return true;
    }}
    //--------------------------------------------------------------------------
}
