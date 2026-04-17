#include "Kmplete/Graphics/Vulkan/Delegates/vulkan_buffer_creator_delegate.h"
#include "Kmplete/Graphics/Vulkan/Delegates/vulkan_memory_type_delegate.h"
#include "Kmplete/Profile/profiler.h"


namespace Kmplete
{
    namespace Graphics
    {
        VulkanBufferCreatorDelegate::VulkanBufferCreatorDelegate(VkDevice device, const VulkanMemoryTypeDelegate& memoryTypeDelegate)
            : _memoryTypeDelegate(memoryTypeDelegate)
            , _device(device)
        {}
        //--------------------------------------------------------------------------


        VulkanBuffer VulkanBufferCreatorDelegate::CreateBuffer(const VulkanBufferParameters& parameters) const KMP_PROFILING(ProfileLevelImportant)
        {
            return VulkanBuffer(_memoryTypeDelegate, _device, parameters);
        }}
        //--------------------------------------------------------------------------

        Nullable<VulkanBuffer*> VulkanBufferCreatorDelegate::CreateBufferPtr(const VulkanBufferParameters& parameters) const KMP_PROFILING(ProfileLevelImportant)
        {
            return new VulkanBuffer(_memoryTypeDelegate, _device, parameters);
        }}
        //--------------------------------------------------------------------------


        VulkanVertexBuffer VulkanBufferCreatorDelegate::CreateVertexBuffer(const VulkanBufferParameters& parameters) const KMP_PROFILING(ProfileLevelImportant)
        {
            return VulkanVertexBuffer(_memoryTypeDelegate, _device, VulkanBufferParameters{
                .usageFlags = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | parameters.usageFlags,
                .memoryPropertyFlags = parameters.memoryPropertyFlags,
                .size = parameters.size
            });
        }}
        //--------------------------------------------------------------------------

        Nullable<VulkanVertexBuffer*> VulkanBufferCreatorDelegate::CreateVertexBufferPtr(const VulkanBufferParameters& parameters) const KMP_PROFILING(ProfileLevelImportant)
        {
            return new VulkanVertexBuffer(_memoryTypeDelegate, _device, VulkanBufferParameters{
                .usageFlags = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | parameters.usageFlags,
                .memoryPropertyFlags = parameters.memoryPropertyFlags,
                .size = parameters.size
            });
        }}
        //--------------------------------------------------------------------------


        VulkanBuffer VulkanBufferCreatorDelegate::CreateIndexBuffer(const VulkanBufferParameters& parameters) const KMP_PROFILING(ProfileLevelImportant)
        {
            return VulkanBuffer(_memoryTypeDelegate, _device, VulkanBufferParameters{
                .usageFlags = VK_BUFFER_USAGE_INDEX_BUFFER_BIT | parameters.usageFlags,
                .memoryPropertyFlags = parameters.memoryPropertyFlags,
                .size = parameters.size
            });
        }}
        //--------------------------------------------------------------------------

        Nullable<VulkanBuffer*> VulkanBufferCreatorDelegate::CreateIndexBufferPtr(const VulkanBufferParameters& parameters) const KMP_PROFILING(ProfileLevelImportant)
        {
            return new VulkanBuffer(_memoryTypeDelegate, _device, VulkanBufferParameters{
                .usageFlags = VK_BUFFER_USAGE_INDEX_BUFFER_BIT | parameters.usageFlags,
                .memoryPropertyFlags = parameters.memoryPropertyFlags,
                .size = parameters.size
            });
        }}
        //--------------------------------------------------------------------------


        VulkanUniformBuffer VulkanBufferCreatorDelegate::CreateUniformBuffer(const VulkanBufferParameters& parameters) const KMP_PROFILING(ProfileLevelImportant)
        {
            return VulkanUniformBuffer(_memoryTypeDelegate, _device, VulkanBufferParameters{
                .usageFlags = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | parameters.usageFlags,
                .memoryPropertyFlags = parameters.memoryPropertyFlags,
                .size = parameters.size
            });
        }}
        //--------------------------------------------------------------------------

        Nullable<VulkanUniformBuffer*> VulkanBufferCreatorDelegate::CreateUniformBufferPtr(const VulkanBufferParameters& parameters) const KMP_PROFILING(ProfileLevelImportant)
        {
            return new VulkanUniformBuffer(_memoryTypeDelegate, _device, VulkanBufferParameters{
                .usageFlags = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | parameters.usageFlags,
                .memoryPropertyFlags = parameters.memoryPropertyFlags,
                .size = parameters.size
            });
        }}
        //--------------------------------------------------------------------------
    }
}