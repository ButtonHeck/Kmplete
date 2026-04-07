#include "Kmplete/Graphics/Vulkan/Utils/initializers.h"


namespace Kmplete
{
    namespace Graphics
    {
        namespace VulkanUtils
        {
            VkDebugUtilsMessengerCreateInfoEXT InitVkDebugUtilsMessengerCreateInfo()
            {
                return VkDebugUtilsMessengerCreateInfoEXT{
                    .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
                    .pUserData = nullptr
                };
            }
            //--------------------------------------------------------------------------

            VkApplicationInfo InitVkApplicationInfo()
            {
                return VkApplicationInfo{
                    .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO
                };
            }
            //--------------------------------------------------------------------------

            VkInstanceCreateInfo InitVkInstanceCreateInfo(const VkApplicationInfo& applicationInfo)
            {
                return VkInstanceCreateInfo{
                    .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
                    .pApplicationInfo = &applicationInfo
                };
            }
            //--------------------------------------------------------------------------


            VkPhysicalDeviceProperties2 InitVkPhysicalDeviceProperties2()
            {
                return VkPhysicalDeviceProperties2{
                    .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2
                };
            }
            //--------------------------------------------------------------------------

            VkPhysicalDeviceVulkan11Properties InitVkPhysicalDeviceVulkan11Properties()
            {
                return VkPhysicalDeviceVulkan11Properties{
                    .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_1_PROPERTIES
                };
            }
            //--------------------------------------------------------------------------

            VkPhysicalDeviceVulkan12Properties InitVkPhysicalDeviceVulkan12Properties()
            {
                return VkPhysicalDeviceVulkan12Properties{
                    .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_PROPERTIES
                };
            }
            //--------------------------------------------------------------------------

            VkPhysicalDeviceFeatures2 InitVkPhysicalDeviceFeatures2()
            {
                return VkPhysicalDeviceFeatures2{
                    .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2
                };
            }
            //--------------------------------------------------------------------------

            VkPhysicalDeviceVulkan13Features InitVkPhysicalDeviceVulkan13Features()
            {
                return VkPhysicalDeviceVulkan13Features{
                    .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES
                };
            }
            //--------------------------------------------------------------------------

            VkPhysicalDeviceExtendedDynamicState2FeaturesEXT InitVkPhysicalDeviceExtendedDynamicState2FeaturesEXT()
            {
                return VkPhysicalDeviceExtendedDynamicState2FeaturesEXT{
                    .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTENDED_DYNAMIC_STATE_2_FEATURES_EXT
                };
            }
            //--------------------------------------------------------------------------

            VkPhysicalDeviceExtendedDynamicState3FeaturesEXT InitVkPhysicalDeviceExtendedDynamicState3FeaturesEXT()
            {
                return VkPhysicalDeviceExtendedDynamicState3FeaturesEXT{
                    .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTENDED_DYNAMIC_STATE_3_FEATURES_EXT
                };
            }
            //--------------------------------------------------------------------------

            VkPhysicalDeviceDepthClipEnableFeaturesEXT InitVkPhysicalDeviceDepthClipEnableFeaturesEXT()
            {
                return VkPhysicalDeviceDepthClipEnableFeaturesEXT{
                    .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DEPTH_CLIP_ENABLE_FEATURES_EXT
                };
            }
            //--------------------------------------------------------------------------

            VkPhysicalDeviceColorWriteEnableFeaturesEXT InitVkPhysicalDeviceColorWriteEnableFeaturesEXT()
            {
                return VkPhysicalDeviceColorWriteEnableFeaturesEXT{
                    .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_COLOR_WRITE_ENABLE_FEATURES_EXT
                };
            }
            //--------------------------------------------------------------------------


            VkDeviceCreateInfo InitVkDeviceCreateInfo()
            {
                return VkDeviceCreateInfo{
                    .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO
                };
            }
            //--------------------------------------------------------------------------

            VkSemaphoreCreateInfo InitVkSemaphoreCreateInfo()
            {
                return VkSemaphoreCreateInfo{
                    .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO
                };
            }
            //--------------------------------------------------------------------------

