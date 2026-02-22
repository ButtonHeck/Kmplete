#include "Kmplete/Graphics/Vulkan/vulkan_utils.h"
#include "Kmplete/Graphics/Vulkan/vulkan_result_description.h"
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

            VkCommandBufferBeginInfo GetVkCommandBufferBeginInfo()
            {
                VkCommandBufferBeginInfo commandBufferBeginInfo{};
                commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
                return commandBufferBeginInfo;
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

            VkSubmitInfo GetVkSubmitInfo()
            {
                VkSubmitInfo submitInfo{};
                submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
                return submitInfo;
            }
            //--------------------------------------------------------------------------

            VkPresentInfoKHR GetVkPresentInfoKHR()
            {
                VkPresentInfoKHR presentInfo{};
                presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
                presentInfo.pNext = nullptr;
                return presentInfo;
            }
            //--------------------------------------------------------------------------

            VkPipelineRenderingCreateInfoKHR GetVkPipelineRenderingCreateInfoKHR()
            {
                VkPipelineRenderingCreateInfoKHR pipelineRenderingCreateInfo{};
                pipelineRenderingCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO;
                return pipelineRenderingCreateInfo;
            }
            //--------------------------------------------------------------------------

            VkRenderingAttachmentInfo GetVkRenderingAttachmentInfo()
            {
                VkRenderingAttachmentInfo colorAttachment{};
                colorAttachment.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO;
                return colorAttachment;
            }
            //--------------------------------------------------------------------------

            VkRenderingInfo GetVkRenderingInfo()
            {
                VkRenderingInfo renderingInfo{};
                renderingInfo.sType = VK_STRUCTURE_TYPE_RENDERING_INFO_KHR;
                return renderingInfo;
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

            VkImageMemoryBarrier GetVkImageMemoryBarrier()
            {
                VkImageMemoryBarrier imageMemoryBarrier{};
                imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
                imageMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
                imageMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
                return imageMemoryBarrier;
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
                auto imageMemoryBarrier = GetVkImageMemoryBarrier();
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
        }
    }
}