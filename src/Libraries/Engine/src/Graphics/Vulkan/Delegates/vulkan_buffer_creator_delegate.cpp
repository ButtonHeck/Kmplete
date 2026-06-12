#include "Kmplete/Graphics/Vulkan/Delegates/vulkan_buffer_creator_delegate.h"
#include "Kmplete/Graphics/Vulkan/Delegates/vulkan_memory_type_delegate.h"
#include "Kmplete/Graphics/Vulkan/Utils/bits_aliases.h"
#include "Kmplete/Profile/profiler.h"


namespace Kmplete
{
    namespace Graphics
    {
        using namespace VKBits;


        VulkanBufferCreatorDelegate::VulkanBufferCreatorDelegate(VkDevice device, const VulkanMemoryTypeDelegate& memoryTypeDelegate)
            : KMP_PROFILE_CONSTRUCTOR_START_BASE_CLASS()
              _memoryTypeDelegate(memoryTypeDelegate)
            , _device(device)
        {
            KMP_PROFILE_CONSTRUCTOR_END()
        }
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
                .usageFlags = VK_BufferUsage_Vertex | parameters.usageFlags,
                .memoryPropertyFlags = parameters.memoryPropertyFlags,
                .size = parameters.size
            });
        }}
        //--------------------------------------------------------------------------

        Nullable<VulkanVertexBuffer*> VulkanBufferCreatorDelegate::CreateVertexBufferPtr(const VulkanBufferParameters& parameters) const KMP_PROFILING(ProfileLevelImportant)
        {
            return new VulkanVertexBuffer(_memoryTypeDelegate, _device, VulkanBufferParameters{
                .usageFlags = VK_BufferUsage_Vertex | parameters.usageFlags,
                .memoryPropertyFlags = parameters.memoryPropertyFlags,
                .size = parameters.size
            });
        }}
        //--------------------------------------------------------------------------


        VulkanBuffer VulkanBufferCreatorDelegate::CreateIndexBuffer(const VulkanBufferParameters& parameters) const KMP_PROFILING(ProfileLevelImportant)
        {
            return VulkanBuffer(_memoryTypeDelegate, _device, VulkanBufferParameters{
                .usageFlags = VK_BufferUsage_Index | parameters.usageFlags,
                .memoryPropertyFlags = parameters.memoryPropertyFlags,
                .size = parameters.size
            });
        }}
        //--------------------------------------------------------------------------

        Nullable<VulkanBuffer*> VulkanBufferCreatorDelegate::CreateIndexBufferPtr(const VulkanBufferParameters& parameters) const KMP_PROFILING(ProfileLevelImportant)
        {
            return new VulkanBuffer(_memoryTypeDelegate, _device, VulkanBufferParameters{
                .usageFlags = VK_BufferUsage_Index | parameters.usageFlags,
                .memoryPropertyFlags = parameters.memoryPropertyFlags,
                .size = parameters.size
            });
        }}
        //--------------------------------------------------------------------------


        VulkanBuffer VulkanBufferCreatorDelegate::CreateUniformBuffer(const VulkanBufferParameters& parameters) const KMP_PROFILING(ProfileLevelImportant)
        {
            return VulkanBuffer(_memoryTypeDelegate, _device, VulkanBufferParameters{
                .usageFlags = VK_BufferUsage_Uniform | parameters.usageFlags,
                .memoryPropertyFlags = parameters.memoryPropertyFlags,
                .size = parameters.size
            });
        }}
        //--------------------------------------------------------------------------

        Nullable<VulkanBuffer*> VulkanBufferCreatorDelegate::CreateUniformBufferPtr(const VulkanBufferParameters& parameters) const KMP_PROFILING(ProfileLevelImportant)
        {
            return new VulkanBuffer(_memoryTypeDelegate, _device, VulkanBufferParameters{
                .usageFlags = VK_BufferUsage_Uniform | parameters.usageFlags,
                .memoryPropertyFlags = parameters.memoryPropertyFlags,
                .size = parameters.size
            });
        }}
        //--------------------------------------------------------------------------


        VulkanBuffer VulkanBufferCreatorDelegate::CreateStorageBuffer(const VulkanBufferParameters& parameters) const KMP_PROFILING(ProfileLevelImportant)
        {
            return VulkanBuffer(_memoryTypeDelegate, _device, VulkanBufferParameters{
                .usageFlags = VK_BufferUsage_Storage | parameters.usageFlags,
                .memoryPropertyFlags = parameters.memoryPropertyFlags,
                .size = parameters.size
            });
        }}
        //--------------------------------------------------------------------------

        Nullable<VulkanBuffer*> VulkanBufferCreatorDelegate::CreateStorageBufferPtr(const VulkanBufferParameters& parameters) const KMP_PROFILING(ProfileLevelImportant)
        {
            return new VulkanBuffer(_memoryTypeDelegate, _device, VulkanBufferParameters{
                .usageFlags = VK_BufferUsage_Storage | parameters.usageFlags,
                .memoryPropertyFlags = parameters.memoryPropertyFlags,
                .size = parameters.size
            });
        }}
        //--------------------------------------------------------------------------


        VulkanBuffer VulkanBufferCreatorDelegate::CreateIndirectBuffer(const VulkanBufferParameters& parameters) const KMP_PROFILING(ProfileLevelImportant)
        {
            return VulkanBuffer(_memoryTypeDelegate, _device, VulkanBufferParameters{
                .usageFlags = VK_BufferUsage_Indirect | parameters.usageFlags,
                .memoryPropertyFlags = parameters.memoryPropertyFlags,
                .size = parameters.size
            });
        }}
        //--------------------------------------------------------------------------

        Nullable<VulkanBuffer*> VulkanBufferCreatorDelegate::CreateIndirectBufferPtr(const VulkanBufferParameters& parameters) const KMP_PROFILING(ProfileLevelImportant)
        {
            return new VulkanBuffer(_memoryTypeDelegate, _device, VulkanBufferParameters{
                .usageFlags = VK_BufferUsage_Indirect | parameters.usageFlags,
                .memoryPropertyFlags = parameters.memoryPropertyFlags,
                .size = parameters.size
            });
        }}
        //--------------------------------------------------------------------------
    }
}