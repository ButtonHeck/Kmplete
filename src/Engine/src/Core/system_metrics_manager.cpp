#include "Kmplete/Core/system_metrics_manager.h"
#include "Kmplete/Core/assertion.h"
#include "Kmplete/Core/log.h"

#if defined (KMP_PLATFORM_WINDOWS)
#include <Windows.h>
#include <Psapi.h>
#include <TlHelp32.h>
#else
#include <sys/types.h>
#include <sys/sysinfo.h>
#include <sys/times.h>
#include <unistd.h>
#endif

namespace Kmplete
{
    SystemMetricsManager::SystemMetricsManager()
        : _lastCPUTimestamp(0ULL)
        , _lastSysCPUTimestamp(0ULL)
        , _lastUserCPUTimestamp(0ULL)
        , _currentProcessId(0)
#if defined (KMP_PLATFORM_WINDOWS)
        , _windowsProcessHandle(nullptr)
#endif
    {
        Initialize();
    }
    //--------------------------------------------------------------------------

    bool SystemMetricsManager::Update()
    {
#if defined (KMP_PLATFORM_WINDOWS)
        if (!_windowsProcessHandle)
        {
            _windowsProcessHandle = GetCurrentProcess();
            if (!_windowsProcessHandle)
            {
                Log::CoreError("SystemMetricsManager: update failed on GetCurrentProcess handle");
                return false;
            }
        }
#endif

        if (!UpdateNumThreads())
        {
            return false;
        }

        if (!UpdateMemoryUsed())
        {
            return false;
        }

        if (!UpdateCPUUsed())
        {
            return false;
        }

        return true;
    }
    //--------------------------------------------------------------------------

    const SystemMetricsManager::SystemMetrics& SystemMetricsManager::GetMetrics() const KMP_NOEXCEPT
    {
        return _systemMetrics;
    }
    //--------------------------------------------------------------------------

    void SystemMetricsManager::Initialize()
    {
#if defined (KMP_PLATFORM_WINDOWS)
        static_assert(sizeof(ULARGE_INTEGER) == sizeof(decltype(_lastCPUTimestamp)));
#endif

        if (!InitializeProcessId())
        {
            return;
        }

        if (!InitializeTotalMemory())
        {
            return;
        }

        if (!InitializeNumProcessors())
        {
            return;
        }

        if (!InitializeWindowsProcessHandle())
        {
            return;
        }

        if (!InitializeCPUTimestamps())
        {
            return;
        }

        KMP_ASSERT(_systemMetrics.numProcessors);
        KMP_ASSERT(_systemMetrics.totalVirtualMemoryMib);
        KMP_ASSERT(_systemMetrics.totalPhysicalMemoryMib);
    }
    //--------------------------------------------------------------------------

    bool SystemMetricsManager::InitializeProcessId()
    {
#if defined (KMP_PLATFORM_WINDOWS)
        _currentProcessId = GetCurrentProcessId();
#else
        _currentProcessId = getpid();
#endif

        return true;
    }
    //--------------------------------------------------------------------------

    bool SystemMetricsManager::InitializeTotalMemory()
    {
        constexpr static auto MibDivisor = 1024.0 * 1024.0;
        
#if defined (KMP_PLATFORM_WINDOWS)
        MEMORYSTATUSEX memoryInfo;
        memoryInfo.dwLength = sizeof(MEMORYSTATUSEX);
        if (!static_cast<bool>(GlobalMemoryStatusEx(&memoryInfo)))
        {
            Log::CoreError("SystemMetricsManager: initialization failed on GlobalMemoryStatusEx, error {}", GetLastError());
            return false;
        }

        _systemMetrics.totalVirtualMemoryMib = static_cast<float>(static_cast<double>(memoryInfo.ullTotalPageFile) / MibDivisor);
        _systemMetrics.totalPhysicalMemoryMib = static_cast<float>(static_cast<double>(memoryInfo.ullTotalPhys) / MibDivisor);
#else
        struct sysinfo memoryInfo;
        const auto error = sysinfo(&memoryInfo);
        if (error != 0)
        {
            Log::CoreError("SystemMetricsManager: initialization failed on sysinfo");
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
    }
    //--------------------------------------------------------------------------

    bool SystemMetricsManager::InitializeNumProcessors()
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
    }
    //--------------------------------------------------------------------------

    bool SystemMetricsManager::InitializeWindowsProcessHandle()
    {
#if defined (KMP_PLATFORM_WINDOWS)
        _windowsProcessHandle = GetCurrentProcess();
        if (!_windowsProcessHandle)
        {
            Log::CoreError("SystemMetricsManager: initialization failed on GetCurrentProcess handle");
            return false;
        }
#else
#endif

        return true;
    }
    //--------------------------------------------------------------------------

