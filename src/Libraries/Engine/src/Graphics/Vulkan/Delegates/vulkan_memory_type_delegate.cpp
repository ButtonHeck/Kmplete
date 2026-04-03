#include "Kmplete/Graphics/Vulkan/Delegates/vulkan_memory_type_delegate.h"
#include "Kmplete/Graphics/Vulkan/Utils/initializers.h"
#include "Kmplete/Log/log.h"
#include "Kmplete/Profile/profiler.h"


namespace Kmplete
{
    namespace Graphics
    {
        VulkanMemoryTypeDelegate::VulkanMemoryTypeDelegate(VkPhysicalDeviceMemoryProperties memoryProperties) noexcept
            : _memoryProperties(memoryProperties)
        {}
        //--------------------------------------------------------------------------

        VulkanMemoryTypeDelegate::MemoryContext VulkanMemoryTypeDelegate::GetBufferMemoryContext(VkDevice device, VkBuffer buffer, VkMemoryPropertyFlags properties) const KMP_PROFILING(ProfileLevelMinor)
        {
            MemoryContext context{};
            vkGetBufferMemoryRequirements(device, buffer, &context.requirements);

            context.allocateInfo = VulkanUtils::InitVkMemoryAllocateInfo();
            context.allocateInfo.allocationSize = context.requirements.size;
            context.allocateInfo.memoryTypeIndex = FindMemoryType(context.requirements.memoryTypeBits, properties);

            return context;
        }}
        //--------------------------------------------------------------------------

        VulkanMemoryTypeDelegate::MemoryContext VulkanMemoryTypeDelegate::GetImageMemoryContext(VkDevice device, VkImage image, VkMemoryPropertyFlags properties) const KMP_PROFILING(ProfileLevelMinor)
        {
            MemoryContext context{};
            vkGetImageMemoryRequirements(device, image, &context.requirements);

            context.allocateInfo = VulkanUtils::InitVkMemoryAllocateInfo();
            context.allocateInfo.allocationSize = context.requirements.size;
            context.allocateInfo.memoryTypeIndex = FindMemoryType(context.requirements.memoryTypeBits, properties);

            return context;
        }}
        //--------------------------------------------------------------------------

        UInt32 VulkanMemoryTypeDelegate::FindMemoryType(UInt32 typeFilter, VkMemoryPropertyFlags properties) const KMP_PROFILING(ProfileLevelMinorVerbose)
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
        }}
        //--------------------------------------------------------------------------
    }
}