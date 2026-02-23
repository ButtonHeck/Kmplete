#include "Kmplete/Graphics/Vulkan/vulkan_memory_type_delegate.h"
#include "Kmplete/Log/log.h"


namespace Kmplete
{
    namespace Graphics
    {
        VulkanMemoryTypeDelegate::VulkanMemoryTypeDelegate(VkPhysicalDeviceMemoryProperties memoryProperties) noexcept
            : _memoryProperties(memoryProperties)
        {}
        //--------------------------------------------------------------------------

        UInt32 VulkanMemoryTypeDelegate::FindMemoryType(UInt32 typeFilter, VkMemoryPropertyFlags properties) const
        {
            for (UInt32 i = 0; i < _memoryProperties.memoryTypeCount; i++)
            {
                if ((typeFilter & (1 << i)) && (_memoryProperties.memoryTypes[i].propertyFlags & properties) == properties)
                {
                    return i;
                }
            }

            KMP_LOG_CRITICAL("failed to find suitable memory type");
            throw std::runtime_error("VulkanMemoryTypeDelegate: failed to find suitable memory type");
        }
        //--------------------------------------------------------------------------
    }
}