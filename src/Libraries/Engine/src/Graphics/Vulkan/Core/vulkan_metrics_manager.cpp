#include "Kmplete/Graphics/Vulkan/Core/vulkan_metrics_manager.h"
#include "Kmplete/Graphics/Vulkan/Utils/initializers.h"
#include "Kmplete/Core/assertion.h"
#include "Kmplete/Profile/profiler.h"


namespace Kmplete
{
    namespace Graphics
    {
        VulkanMetricsManager::VulkanMetricsManager(VkPhysicalDevice physicalDevice)
            : KMP_PROFILE_CONSTRUCTOR_START_BASE_CLASS()
              _physicalDevice(physicalDevice)
            , _memoryProperties()
            , _memoryProperties2(VKUtils::InitVkPhysicalDeviceMemoryProperties2())
            , _memoryBudgetProperties(VKUtils::InitVkPhysicalDeviceMemoryBudgetPropertiesEXT())
            , _metrics()
        {
            _Initialize();

            KMP_PROFILE_CONSTRUCTOR_END()
        }
        //--------------------------------------------------------------------------

        const VulkanMetricsManager::Metrics& VulkanMetricsManager::QueryMetrics() KMP_PROFILING(ProfileLevelImportant)
        {
            vkGetPhysicalDeviceMemoryProperties2(_physicalDevice, &_memoryProperties2);

            _metrics.totalBudget = 0ULL;
            _metrics.totalUsage = 0ULL;
            for (UInt32 heap = 0; heap < _memoryProperties.memoryHeapCount; heap++)
            {
                _metrics.heapBudgets[heap] = _memoryBudgetProperties.heapBudget[heap];
                _metrics.heapUsages[heap] = _memoryBudgetProperties.heapUsage[heap];

                _metrics.totalBudget += _metrics.heapBudgets[heap];
                _metrics.totalUsage += _metrics.heapUsages[heap];
            }

            if (_metrics.totalBudget > 0ULL)
            {
                _metrics.usagePercent = float(double(_metrics.totalUsage) / double(_metrics.totalBudget) * 100.0);
            }

            return _metrics;
        }}
        //--------------------------------------------------------------------------

        const VulkanMetricsManager::Metrics& VulkanMetricsManager::GetMetrics() const noexcept
        {
            return _metrics;
        }
        //--------------------------------------------------------------------------

        void VulkanMetricsManager::_Initialize()
        {
            KMP_ASSERT(_physicalDevice);

            vkGetPhysicalDeviceMemoryProperties(_physicalDevice, &_memoryProperties);
            _memoryProperties2.pNext = &_memoryBudgetProperties;

            _metrics.heapBudgets.resize(_memoryProperties.memoryHeapCount);
            _metrics.heapUsages.resize(_memoryProperties.memoryHeapCount);

            QueryMetrics();
        }
        //--------------------------------------------------------------------------
    }
}