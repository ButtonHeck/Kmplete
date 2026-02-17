#include "Kmplete/Graphics/Vulkan/vulkan_utils.h"


namespace Kmplete
{
    namespace Graphics
    {
        namespace VulkanUtils
        {
            VkDebugUtilsMessengerCreateInfoEXT GetVkDebugUtilsMessengerCreateInfo()
            {
                VkDebugUtilsMessengerCreateInfoEXT debugMessengerCreateInfo{};
                debugMessengerCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
                debugMessengerCreateInfo.pUserData = nullptr;
                return debugMessengerCreateInfo;
            }
            //--------------------------------------------------------------------------

            VkApplicationInfo GetVkApplicationInfo()
            {
                VkApplicationInfo applicationInfo{};
                applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
                return applicationInfo;
            }
            //--------------------------------------------------------------------------

            VkInstanceCreateInfo GetVkInstanceCreateInfo()
            {
                VkInstanceCreateInfo instanceCreateInfo{};
                instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
                return instanceCreateInfo;
            }
            //--------------------------------------------------------------------------

            VkPhysicalDeviceDynamicRenderingFeatures GetVkPhysicalDeviceDynamicRenderingFeatures()
            {
                VkPhysicalDeviceDynamicRenderingFeatures dynamicRenderingFeatures{};
                dynamicRenderingFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DYNAMIC_RENDERING_FEATURES_KHR;
                dynamicRenderingFeatures.dynamicRendering = VK_TRUE;
                return dynamicRenderingFeatures;
            }
            //--------------------------------------------------------------------------

            VkPhysicalDeviceProperties2 GetVkPhysicalDeviceProperties2()
            {
                VkPhysicalDeviceProperties2 properties2{};
                properties2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2;
                return properties2;
            }
            //--------------------------------------------------------------------------

            VkPhysicalDeviceVulkan11Properties GetVkPhysicalDeviceVulkan11Properties()
            {
                VkPhysicalDeviceVulkan11Properties propertiesVersion11{};
                propertiesVersion11.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_1_PROPERTIES;
                return propertiesVersion11;
            }
            //--------------------------------------------------------------------------

            VkPhysicalDeviceVulkan12Properties GetVkPhysicalDeviceVulkan12Properties()
            {
                VkPhysicalDeviceVulkan12Properties propertiesVersion12{};
                propertiesVersion12.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_PROPERTIES;
                return propertiesVersion12;
            }
            //--------------------------------------------------------------------------

            VkDeviceCreateInfo GetVkDeviceCreateInfo()
            {
                VkDeviceCreateInfo deviceCreateInfo{};
                deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
                return deviceCreateInfo;
            }
            //--------------------------------------------------------------------------

            VkSemaphoreCreateInfo GetVkSemaphoreCreateInfo()
            {
                VkSemaphoreCreateInfo semaphoreCreateInfo{};
                semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
                return semaphoreCreateInfo;
            }
            //--------------------------------------------------------------------------

            VkCommandBufferAllocateInfo GetVkCommandBufferAllocateInfo(bool primary /*= true*/)
            {
                VkCommandBufferAllocateInfo commandBufferAllocateInfo{};
                commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
                commandBufferAllocateInfo.level = primary ? VK_COMMAND_BUFFER_LEVEL_PRIMARY : VK_COMMAND_BUFFER_LEVEL_SECONDARY;
                return commandBufferAllocateInfo;
            }
            //--------------------------------------------------------------------------

            VkFenceCreateInfo GetVkFenceCreateInfo(bool signaled /*= true*/)
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

            VkPipelineCacheCreateInfo GetVkPipelineCacheCreateInfo()
            {
                VkPipelineCacheCreateInfo pipelineCacheCreateInfo{};
                pipelineCacheCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
                return pipelineCacheCreateInfo;
            }
            //--------------------------------------------------------------------------

            VkDescriptorPoolCreateInfo GetVkDescriptorPoolCreateInfo()
            {
                VkDescriptorPoolCreateInfo poolCreateInfo{};
                poolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
                poolCreateInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
                return poolCreateInfo;
            }
            //--------------------------------------------------------------------------

            VkDeviceQueueCreateInfo GetVkDeviceQueueCreateInfo()
            {
                VkDeviceQueueCreateInfo queueCreateInfo{};
                queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
                return queueCreateInfo;
            }
            //--------------------------------------------------------------------------

            VkCommandPoolCreateInfo GetVkCommandPoolCreateInfo()
            {
                VkCommandPoolCreateInfo commandPoolCreateInfo{};
                commandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
                return commandPoolCreateInfo;
            }
            //--------------------------------------------------------------------------

            VkSwapchainCreateInfoKHR GetVkSwapchainCreateInfoKHR()
            {
                VkSwapchainCreateInfoKHR swapchainCreateInfo{};
                swapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
                return swapchainCreateInfo;
            }
            //--------------------------------------------------------------------------

            VkPipelineRenderingCreateInfoKHR GetVkPipelineRenderingCreateInfoKHR()
            {
                VkPipelineRenderingCreateInfoKHR pipelineRenderingCreateInfo{};
                pipelineRenderingCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO;
                return pipelineRenderingCreateInfo;
            }
            //--------------------------------------------------------------------------

            VkImageCreateInfo GetVkImageCreateInfo()
            {
                VkImageCreateInfo imageCreationInfo{};
                imageCreationInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
                return imageCreationInfo;
            }
            //--------------------------------------------------------------------------

            VkImageViewCreateInfo GetVkImageViewCreateInfo()
            {
                VkImageViewCreateInfo viewCreateInfo{};
                viewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
                return viewCreateInfo;
            }
            //--------------------------------------------------------------------------

            VkMemoryAllocateInfo GetVkMemoryAllocateInfo()
            {
                VkMemoryAllocateInfo memoryAllocateInfo{};
                memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
                return memoryAllocateInfo;
            }
            //--------------------------------------------------------------------------

            VkMemoryAllocateFlagsInfoKHR GetVkMemoryAllocateFlagsInfoKHR()
            {
                VkMemoryAllocateFlagsInfoKHR memoryAllocateFlagsInfo{};
                memoryAllocateFlagsInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_FLAGS_INFO_KHR;
                return memoryAllocateFlagsInfo;
            }
            //--------------------------------------------------------------------------

            VkMappedMemoryRange GetVkMappedMemoryRange(VkDeviceSize size, VkDeviceSize offset)
            {
                VkMappedMemoryRange mappedRange{};
                mappedRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
                mappedRange.size = size;
                mappedRange.offset = offset;
                return mappedRange;
            }
            //--------------------------------------------------------------------------

            VkBufferCreateInfo GetVkBufferCreateInfo(VkDeviceSize size, VkBufferUsageFlags usageFlags)
            {
                VkBufferCreateInfo bufferCreateInfo{};
                bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
                bufferCreateInfo.size = size;
                bufferCreateInfo.usage = usageFlags;
                return bufferCreateInfo;
            }
            //--------------------------------------------------------------------------
        }
    }
}