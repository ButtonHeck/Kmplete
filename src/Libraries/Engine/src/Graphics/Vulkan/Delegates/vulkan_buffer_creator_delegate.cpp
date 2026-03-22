#include "Kmplete/Graphics/Vulkan/Delegates/vulkan_buffer_creator_delegate.h"
#include "Kmplete/Graphics/Vulkan/Delegates/vulkan_memory_type_delegate.h"
#include "Kmplete/Profile/profiler.h"


namespace Kmplete
{
    namespace Graphics
    {
        VulkanBufferCreatorDelegate::VulkanBufferCreatorDelegate(VkDevice device, VkDescriptorPool descriptorPool, const VulkanMemoryTypeDelegate& memoryTypeDelegate)
            : _memoryTypeDelegate(memoryTypeDelegate)
            , _device(device)
            , _descriptorPool(descriptorPool)
        {}
        //--------------------------------------------------------------------------


        VulkanBuffer VulkanBufferCreatorDelegate::CreateBuffer(VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags memoryPropertyFlags, VkDeviceSize size) const
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

            return VulkanBuffer(_memoryTypeDelegate, _device, usageFlags, memoryPropertyFlags, size);
        }
        //--------------------------------------------------------------------------

        Nullable<VulkanBuffer*> VulkanBufferCreatorDelegate::CreateBufferPtr(VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags memoryPropertyFlags, VkDeviceSize size) const
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

            return new VulkanBuffer(_memoryTypeDelegate, _device, usageFlags, memoryPropertyFlags, size);
        }
        //--------------------------------------------------------------------------


        VulkanVertexBuffer VulkanBufferCreatorDelegate::CreateVertexBuffer(VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags memoryPropertyFlags, VkDeviceSize size) const
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

            return VulkanVertexBuffer(_memoryTypeDelegate, _device, usageFlags, memoryPropertyFlags, size);
        }
        //--------------------------------------------------------------------------

        Nullable<VulkanVertexBuffer*> VulkanBufferCreatorDelegate::CreateVertexBufferPtr(VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags memoryPropertyFlags, VkDeviceSize size) const
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

            return new VulkanVertexBuffer(_memoryTypeDelegate, _device, usageFlags, memoryPropertyFlags, size);
        }
        //--------------------------------------------------------------------------


        VulkanUniformBuffer VulkanBufferCreatorDelegate::CreateUniformBuffer(VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags memoryPropertyFlags, VkDeviceSize size, const Vector<VkDescriptorSetLayout>& descriptorSetLayouts, UInt32 binding) const
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

            return VulkanUniformBuffer(_memoryTypeDelegate, _device, usageFlags, memoryPropertyFlags, size, _descriptorPool, descriptorSetLayouts, binding);
        }
        //--------------------------------------------------------------------------

        Nullable<VulkanUniformBuffer*> VulkanBufferCreatorDelegate::CreateUniformBufferPtr(VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags memoryPropertyFlags, VkDeviceSize size, const Vector<VkDescriptorSetLayout>& descriptorSetLayouts, UInt32 binding) const
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

            return new VulkanUniformBuffer(_memoryTypeDelegate, _device, usageFlags, memoryPropertyFlags, size, _descriptorPool, descriptorSetLayouts, binding);
        }
        //--------------------------------------------------------------------------
    }
}