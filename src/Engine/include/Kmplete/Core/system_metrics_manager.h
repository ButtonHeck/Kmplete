#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/platform.h"
#include "Kmplete/Profile/profiler.h"
#include "Kmplete/Log/log_class_macro.h"

namespace Kmplete
{
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
        };

        enum SystemMetricsUpdateMode
        {
            MemoryUsed      = 1 << 0,
            CPUUsed         = 1 << 1,
            NumThreads      = 1 << 2,
            MemoryAndCPU    = MemoryUsed | CPUUsed,
            Everything      = MemoryUsed | CPUUsed | NumThreads
        };

    public:
        KMP_DISABLE_COPY_MOVE(SystemMetricsManager)

        KMP_API SystemMetricsManager();
        ~SystemMetricsManager() = default;

        KMP_API bool Update(SystemMetricsUpdateMode updateMode);
        KMP_NODISCARD KMP_API const SystemMetrics& GetMetrics() const noexcept;

    private:
        void Initialize();
        KMP_NODISCARD bool InitializeProcessId();
        KMP_NODISCARD bool InitializeTotalMemory();
        KMP_NODISCARD bool InitializeNumProcessors();
        KMP_NODISCARD bool InitializeWindowsProcessHandle();
        KMP_NODISCARD bool InitializeCPUTimestamps();

        KMP_NODISCARD bool UpdateNumThreads();
        KMP_NODISCARD bool UpdateMemoryUsed();
        KMP_NODISCARD bool UpdateCPUUsed();

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