            VkCommandBufferAllocateInfo InitVkCommandBufferAllocateInfo(bool primary /*= true*/)
            {
                return VkCommandBufferAllocateInfo{
                    .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
                    .level = primary ? VK_COMMAND_BUFFER_LEVEL_PRIMARY : VK_COMMAND_BUFFER_LEVEL_SECONDARY
                };
            }
            //--------------------------------------------------------------------------

            VkCommandBufferBeginInfo InitVkCommandBufferBeginInfo()
            {
                return VkCommandBufferBeginInfo{
                    .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO
                };
            }
            //--------------------------------------------------------------------------

            VkFenceCreateInfo InitVkFenceCreateInfo(bool signaled /*= true*/)
            {
                VkFenceCreateInfo fenceCreateInfo{};
                fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
                if (signaled)
                {
                    fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
                }
                return fenceCreateInfo;
            }
            //--------------------------------------------------------------------------

            VkDescriptorPoolCreateInfo InitVkDescriptorPoolCreateInfo()
            {
                return VkDescriptorPoolCreateInfo{
                    .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
                    .flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT
                };
            }
            //--------------------------------------------------------------------------

            VkDeviceQueueCreateInfo InitVkDeviceQueueCreateInfo()
            {
                return VkDeviceQueueCreateInfo{
                    .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO
                };
            }
            //--------------------------------------------------------------------------

            VkCommandPoolCreateInfo InitVkCommandPoolCreateInfo()
            {
                return VkCommandPoolCreateInfo{
                    .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO
                };
            }
            //--------------------------------------------------------------------------

            VkSwapchainCreateInfoKHR InitVkSwapchainCreateInfoKHR()
            {
                return VkSwapchainCreateInfoKHR{
                    .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR
                };
            }
            //--------------------------------------------------------------------------

            VkSubmitInfo InitVkSubmitInfo()
            {
                return VkSubmitInfo{
                    .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO
                };
            }
            //--------------------------------------------------------------------------

            VkPresentInfoKHR InitVkPresentInfoKHR()
            {
                return VkPresentInfoKHR{
                    .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR
                };
            }
            //--------------------------------------------------------------------------

            VkRenderingAttachmentInfo InitVkRenderingAttachmentInfo()
            {
                return VkRenderingAttachmentInfo{
                    .sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO
                };
            }
            //--------------------------------------------------------------------------

            VkRenderingInfo InitVkRenderingInfo()
            {
                return VkRenderingInfo{
                    .sType = VK_STRUCTURE_TYPE_RENDERING_INFO_KHR
                };
            }
            //--------------------------------------------------------------------------

            VkDescriptorSetLayoutCreateInfo InitVkDescriptorSetLayoutCreateInfo()
            {
                return VkDescriptorSetLayoutCreateInfo{
                    .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO
                };
            }
            //--------------------------------------------------------------------------

            VkDescriptorSetAllocateInfo InitVkDescriptorSetAllocateInfo()
            {
                return VkDescriptorSetAllocateInfo{
                    .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO
                };
            }
            //--------------------------------------------------------------------------

            VkWriteDescriptorSet InitVkWriteDescriptorSet()
            {
                return VkWriteDescriptorSet{
                    .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET
                };
            }
            //--------------------------------------------------------------------------


            VkPipelineLayoutCreateInfo InitVkPipelineLayoutCreateInfo()
            {
                return VkPipelineLayoutCreateInfo{
                    .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO
                };
            }
            //--------------------------------------------------------------------------

            VkPipelineCacheCreateInfo InitVkPipelineCacheCreateInfo()
            {
                return VkPipelineCacheCreateInfo{
                    .sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO
                };
            }
            //--------------------------------------------------------------------------

            VkPipelineRenderingCreateInfoKHR InitVkPipelineRenderingCreateInfoKHR()
            {
                return VkPipelineRenderingCreateInfoKHR{
                    .sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO
                };
            }
            //--------------------------------------------------------------------------

            VkGraphicsPipelineCreateInfo InitVkGraphicsPipelineCreateInfo()
            {
                return VkGraphicsPipelineCreateInfo{
                    .sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO
                };
            }
            //--------------------------------------------------------------------------

