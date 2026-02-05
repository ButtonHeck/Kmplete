#include "Kmplete/Graphics/Vulkan/vulkan_physical_device_implementation_info.h"
#include "Kmplete/Log/log.h"

#include <stdexcept>


namespace Kmplete
{
    namespace Graphics
    {
        VkSampleCountFlagBits PhysicalDeviceImplementationInfo::MaximumSupportedSampleCount() const
        {
            return supportedSampleCounts.top();
        }
        //--------------------------------------------------------------------------

        UInt32 PhysicalDeviceImplementationInfo::FindMemoryType(UInt32 typeFilter, VkMemoryPropertyFlags properties) const
        {
            for (UInt32 i = 0; i < memoryProperties.memoryTypeCount; i++)
            {
                if ((typeFilter & (1 << i)) && (memoryProperties.memoryTypes[i].propertyFlags & properties) == properties)
                {
                    return i;
                }
            }

            KMP_LOG_CRITICAL("failed to find suitable memory type");
            throw std::runtime_error("PhysicalDeviceImplementationInfo: failed to find suitable memory type");
        }
        //--------------------------------------------------------------------------
    }
}