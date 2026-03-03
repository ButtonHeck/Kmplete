#include "Kmplete/Graphics/Vulkan/Utils/function_utils.h"
#include "Kmplete/Graphics/Vulkan/Utils/initializers.h"
#include "Kmplete/Graphics/Vulkan/Utils/result_description.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Log/log.h"
#include "Kmplete/Profile/profiler.h"

#include <stdexcept>


namespace Kmplete
{
    namespace Graphics
    {
        namespace VulkanUtils
        {
            VkResult CheckResult(VkResult result, const char* message, bool throwException /*= true*/)
            {
                if (result != VK_SUCCESS)
                {
                    const auto resultDescription = VkResultToString(result);
                    KMP_LOG_CRITICAL_FN("{}: {}", message, resultDescription);
                    if (throwException)
                    {
                        throw std::runtime_error(String(message).append(": ").append(resultDescription));
                    }
                }

                return result;
            }
            //--------------------------------------------------------------------------

            void InsertImageMemoryBarrier(
                VkCommandBuffer cmdbuffer, 
                VkImage image, 
                VkAccessFlags srcAccessMask,
                VkAccessFlags dstAccessMask, 
                VkImageLayout oldImageLayout, 
                VkImageLayout newImageLayout, 
                VkPipelineStageFlags srcStageMask, 
                VkPipelineStageFlags dstStageMask, 
                VkImageSubresourceRange subresourceRange)
            {
                KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

                auto imageMemoryBarrier = InitVkImageMemoryBarrier();
                imageMemoryBarrier.srcAccessMask = srcAccessMask;
                imageMemoryBarrier.dstAccessMask = dstAccessMask;
                imageMemoryBarrier.oldLayout = oldImageLayout;
                imageMemoryBarrier.newLayout = newImageLayout;
                imageMemoryBarrier.image = image;
                imageMemoryBarrier.subresourceRange = subresourceRange;

                vkCmdPipelineBarrier(
                    cmdbuffer,
                    srcStageMask,
                    dstStageMask,
                    0,
                    0, nullptr,
                    0, nullptr,
                    1, &imageMemoryBarrier);
            }
            //--------------------------------------------------------------------------

            SurfaceAndPresentModeProperties QuerySurfaceAndPresentModeProperties(VkPhysicalDevice device, VkSurfaceKHR surface)
            {
                KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

                SurfaceAndPresentModeProperties properties;
                vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &properties.surfaceCapabilities);

                UInt32 formatCount = 0;
                vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);
                if (formatCount != 0)
                {
                    properties.surfaceFormats.resize(formatCount);
                    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, properties.surfaceFormats.data());
                }

                UInt32 presentModeCount = 0;
                vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);
                if (presentModeCount != 0)
                {
                    properties.presentModes.resize(presentModeCount);
                    vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, properties.presentModes.data());
                }

                return properties;
            }
            //--------------------------------------------------------------------------

            QueueFamilyIndices QueryQueueFamiliesIndices(VkPhysicalDevice device, VkSurfaceKHR surface)
            {
                KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

                QueueFamilyIndices indices;

                UInt32 queueFamilyCount = 0;
                vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

                Vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
                vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

                int index = 0;
                for (const auto& queueFamily : queueFamilies)
                {
                    if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
                    {
                        indices.graphicsFamilyIndex = index;
                    }

                    VkBool32 presentFamilySupport = false;
                    vkGetPhysicalDeviceSurfaceSupportKHR(device, index, surface, &presentFamilySupport);
                    if (presentFamilySupport)
                    {
                        indices.presentFamilyIndex = index;
                    }

                    if (indices.IsValid())
                    {
                        break;
                    }

                    index++;
                }

                return indices;
            }
            //--------------------------------------------------------------------------

            bool QueryDeviceExtensionSupport(VkPhysicalDevice device, const Vector<const char*>& enabledExtensions)
            {
                KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

                UInt32 extensionCount;
                vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

                Vector<VkExtensionProperties> availableExtensions(extensionCount);
                vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

                Set<String> requiredExtensions(enabledExtensions.begin(), enabledExtensions.end());

                for (const auto& extension : availableExtensions)
                {
                    requiredExtensions.erase(extension.extensionName);
                }

                return requiredExtensions.empty();
            }
            //--------------------------------------------------------------------------

            std::pair<bool, std::pair<QueueFamilyIndices, SurfaceAndPresentModeProperties>> IsDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface, const Vector<const char*>& enabledExtensions)
            {
                KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

                const auto queueFamiliesIndices = QueryQueueFamiliesIndices(device, surface);
                if (!queueFamiliesIndices.IsValid())
                {
                    return { false, {} };
                }

                const auto extensionsSupported = QueryDeviceExtensionSupport(device, enabledExtensions);
                if (!extensionsSupported)
                {
                    return { false, {} };
                }

                const auto surfaceAndPresentModeProperties = QuerySurfaceAndPresentModeProperties(device, surface);
                if (!surfaceAndPresentModeProperties.IsValid())
                {
                    return { false, {} };
                }

                VkPhysicalDeviceFeatures supportedFeatures;
                vkGetPhysicalDeviceFeatures(device, &supportedFeatures);
                if (!supportedFeatures.samplerAnisotropy)
                {
                    return { false, {} };
                }

                return { true, { queueFamiliesIndices, surfaceAndPresentModeProperties } };
            }
            //--------------------------------------------------------------------------

            VkCommandBuffer StartSingleTimeCommandBuffer(VkDevice logicalDevice, VkCommandPool commandPool)
            {
                KMP_PROFILE_FUNCTION(ProfileLevelMinorFunctions);

                auto commandBufferAllocateInfo = InitVkCommandBufferAllocateInfo();
                commandBufferAllocateInfo.commandPool = commandPool;
                commandBufferAllocateInfo.commandBufferCount = 1;

                VkCommandBuffer commandBuffer;
                auto result = vkAllocateCommandBuffers(logicalDevice, &commandBufferAllocateInfo, &commandBuffer);
                CheckResult(result, "failed to allocate command buffers");

                auto commandBufferBeginInfo = InitVkCommandBufferBeginInfo();
                commandBufferBeginInfo.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

                result = vkBeginCommandBuffer(commandBuffer, &commandBufferBeginInfo);
                CheckResult(result, "failed to begin command buffer");

                return commandBuffer;
            }
            //--------------------------------------------------------------------------

            void EndSingleTimeCommandBuffer(VkDevice logicalDevice, VkCommandBuffer commandBuffer, VkCommandPool commandPool, VkQueue graphicsQueue)
            {
                KMP_PROFILE_FUNCTION(ProfileLevelMinorFunctions);

                auto result = vkEndCommandBuffer(commandBuffer);
                CheckResult(result, "failed to end command buffer");

                auto submitInfo = InitVkSubmitInfo();
                submitInfo.commandBufferCount = 1;
                submitInfo.pCommandBuffers = &commandBuffer;

                result = vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
                CheckResult(result, "failed to submit to queue");

                result = vkDeviceWaitIdle(logicalDevice);
                CheckResult(result, "failed to wait device to be idle");

                vkFreeCommandBuffers(logicalDevice, commandPool, 1, &commandBuffer);
            }
            //--------------------------------------------------------------------------
        }
    }
}