            VkPipelineInputAssemblyStateCreateInfo InitVkPipelineInputAssemblyStateCreateInfo()
            {
                return VkPipelineInputAssemblyStateCreateInfo{
                    .sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO
                };
            }
            //--------------------------------------------------------------------------

            VkPipelineRasterizationStateCreateInfo InitVkPipelineRasterizationStateCreateInfo()
            {
                return VkPipelineRasterizationStateCreateInfo{
                    .sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO
                };
            }
            //--------------------------------------------------------------------------

            VkPipelineColorBlendStateCreateInfo InitVkPipelineColorBlendStateCreateInfo()
            {
                return VkPipelineColorBlendStateCreateInfo{
                    .sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO
                };
            }
            //--------------------------------------------------------------------------

            VkPipelineViewportStateCreateInfo InitVkPipelineViewportStateCreateInfo()
            {
                return VkPipelineViewportStateCreateInfo{
                    .sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO
                };
            }
            //--------------------------------------------------------------------------

            VkPipelineDynamicStateCreateInfo InitVkPipelineDynamicStateCreateInfo()
            {
                return VkPipelineDynamicStateCreateInfo{
                    .sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO
                };
            }
            //--------------------------------------------------------------------------

            VkPipelineDepthStencilStateCreateInfo InitVkPipelineDepthStencilStateCreateInfo()
            {
                return VkPipelineDepthStencilStateCreateInfo{
                    .sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO
                };
            }
            //--------------------------------------------------------------------------

            VkPipelineMultisampleStateCreateInfo InitVkPipelineMultisampleStateCreateInfo()
            {
                return VkPipelineMultisampleStateCreateInfo{
                    .sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO
                };
            }
            //--------------------------------------------------------------------------

            VkPipelineVertexInputStateCreateInfo InitVkPipelineVertexInputStateCreateInfo()
            {
                return VkPipelineVertexInputStateCreateInfo{
                    .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO
                };
            }
            //--------------------------------------------------------------------------

            VkPipelineShaderStageCreateInfo InitVkPipelineShaderStageCreateInfo(VkShaderStageFlagBits stage)
            {
                return VkPipelineShaderStageCreateInfo{
                    .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
                    .stage = stage
                };
            }
            //--------------------------------------------------------------------------


            VkImageCreateInfo InitVkImageCreateInfo()
            {
                return VkImageCreateInfo{
                    .sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO
                };
            }
            //--------------------------------------------------------------------------

            VkImageViewCreateInfo InitVkImageViewCreateInfo()
            {
                return VkImageViewCreateInfo{
                    .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO
                };
            }
            //--------------------------------------------------------------------------

            VkImageMemoryBarrier InitVkImageMemoryBarrier()
            {
                return VkImageMemoryBarrier{
                    .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
                    .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
                    .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED
                };
            }
            //--------------------------------------------------------------------------

            VkSamplerCreateInfo InitVkSamplerCreateInfo()
            {
                return VkSamplerCreateInfo{
                    .sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO
                };
            }
            //--------------------------------------------------------------------------


            VkMemoryAllocateInfo InitVkMemoryAllocateInfo()
            {
                return VkMemoryAllocateInfo{
                    .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO
                };
            }
            //--------------------------------------------------------------------------

            VkMemoryAllocateFlagsInfoKHR InitVkMemoryAllocateFlagsInfoKHR()
            {
                return VkMemoryAllocateFlagsInfoKHR{
                    .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_FLAGS_INFO_KHR
                };
            }
            //--------------------------------------------------------------------------

            VkMappedMemoryRange InitVkMappedMemoryRange(VkDeviceSize size, VkDeviceSize offset)
            {
                return VkMappedMemoryRange{
                    .sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE,
                    .offset = offset,
                    .size = size
                };
            }
            //--------------------------------------------------------------------------

            VkBufferCreateInfo InitVkBufferCreateInfo(VkDeviceSize size, VkBufferUsageFlags usageFlags)
            {
                return VkBufferCreateInfo{
                    .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
                    .size = size,
                    .usage = usageFlags
                };
            }
            //--------------------------------------------------------------------------

            VkShaderModuleCreateInfo InitVkShaderModuleCreateInfo()
            {
                return VkShaderModuleCreateInfo{
                    .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO
                };
            }
            //--------------------------------------------------------------------------
        }
    }
}