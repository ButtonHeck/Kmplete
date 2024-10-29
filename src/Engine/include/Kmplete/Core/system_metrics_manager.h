#pragma once

#include "Kmplete/Core/kmplete_api.h"
#include "Kmplete/Core/platform.h"

namespace Kmplete
{
    class SystemMetricsManager
    {
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

    public:
        KMP_DISABLE_COPY_MOVE(SystemMetricsManager)

        KMP_API SystemMetricsManager();
        KMP_API ~SystemMetricsManager() = default;

        KMP_API bool Update();
        KMP_NODISCARD KMP_API const SystemMetrics& GetMetrics() const KMP_NOEXCEPT;

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