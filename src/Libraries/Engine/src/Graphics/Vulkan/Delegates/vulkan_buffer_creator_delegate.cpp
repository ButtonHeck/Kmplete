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


        VulkanBuffer VulkanBufferCreatorDelegate::CreateBuffer(const VulkanBufferParameters& parameters) const
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

            return VulkanBuffer(_memoryTypeDelegate, _device, parameters);
        }
        //--------------------------------------------------------------------------

        Nullable<VulkanBuffer*> VulkanBufferCreatorDelegate::CreateBufferPtr(const VulkanBufferParameters& parameters) const
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

            return new VulkanBuffer(_memoryTypeDelegate, _device, parameters);
        }
        //--------------------------------------------------------------------------


        VulkanVertexBuffer VulkanBufferCreatorDelegate::CreateVertexBuffer(const VulkanBufferParameters& parameters) const
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

            return VulkanVertexBuffer(_memoryTypeDelegate, _device, VulkanBufferParameters{
                .usageFlags = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | parameters.usageFlags,
                .memoryPropertyFlags = parameters.memoryPropertyFlags,
                .size = parameters.size
            });
        }
        //--------------------------------------------------------------------------

        Nullable<VulkanVertexBuffer*> VulkanBufferCreatorDelegate::CreateVertexBufferPtr(const VulkanBufferParameters& parameters) const
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

            return new VulkanVertexBuffer(_memoryTypeDelegate, _device, VulkanBufferParameters{
                .usageFlags = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | parameters.usageFlags,
                .memoryPropertyFlags = parameters.memoryPropertyFlags,
                .size = parameters.size
            });
        }
        //--------------------------------------------------------------------------


        VulkanBuffer VulkanBufferCreatorDelegate::CreateIndexBuffer(const VulkanBufferParameters& parameters) const
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

            return VulkanBuffer(_memoryTypeDelegate, _device, VulkanBufferParameters{
                .usageFlags = VK_BUFFER_USAGE_INDEX_BUFFER_BIT | parameters.usageFlags,
                .memoryPropertyFlags = parameters.memoryPropertyFlags,
                .size = parameters.size
            });
        }
        //--------------------------------------------------------------------------

        Nullable<VulkanBuffer*> VulkanBufferCreatorDelegate::CreateIndexBufferPtr(const VulkanBufferParameters& parameters) const
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

            return new VulkanBuffer(_memoryTypeDelegate, _device, VulkanBufferParameters{
                .usageFlags = VK_BUFFER_USAGE_INDEX_BUFFER_BIT | parameters.usageFlags,
                .memoryPropertyFlags = parameters.memoryPropertyFlags,
                .size = parameters.size
            });
        }
        //--------------------------------------------------------------------------


        VulkanUniformBuffer VulkanBufferCreatorDelegate::CreateUniformBuffer(const VulkanBufferParameters& parameters, const Vector<VkDescriptorSetLayout>& descriptorSetLayouts, UInt32 binding) const
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

            return VulkanUniformBuffer(_memoryTypeDelegate, _device, VulkanBufferParameters{
                .usageFlags = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | parameters.usageFlags,
                .memoryPropertyFlags = parameters.memoryPropertyFlags,
                .size = parameters.size
            }, _descriptorPool, descriptorSetLayouts, binding);
        }
        //--------------------------------------------------------------------------

        Nullable<VulkanUniformBuffer*> VulkanBufferCreatorDelegate::CreateUniformBufferPtr(const VulkanBufferParameters& parameters, const Vector<VkDescriptorSetLayout>& descriptorSetLayouts, UInt32 binding) const
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

            return new VulkanUniformBuffer(_memoryTypeDelegate, _device, VulkanBufferParameters{
                .usageFlags = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | parameters.usageFlags,
                .memoryPropertyFlags = parameters.memoryPropertyFlags,
                .size = parameters.size
            }, _descriptorPool, descriptorSetLayouts, binding);
        }
        //--------------------------------------------------------------------------
    }
}