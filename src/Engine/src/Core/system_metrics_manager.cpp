#include "Kmplete/Core/system_metrics_manager.h"
#include "Kmplete/Core/assertion.h"
#include "Kmplete/Core/log.h"

#if defined KMP_PLATFORM_WINDOWS
#include <Windows.h>
#include <Psapi.h>
#include <TlHelp32.h>
#else
#endif

namespace Kmplete
{
    SystemMetricsManager::SystemMetricsManager()
        : _lastCPUTimestamp(0ULL)
        , _lastSysCPUTimestamp(0ULL)
        , _lastUserCPUTimestamp(0ULL)
        , _currentProcessId(0)
#if defined KMP_PLATFORM_WINDOWS
        , _windowsProcessHandle(nullptr)
#endif
    {
        Initialize();
    }
    //--------------------------------------------------------------------------

    bool SystemMetricsManager::Update()
    {
#if defined KMP_PLATFORM_WINDOWS
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
#if defined KMP_PLATFORM_WINDOWS
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
#if defined KMP_PLATFORM_WINDOWS
        _currentProcessId = GetCurrentProcessId();
#else
#endif

        return true;
    }
    //--------------------------------------------------------------------------

    bool SystemMetricsManager::InitializeTotalMemory()
    {
#if defined KMP_PLATFORM_WINDOWS
        constexpr static auto MibDivisor = 1024.0 * 1024.0;
        MEMORYSTATUSEX memoryInfo{.dwLength = sizeof(MEMORYSTATUSEX) };
        if (!static_cast<bool>(GlobalMemoryStatusEx(&memoryInfo)))
        {
            Log::CoreError("SystemMetricsManager: initialization failed on GlobalMemoryStatusEx, error {}", GetLastError());
            return false;
        }

        _systemMetrics.totalVirtualMemoryMib = static_cast<float>(static_cast<double>(memoryInfo.ullTotalPageFile) / MibDivisor);
        _systemMetrics.totalPhysicalMemoryMib = static_cast<float>(static_cast<double>(memoryInfo.ullTotalPhys) / MibDivisor);
#else
#endif

        return true;
    }
    //--------------------------------------------------------------------------

    bool SystemMetricsManager::InitializeNumProcessors()
    {
#if defined KMP_PLATFORM_WINDOWS
        SYSTEM_INFO systemInfo;
        GetSystemInfo(&systemInfo);
        _systemMetrics.numProcessors = systemInfo.dwNumberOfProcessors;
#else
#endif

        return true;
    }
    //--------------------------------------------------------------------------

    bool SystemMetricsManager::InitializeWindowsProcessHandle()
    {
#if defined KMP_PLATFORM_WINDOWS
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
#if defined KMP_PLATFORM_WINDOWS
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
#endif

        return true;
    }
    //--------------------------------------------------------------------------

    bool SystemMetricsManager::UpdateNumThreads()
    {
#if defined KMP_PLATFORM_WINDOWS
        HANDLE threadSnapHandle = INVALID_HANDLE_VALUE;
        unsigned int numThreads = 0;
        threadSnapHandle = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
        if (threadSnapHandle == INVALID_HANDLE_VALUE)
        {
            Log::CoreWarn("SystemMetricsManager: failed to take snapshot of all running threads");
        }
        else
        {
            THREADENTRY32 threadEntry{.dwSize = sizeof(THREADENTRY32) };

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
#endif

        return true;
    }
    //--------------------------------------------------------------------------

    bool SystemMetricsManager::UpdateMemoryUsed()
    {
#if defined KMP_PLATFORM_WINDOWS
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
#endif

        return true;
    }
    //--------------------------------------------------------------------------

    bool SystemMetricsManager::UpdateCPUUsed()
    {
#if defined KMP_PLATFORM_WINDOWS
        FILETIME fTime, fSystem, fUser;
        ULARGE_INTEGER now, system, user;
        double cpuPercent;
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

        _systemMetrics.cpuUsagePercent = static_cast<float>(cpuPercent);
#else
#endif

        return true;
    }
    //--------------------------------------------------------------------------
}
