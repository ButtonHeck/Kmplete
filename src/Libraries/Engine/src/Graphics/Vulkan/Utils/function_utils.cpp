#include "Kmplete/Graphics/Vulkan/Utils/function_utils.h"
#include "Kmplete/Graphics/Vulkan/Utils/initializers.h"
#include "Kmplete/Graphics/Vulkan/Utils/result_description.h"
#include "Kmplete/Graphics/Vulkan/Utils/bits_aliases.h"
#include "Kmplete/Base/named_bool.h"
#include "Kmplete/Profile/profiler.h"
#include "Kmplete/Log/log.h"


namespace Kmplete
{
    namespace Graphics
    {
        using namespace VKBits;


        namespace VKUtils
        {
            SurfaceAndPresentModeProperties QuerySurfaceAndPresentModeProperties(VkPhysicalDevice device, VkSurfaceKHR surface) KMP_PROFILING(ProfileLevelImportant)
            {
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
            }}
            //--------------------------------------------------------------------------

            QueueFamilyIndices QueryQueueFamiliesIndices(VkPhysicalDevice device, VkSurfaceKHR surface) KMP_PROFILING(ProfileLevelImportant)
            {
                QueueFamilyIndices indices;

                UInt32 queueFamilyCount = 0;
                vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

                Vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
                vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

                int index = 0;
                for (const auto& queueFamily : queueFamilies)
                {
                    if (queueFamily.queueFlags & VK_Queue_Graphics)
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
            }}
            //--------------------------------------------------------------------------

            bool QueryDeviceExtensionSupport(VkPhysicalDevice device, const Vector<const char*>& enabledExtensions) KMP_PROFILING(ProfileLevelImportant)
            {
                UInt32 extensionCount;
                vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

                Vector<VkExtensionProperties> availableExtensions(extensionCount);
                vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

                Set<String> requiredExtensions(enabledExtensions.begin(), enabledExtensions.end());

                for (const auto& extension : availableExtensions)
                {
                    requiredExtensions.erase(extension.extensionName);
                }

#if !defined (KMP_CONFIG_TYPE_PRODUCTION)
                if (!requiredExtensions.empty())
                {
                    auto properties2 = VKUtils::InitVkPhysicalDeviceProperties2();
                    vkGetPhysicalDeviceProperties2(device, &properties2);

                    for (const auto& extension : requiredExtensions)
                    {
                        KMP_LOG_WARN_FN("VKUtils::QueryDeviceExtensionSupport: for device '{}' required extension {} not found", properties2.properties.deviceName, extension);
                    }
                }
#endif

                return requiredExtensions.empty();
            }}
            //--------------------------------------------------------------------------

            Pair<bool, Pair<QueueFamilyIndices, SurfaceAndPresentModeProperties>> IsDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface, const Vector<const char*>& enabledExtensions) KMP_PROFILING(ProfileLevelImportant)
            {
                auto properties2 = VKUtils::InitVkPhysicalDeviceProperties2();
                vkGetPhysicalDeviceProperties2(device, &properties2);

                const auto queueFamiliesIndices = QueryQueueFamiliesIndices(device, surface);
                if (!queueFamiliesIndices.IsValid())
                {
                    KMP_LOG_WARN_FN("VKUtils::IsDeviceSuitable: '{}' is not suitable - queue families indices are invalid", properties2.properties.deviceName);
                    return { "device suitable"_false, {} };
                }

                const auto extensionsSupported = QueryDeviceExtensionSupport(device, enabledExtensions);
                if (!extensionsSupported)
                {
                    KMP_LOG_WARN_FN("VKUtils::IsDeviceSuitable: '{}' is not suitable - required extensions are not supported", properties2.properties.deviceName);
                    return { "device suitable"_false, {} };
                }

                const auto surfaceAndPresentModeProperties = QuerySurfaceAndPresentModeProperties(device, surface);
                if (!surfaceAndPresentModeProperties.IsValid())
                {
                    KMP_LOG_WARN_FN("VKUtils::IsDeviceSuitable: '{}' is not suitable - surface and present modes properties are invalid", properties2.properties.deviceName);
                    return { "device suitable"_false, {} };
                }

                VkPhysicalDeviceFeatures supportedFeatures;
                vkGetPhysicalDeviceFeatures(device, &supportedFeatures);
                if (!supportedFeatures.samplerAnisotropy ||
                    !supportedFeatures.independentBlend ||
                    !supportedFeatures.depthBounds)
                {
                    KMP_LOG_WARN_FN("VKUtils::IsDeviceSuitable: '{}' is not suitable - some device features are not supported", properties2.properties.deviceName);
                    return { "device suitable"_false, {} };
                }

                return { "device suitable"_true, { queueFamiliesIndices, surfaceAndPresentModeProperties } };
            }}
            //--------------------------------------------------------------------------

            void InsertImageMemoryBarrier(const MemoryBarrierParameters& barrierParameters) KMP_PROFILING(ProfileLevelImportant)
            {
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
            }}
            //--------------------------------------------------------------------------
        }
    }
}