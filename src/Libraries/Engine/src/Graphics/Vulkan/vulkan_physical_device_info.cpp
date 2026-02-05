#include "Kmplete/Graphics/Vulkan/vulkan_physical_device_info.h"
#include "Kmplete/Log/log.h"

#include <stdexcept>


namespace Kmplete
{
    namespace Graphics
    {
        VkSampleCountFlagBits PhysicalDeviceInfo::MaximumSupportedSampleCount() const
        {
            return supportedSampleCounts.top();
        }
        //--------------------------------------------------------------------------

        UInt32 PhysicalDeviceInfo::FindMemoryType(UInt32 typeFilter, VkMemoryPropertyFlags properties) const
        {
            for (UInt32 i = 0; i < memoryProperties.memoryTypeCount; i++)
            {
                if ((typeFilter & (1 << i)) && (memoryProperties.memoryTypes[i].propertyFlags & properties) == properties)
                {
                    return i;
                }
            }

            KMP_LOG_CRITICAL("failed to find suitable memory type");
            throw std::runtime_error("PhysicalDeviceInfo: failed to find suitable memory type");
        }
        //--------------------------------------------------------------------------
    }
}