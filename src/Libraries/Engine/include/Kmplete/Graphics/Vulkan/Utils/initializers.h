#pragma once

#include "Kmplete/Base/kmplete_api.h"

#include <vulkan/vulkan.h>


namespace Kmplete
{
    namespace Graphics
    {
        namespace VulkanUtils
        {
            KMP_NODISCARD KMP_API VkDebugUtilsMessengerCreateInfoEXT InitVkDebugUtilsMessengerCreateInfo();
            KMP_NODISCARD KMP_API VkApplicationInfo InitVkApplicationInfo();
            KMP_NODISCARD KMP_API VkInstanceCreateInfo InitVkInstanceCreateInfo();

            KMP_NODISCARD KMP_API VkPhysicalDeviceDynamicRenderingFeatures InitVkPhysicalDeviceDynamicRenderingFeatures();
            KMP_NODISCARD KMP_API VkPhysicalDeviceProperties2 InitVkPhysicalDeviceProperties2();
            KMP_NODISCARD KMP_API VkPhysicalDeviceVulkan11Properties InitVkPhysicalDeviceVulkan11Properties();
            KMP_NODISCARD KMP_API VkPhysicalDeviceVulkan12Properties InitVkPhysicalDeviceVulkan12Properties();
            KMP_NODISCARD KMP_API VkPhysicalDeviceFeatures2 InitVkPhysicalDeviceFeatures2();
            KMP_NODISCARD KMP_API VkPhysicalDeviceVulkan13Features InitVkPhysicalDeviceVulkan13Features();

            KMP_NODISCARD KMP_API VkDeviceCreateInfo InitVkDeviceCreateInfo();
            KMP_NODISCARD KMP_API VkSemaphoreCreateInfo InitVkSemaphoreCreateInfo();
            KMP_NODISCARD KMP_API VkCommandBufferAllocateInfo InitVkCommandBufferAllocateInfo(bool primary = true);
            KMP_NODISCARD KMP_API VkCommandBufferBeginInfo InitVkCommandBufferBeginInfo();
            KMP_NODISCARD KMP_API VkFenceCreateInfo InitVkFenceCreateInfo(bool signaled = true);
            KMP_NODISCARD KMP_API VkDescriptorPoolCreateInfo InitVkDescriptorPoolCreateInfo();
            KMP_NODISCARD KMP_API VkDeviceQueueCreateInfo InitVkDeviceQueueCreateInfo();
            KMP_NODISCARD KMP_API VkCommandPoolCreateInfo InitVkCommandPoolCreateInfo();
            KMP_NODISCARD KMP_API VkSwapchainCreateInfoKHR InitVkSwapchainCreateInfoKHR();
            KMP_NODISCARD KMP_API VkSubmitInfo InitVkSubmitInfo();
            KMP_NODISCARD KMP_API VkPresentInfoKHR InitVkPresentInfoKHR();
            KMP_NODISCARD KMP_API VkRenderingAttachmentInfo InitVkRenderingAttachmentInfo();
            KMP_NODISCARD KMP_API VkRenderingInfo InitVkRenderingInfo();
            KMP_NODISCARD KMP_API VkDescriptorSetLayoutCreateInfo InitVkDescriptorSetLayoutCreateInfo();

            KMP_NODISCARD KMP_API VkPipelineLayoutCreateInfo InitVkPipelineLayoutCreateInfo();
            KMP_NODISCARD KMP_API VkPipelineCacheCreateInfo InitVkPipelineCacheCreateInfo();
            KMP_NODISCARD KMP_API VkPipelineRenderingCreateInfoKHR InitVkPipelineRenderingCreateInfoKHR();
            KMP_NODISCARD KMP_API VkGraphicsPipelineCreateInfo InitVkGraphicsPipelineCreateInfo();
            KMP_NODISCARD KMP_API VkPipelineInputAssemblyStateCreateInfo InitVkPipelineInputAssemblyStateCreateInfo();
            KMP_NODISCARD KMP_API VkPipelineRasterizationStateCreateInfo InitVkPipelineRasterizationStateCreateInfo();
            KMP_NODISCARD KMP_API VkPipelineColorBlendStateCreateInfo InitVkPipelineColorBlendStateCreateInfo();
            KMP_NODISCARD KMP_API VkPipelineViewportStateCreateInfo InitVkPipelineViewportStateCreateInfo();
            KMP_NODISCARD KMP_API VkPipelineDynamicStateCreateInfo InitVkPipelineDynamicStateCreateInfo();
            KMP_NODISCARD KMP_API VkPipelineDepthStencilStateCreateInfo InitVkPipelineDepthStencilStateCreateInfo();
            KMP_NODISCARD KMP_API VkPipelineMultisampleStateCreateInfo InitVkPipelineMultisampleStateCreateInfo();
            KMP_NODISCARD KMP_API VkPipelineVertexInputStateCreateInfo InitVkPipelineVertexInputStateCreateInfo();

            KMP_NODISCARD KMP_API VkImageCreateInfo InitVkImageCreateInfo();
            KMP_NODISCARD KMP_API VkImageViewCreateInfo InitVkImageViewCreateInfo();
            KMP_NODISCARD KMP_API VkImageMemoryBarrier InitVkImageMemoryBarrier();
            KMP_NODISCARD KMP_API VkSamplerCreateInfo InitVkSamplerCreateInfo();

            KMP_NODISCARD KMP_API VkMemoryAllocateInfo InitVkMemoryAllocateInfo();
            KMP_NODISCARD KMP_API VkMemoryAllocateFlagsInfoKHR InitVkMemoryAllocateFlagsInfoKHR();
            KMP_NODISCARD KMP_API VkMappedMemoryRange InitVkMappedMemoryRange(VkDeviceSize size, VkDeviceSize offset);

            KMP_NODISCARD KMP_API VkBufferCreateInfo InitVkBufferCreateInfo(VkDeviceSize size, VkBufferUsageFlags usageFlags);

            KMP_NODISCARD KMP_API VkShaderModuleCreateInfo InitVkShaderModuleCreateInfo();
        }
    }
}