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

            return VulkanVertexBuffer(_memoryTypeDelegate, _device, parameters);
        }
        //--------------------------------------------------------------------------

        Nullable<VulkanVertexBuffer*> VulkanBufferCreatorDelegate::CreateVertexBufferPtr(const VulkanBufferParameters& parameters) const
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

            return new VulkanVertexBuffer(_memoryTypeDelegate, _device, parameters);
        }
        //--------------------------------------------------------------------------


        VulkanUniformBuffer VulkanBufferCreatorDelegate::CreateUniformBuffer(const VulkanBufferParameters& parameters, const Vector<VkDescriptorSetLayout>& descriptorSetLayouts, UInt32 binding) const
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

            return VulkanUniformBuffer(_memoryTypeDelegate, _device, parameters, _descriptorPool, descriptorSetLayouts, binding);
        }
        //--------------------------------------------------------------------------

        Nullable<VulkanUniformBuffer*> VulkanBufferCreatorDelegate::CreateUniformBufferPtr(const VulkanBufferParameters& parameters, const Vector<VkDescriptorSetLayout>& descriptorSetLayouts, UInt32 binding) const
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

            return new VulkanUniformBuffer(_memoryTypeDelegate, _device, parameters, _descriptorPool, descriptorSetLayouts, binding);
        }
        //--------------------------------------------------------------------------
    }
}