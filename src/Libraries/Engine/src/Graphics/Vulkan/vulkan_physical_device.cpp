#include "Kmplete/Graphics/Vulkan/vulkan_physical_device.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Base/optional.h"
#include "Kmplete/Log/log.h"

#include <stdexcept>


namespace Kmplete
{
    namespace Graphics
    {
        static const Vector<const char*> DeviceExtensions = 
        {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME
        };
        //--------------------------------------------------------------------------

        struct QueueFamilyIndices
        {
            Optional<UInt32> graphicsFamily;
            Optional<UInt32> presentFamily;

            bool IsComplete() const noexcept
            {
                return graphicsFamily.has_value() && presentFamily.has_value();
            }
        };
        //--------------------------------------------------------------------------

        struct SwapChainSupportDetails
        {
            VkSurfaceCapabilitiesKHR capabilities;
            Vector<VkSurfaceFormatKHR> formats;
            Vector<VkPresentModeKHR> presentModes;
        };
        //--------------------------------------------------------------------------

        static QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device, const VkSurfaceKHR& surface)
        {
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
                    indices.graphicsFamily = index;
                }

                VkBool32 presentFamilySupport = false;
                vkGetPhysicalDeviceSurfaceSupportKHR(device, index, surface, &presentFamilySupport);
                if (presentFamilySupport)
                {
                    indices.presentFamily = index;
                }

                if (indices.IsComplete())
                {
                    break;
                }

                index++;
            }

            return indices;
        }
        //--------------------------------------------------------------------------

        static bool CheckDeviceExtensionSupport(VkPhysicalDevice device)
        {
            UInt32 extensionCount;
            vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

            Vector<VkExtensionProperties> availableExtensions(extensionCount);
            vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

            Set<String> requiredExtensions(DeviceExtensions.begin(), DeviceExtensions.end());

            for (const auto& extension : availableExtensions)
            {
                requiredExtensions.erase(extension.extensionName);
            }

            return requiredExtensions.empty();
        }
        //--------------------------------------------------------------------------

        static SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device, const VkSurfaceKHR& surface)
        {
            SwapChainSupportDetails details;
            vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

            UInt32 formatCount = 0;
            vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);
            if (formatCount != 0)
            {
                details.formats.resize(formatCount);
                vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
            }

            UInt32 presentModeCount = 0;
            vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);
            if (presentModeCount != 0)
            {
                details.presentModes.resize(presentModeCount);
                vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
            }

            return details;
        }
        //--------------------------------------------------------------------------

        static bool IsDeviceSuitable(VkPhysicalDevice device, const VkSurfaceKHR& surface)
        {
            const auto indices = FindQueueFamilies(device, surface);
            if (!indices.IsComplete())
            {
                return false;
            }

            const auto extensionsSupported = CheckDeviceExtensionSupport(device);
            if (!extensionsSupported)
            {
                return false;
            }

            const auto swapChainSupportDetails = QuerySwapChainSupport(device, surface);
            if (swapChainSupportDetails.formats.empty() || swapChainSupportDetails.presentModes.empty())
            {
                return false;
            }

            VkPhysicalDeviceFeatures supportedFeatures;
            vkGetPhysicalDeviceFeatures(device, &supportedFeatures);
            if (!supportedFeatures.samplerAnisotropy)
            {
                return false;
            }

            return true;
        }
        //--------------------------------------------------------------------------


        VulkanPhysicalDevice::VulkanPhysicalDevice(const VkInstance& instance, const VkSurfaceKHR& surface)
            : PhysicalDevice()
            , _instance(instance)
            , _surface(surface)
            , _physicalDevice(VK_NULL_HANDLE)
        {
            UInt32 deviceCount = 0;
            vkEnumeratePhysicalDevices(_instance, &deviceCount, nullptr);
            if (deviceCount == 0)
            {
                KMP_LOG_CRITICAL("failed to find GPUs with Vulkan support");
                throw std::runtime_error("VulkanPhysicalDevice: failed to find GPUs with Vulkan support");
            }

            Vector<VkPhysicalDevice> devices(deviceCount);
            vkEnumeratePhysicalDevices(_instance, &deviceCount, devices.data());
            for (const auto& device : devices)
            {
                if (IsDeviceSuitable(device, _surface))
                {
                    _physicalDevice = device;
                    break;
                }
            }

            if (_physicalDevice == VK_NULL_HANDLE)
            {
                KMP_LOG_CRITICAL("failed to find a suitable GPU");
                throw std::runtime_error("VulkanPhysicalDevice: failed to find a suitable GPU");
            }
        }
        //--------------------------------------------------------------------------
    }
}