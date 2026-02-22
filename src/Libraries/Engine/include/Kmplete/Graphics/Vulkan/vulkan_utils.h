#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/optional.h"
#include "Kmplete/Base/types_aliases.h"

#include <vulkan/vulkan.h>

#include <utility>


namespace Kmplete
{
    namespace Graphics
    {
        namespace VulkanUtils
        {
            KMP_NODISCARD KMP_API VkDebugUtilsMessengerCreateInfoEXT GetVkDebugUtilsMessengerCreateInfo();
            KMP_NODISCARD KMP_API VkApplicationInfo GetVkApplicationInfo();
            KMP_NODISCARD KMP_API VkInstanceCreateInfo GetVkInstanceCreateInfo();

            KMP_NODISCARD KMP_API VkPhysicalDeviceDynamicRenderingFeatures GetVkPhysicalDeviceDynamicRenderingFeatures();
            KMP_NODISCARD KMP_API VkPhysicalDeviceProperties2 GetVkPhysicalDeviceProperties2();
            KMP_NODISCARD KMP_API VkPhysicalDeviceVulkan11Properties GetVkPhysicalDeviceVulkan11Properties();
            KMP_NODISCARD KMP_API VkPhysicalDeviceVulkan12Properties GetVkPhysicalDeviceVulkan12Properties();

            KMP_NODISCARD KMP_API VkDeviceCreateInfo GetVkDeviceCreateInfo();
            KMP_NODISCARD KMP_API VkSemaphoreCreateInfo GetVkSemaphoreCreateInfo();
            KMP_NODISCARD KMP_API VkCommandBufferAllocateInfo GetVkCommandBufferAllocateInfo(bool primary = true);
            KMP_NODISCARD KMP_API VkCommandBufferBeginInfo GetVkCommandBufferBeginInfo();
            KMP_NODISCARD KMP_API VkFenceCreateInfo GetVkFenceCreateInfo(bool signaled = true);
            KMP_NODISCARD KMP_API VkPipelineCacheCreateInfo GetVkPipelineCacheCreateInfo();
            KMP_NODISCARD KMP_API VkDescriptorPoolCreateInfo GetVkDescriptorPoolCreateInfo();
            KMP_NODISCARD KMP_API VkDeviceQueueCreateInfo GetVkDeviceQueueCreateInfo();
            KMP_NODISCARD KMP_API VkCommandPoolCreateInfo GetVkCommandPoolCreateInfo();
            KMP_NODISCARD KMP_API VkSwapchainCreateInfoKHR GetVkSwapchainCreateInfoKHR();
            KMP_NODISCARD KMP_API VkSubmitInfo GetVkSubmitInfo();
            KMP_NODISCARD KMP_API VkPresentInfoKHR GetVkPresentInfoKHR();
            KMP_NODISCARD KMP_API VkPipelineRenderingCreateInfoKHR GetVkPipelineRenderingCreateInfoKHR();
            KMP_NODISCARD KMP_API VkRenderingAttachmentInfo GetVkRenderingAttachmentInfo();
            KMP_NODISCARD KMP_API VkRenderingInfo GetVkRenderingInfo();

            KMP_NODISCARD KMP_API VkImageCreateInfo GetVkImageCreateInfo();
            KMP_NODISCARD KMP_API VkImageViewCreateInfo GetVkImageViewCreateInfo();
            KMP_NODISCARD KMP_API VkImageMemoryBarrier GetVkImageMemoryBarrier();

            KMP_NODISCARD KMP_API VkMemoryAllocateInfo GetVkMemoryAllocateInfo();
            KMP_NODISCARD KMP_API VkMemoryAllocateFlagsInfoKHR GetVkMemoryAllocateFlagsInfoKHR();
            KMP_NODISCARD KMP_API VkMappedMemoryRange GetVkMappedMemoryRange(VkDeviceSize size, VkDeviceSize offset);

            KMP_NODISCARD KMP_API VkBufferCreateInfo GetVkBufferCreateInfo(VkDeviceSize size, VkBufferUsageFlags usageFlags);

            KMP_API VkResult CheckResult(VkResult result, const char* message, bool throwException = true);

            KMP_API void InsertImageMemoryBarrier(
                VkCommandBuffer cmdbuffer,
                VkImage image,
                VkAccessFlags srcAccessMask,
                VkAccessFlags dstAccessMask,
                VkImageLayout oldImageLayout,
                VkImageLayout newImageLayout,
                VkPipelineStageFlags srcStageMask,
                VkPipelineStageFlags dstStageMask,
                VkImageSubresourceRange subresourceRange
            );

            //TODO: comments
            struct QueueFamilyIndices
            {
                Optional<UInt32> graphicsFamilyIndex{};
                Optional<UInt32> presentFamilyIndex{};

                inline bool IsValid() const noexcept
                {
                    return graphicsFamilyIndex.has_value() && presentFamilyIndex.has_value();
                }
            };
            //--------------------------------------------------------------------------

            //TODO: comments
            struct SurfaceAndPresentModeProperties
            {
                VkSurfaceCapabilitiesKHR surfaceCapabilities{};
                Vector<VkSurfaceFormatKHR> surfaceFormats{};
                Vector<VkPresentModeKHR> presentModes{};

                inline bool IsValid() const noexcept
                {
                    return !surfaceFormats.empty() && !presentModes.empty();
                }
            };
            //--------------------------------------------------------------------------

            KMP_NODISCARD SurfaceAndPresentModeProperties QuerySurfaceAndPresentModeProperties(VkPhysicalDevice device, VkSurfaceKHR surface);
            KMP_NODISCARD QueueFamilyIndices QueryQueueFamiliesIndices(VkPhysicalDevice device, VkSurfaceKHR surface);
            KMP_NODISCARD bool QueryDeviceExtensionSupport(VkPhysicalDevice device, const Vector<const char*>& enabledExtensions);
            KMP_NODISCARD std::pair<bool, std::pair<QueueFamilyIndices, SurfaceAndPresentModeProperties>> IsDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface, const Vector<const char*>& enabledExtensions);
        }
        //--------------------------------------------------------------------------
    }
}