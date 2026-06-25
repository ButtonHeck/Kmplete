#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Profile/profiler_fwd.h"

#include <vulkan/vulkan.h>


namespace Kmplete
{
    namespace Graphics
    {
        //TODO: comments
        class VulkanMetricsManager
        {
            KMP_DISABLE_COPY_MOVE(VulkanMetricsManager)
            KMP_PROFILE_CONSTRUCTOR_DECLARE()

        public:
            struct Metrics
            {
                Vector<UInt64> heapBudgets;
                Vector<UInt64> heapUsages;
                UInt64 totalBudget = 0ULL;
                UInt64 totalUsage = 0ULL;
                float usagePercent = 0.0f;
            };

        public:
            KMP_API explicit VulkanMetricsManager(VkPhysicalDevice physicalDevice);
            ~VulkanMetricsManager() = default;

            KMP_API const Metrics& QueryMetrics();
            KMP_NODISCARD KMP_API const Metrics& GetMetrics() const noexcept;

        private:
            void _Initialize();

        private:
            VkPhysicalDevice _physicalDevice;
            VkPhysicalDeviceMemoryProperties _memoryProperties;
            VkPhysicalDeviceMemoryProperties2 _memoryProperties2;
            VkPhysicalDeviceMemoryBudgetPropertiesEXT _memoryBudgetProperties;
            Metrics _metrics;
        };
        //--------------------------------------------------------------------------
    }
}