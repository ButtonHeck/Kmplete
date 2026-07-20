#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Profile/profiler_fwd.h"

#include <vulkan/vulkan.h>


namespace Kmplete
{
    namespace Graphics
    {
        //! Utility class for tracking memory usage by Vulkan API
        class KMP_API VulkanMetricsManager
        {
            KMP_DISABLE_COPY_MOVE(VulkanMetricsManager)
            KMP_PROFILE_CONSTRUCTOR_DECLARE()

        public:
            //! Memory usage metrics struct
            struct Metrics
            {
                Vector<UInt64> heapBudgets;
                Vector<UInt64> heapUsages;
                UInt64 totalBudget = 0ULL;
                UInt64 totalUsage = 0ULL;
                float usagePercent = 0.0f;
            };

        public:
            explicit VulkanMetricsManager(VkPhysicalDevice physicalDevice);
            ~VulkanMetricsManager() = default;

            const Metrics& QueryMetrics();
            KMP_NODISCARD const Metrics& GetMetrics() const noexcept;

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