    bool SystemMetricsManager::InitializeCPUTimestamps()
    {
#if defined (KMP_PLATFORM_WINDOWS)
        FILETIME fTime, fSystem, fUser;
        const auto filetimeSize = sizeof(FILETIME);

        GetSystemTimeAsFileTime(&fTime);
        memcpy(&_lastCPUTimestamp, &fTime, filetimeSize);

        if (!static_cast<bool>(GetProcessTimes(reinterpret_cast<HANDLE>(_windowsProcessHandle), &fTime, &fTime, &fSystem, &fUser)))
        {
            Log::CoreError("SystemMetricsManager: initialization failed on GetProcessTimes, error {}", GetLastError());
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
    }
    //--------------------------------------------------------------------------

    bool SystemMetricsManager::UpdateNumThreads()
    {
#if defined (KMP_PLATFORM_WINDOWS)
        HANDLE threadSnapHandle = INVALID_HANDLE_VALUE;
        unsigned int numThreads = 0;
        threadSnapHandle = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
        if (threadSnapHandle == INVALID_HANDLE_VALUE)
        {
            Log::CoreWarn("SystemMetricsManager: failed to take snapshot of all running threads");
        }
        else
        {
            THREADENTRY32 threadEntry; 
            threadEntry.dwSize = sizeof(THREADENTRY32);

            if (!Thread32First(threadSnapHandle, &threadEntry))
            {
                Log::CoreWarn("SystemMetricsManager: failed to retrieve information about the first thread");
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
        const auto parseLine = [](char* line) {
            int i = strlen(line);
            const char* p = line;
            while (*p <'0' || *p > '9') p++;
            line[i-3] = '\0';
            i = atoi(p);
            return i;
        };
        
        FILE* file = fopen("/proc/self/status", "r");
        int resultThreads = -1;
        char line[128] = {0};
        
        while (fgets(line, 128, file) != NULL)
        {
            if (strncmp(line, "Threads:", 8) == 0)
            {
                resultThreads = parseLine(line);
                break;
            }
        }
        
        fclose(file);
        
        _systemMetrics.numThreads = resultThreads;
#endif

        return true;
    }
    //--------------------------------------------------------------------------

    bool SystemMetricsManager::UpdateMemoryUsed()
    {
#if defined (KMP_PLATFORM_WINDOWS)
        constexpr static auto MibDivisor = 1024.0 * 1024.0;
        PROCESS_MEMORY_COUNTERS_EX pmc;
        if (!static_cast<bool>(GetProcessMemoryInfo(reinterpret_cast<HANDLE>(_windowsProcessHandle), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc))))
        {
            Log::CoreError("SystemMetricsManager: update failed on GetProcessMemoryInfo, error {}", GetLastError());
            return false;
        }

        _systemMetrics.virtualMemoryUsedMib = static_cast<float>(static_cast<double>(pmc.PrivateUsage) / MibDivisor);
        _systemMetrics.physicalMemoryUsedMib = static_cast<float>(static_cast<double>(pmc.WorkingSetSize) / MibDivisor);
#else
        const auto parseLine = [](char* line) {
            int i = strlen(line);
            const char* p = line;
            while (*p <'0' || *p > '9') p++;
            line[i-3] = '\0';
            i = atoi(p);
            return i;
        };
        
        FILE* file = fopen("/proc/self/status", "r");
        int resultVirtual = -1;
        int resultPhysical = -1;
        char line[128] = {0};
        
        while (fgets(line, 128, file) != NULL)
        {
            if (strncmp(line, "VmSize:", 7) == 0)
            {
                resultVirtual = parseLine(line);
            }
            if (strncmp(line, "VmRSS:", 6) == 0)
            {
                resultPhysical = parseLine(line);
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
    }
    //--------------------------------------------------------------------------

    bool SystemMetricsManager::UpdateCPUUsed()
    {
        double cpuPercent = 0.0;
        
#if defined (KMP_PLATFORM_WINDOWS)
        FILETIME fTime, fSystem, fUser;
        ULARGE_INTEGER now, system, user;
        const auto filetimeSize = sizeof(FILETIME);

        GetSystemTimeAsFileTime(&fTime);
        memcpy(&now, &fTime, filetimeSize);

        if (!static_cast<bool>(GetProcessTimes(reinterpret_cast<HANDLE>(_windowsProcessHandle), &fTime, &fTime, &fSystem, &fUser)))
        {
            Log::CoreError("SystemMetricsManager: update failed on GetProcessTimes, error {}", GetLastError());
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
    }
    //--------------------------------------------------------------------------
}
