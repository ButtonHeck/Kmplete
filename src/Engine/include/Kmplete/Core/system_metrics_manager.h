#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/platform.h"
#include "Kmplete/Profile/profiler.h"
#include "Kmplete/Log/log_class_macro.h"


namespace Kmplete
{
    //! Manager of the system metrics such as CPU usage, RAM usage, threads and CPUs count, stack memory usage.
    //! Responsible for updating/fetching different metrics values on demand, which metrics to fetch
    //! are determined by mode flags
    class SystemMetricsManager
    {
        KMP_LOG_CLASSNAME(SystemMetricsManager)
        KMP_PROFILE_CONSTRUCTOR_DECLARE()

    public:
        struct SystemMetrics
        {
            unsigned int numProcessors = 1;
            unsigned int numThreads = 0;
            float totalVirtualMemoryMib = 0.0f;
            float totalPhysicalMemoryMib = 0.0f;
            float virtualMemoryUsedMib = 0.0f;
            float physicalMemoryUsedMib = 0.0f;
            float cpuUsagePercent = 0.0f;
            float currentThreadStackTotal = 0.0f;
            float currentThreadStackOverallUsed = 0.0f;
            float currentThreadStackOverallUsagePercent = 0.0f;
            float currentThreadStackUsed = 0.0f;
            float currentThreadStackUsagePercent = 0.0f;
        };

        enum SystemMetricsUpdateMode
        {
            MemoryUsed          = 1 << 0,
            CPUUsed             = 1 << 1,
            NumThreads          = 1 << 2,
            StackUsed           = 1 << 3,
            MemoryAndCPU        = MemoryUsed | CPUUsed,
            MemoryCPUAndStack   = MemoryUsed | CPUUsed | StackUsed,
            Everything          = MemoryUsed | CPUUsed | NumThreads | StackUsed
        };

    public:
        KMP_DISABLE_COPY_MOVE(SystemMetricsManager)

        KMP_API SystemMetricsManager();
        ~SystemMetricsManager() = default;

        KMP_API bool Update(SystemMetricsUpdateMode updateMode);
        KMP_NODISCARD KMP_API const SystemMetrics& GetMetrics() const noexcept;

    private:
        void _Initialize();
        KMP_NODISCARD bool _InitializeProcessId();
        KMP_NODISCARD bool _InitializeTotalMemory();
        KMP_NODISCARD bool _InitializeNumProcessors();
        KMP_NODISCARD bool _InitializeWindowsProcessHandle();
        KMP_NODISCARD bool _InitializeCPUTimestamps();

        KMP_NODISCARD bool _UpdateNumThreads();
        KMP_NODISCARD bool _UpdateMemoryUsed();
        KMP_NODISCARD bool _UpdateCPUUsed();
        KMP_NODISCARD bool _UpdateCurrentThreadStackUsed();

    private:
        SystemMetrics _systemMetrics;
        unsigned long long _lastCPUTimestamp;
        unsigned long long _lastSysCPUTimestamp;
        unsigned long long _lastUserCPUTimestamp;
        unsigned long _currentProcessId;

#if defined (KMP_PLATFORM_WINDOWS)
        void* _windowsProcessHandle;
#endif
    };
    //--------------------------------------------------------------------------
}