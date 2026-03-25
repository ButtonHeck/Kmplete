#include "Kmplete/Graphics/Vulkan/Utils/function_utils.h"
#include "Kmplete/Graphics/Vulkan/Utils/initializers.h"
#include "Kmplete/Graphics/Vulkan/Utils/result_description.h"
#include "Kmplete/Base/named_bool.h"
#include "Kmplete/Profile/profiler.h"


namespace Kmplete
{
    namespace Graphics
    {
        namespace VulkanUtils
        {
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
                    return { "device suitable"_false, {} };
                }

                const auto extensionsSupported = QueryDeviceExtensionSupport(device, enabledExtensions);
                if (!extensionsSupported)
                {
                    return { "device suitable"_false, {} };
                }

                const auto surfaceAndPresentModeProperties = QuerySurfaceAndPresentModeProperties(device, surface);
                if (!surfaceAndPresentModeProperties.IsValid())
                {
                    return { "device suitable"_false, {} };
                }

                VkPhysicalDeviceFeatures supportedFeatures;
                vkGetPhysicalDeviceFeatures(device, &supportedFeatures);
                if (!supportedFeatures.samplerAnisotropy)
                {
                    return { "device suitable"_false, {} };
                }

                return { "device suitable"_true, { queueFamiliesIndices, surfaceAndPresentModeProperties } };
            }
            //--------------------------------------------------------------------------

            void InsertImageMemoryBarrier(const MemoryBarrierParameters& barrierParameters)
            {
                KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

                auto imageMemoryBarrier = InitVkImageMemoryBarrier();
                imageMemoryBarrier.srcAccessMask = barrierParameters.srcAccessMask;
                imageMemoryBarrier.dstAccessMask = barrierParameters.dstAccessMask;
                imageMemoryBarrier.oldLayout = barrierParameters.oldImageLayout;
                imageMemoryBarrier.newLayout = barrierParameters.newImageLayout;
                imageMemoryBarrier.image = barrierParameters.image;
                imageMemoryBarrier.subresourceRange = barrierParameters.subresourceRange;

                vkCmdPipelineBarrier(
                    barrierParameters.cmdbuffer,
                    barrierParameters.srcStageMask,
                    barrierParameters.dstStageMask,
                    0,
                    0, nullptr,
                    0, nullptr,
                    1, &imageMemoryBarrier);
            }
            //--------------------------------------------------------------------------
        }
    }
}