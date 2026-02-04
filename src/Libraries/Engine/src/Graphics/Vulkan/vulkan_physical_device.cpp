#include "Kmplete/Graphics/Vulkan/vulkan_physical_device.h"
#include "Kmplete/Graphics/Vulkan/vulkan_logical_device.h"
#include "Kmplete/Window/window.h"
#include "Kmplete/Log/log.h"
#include "Kmplete/Profile/profiler.h"

#include <stdexcept>


namespace Kmplete
{
    namespace Graphics
    {
        const Vector<const char*>& VulkanPhysicalDevice::GetEnabledDeviceExtensions()
        {
            static const Vector<const char*> deviceExtensions =
            {
                VK_KHR_SWAPCHAIN_EXTENSION_NAME
            };

            return deviceExtensions;
        }
        //--------------------------------------------------------------------------

        VulkanPhysicalDevice::VulkanPhysicalDevice(const VkInstance& instance, const VkSurfaceKHR& surface, const Window& window)
            : PhysicalDevice()
            , _instance(instance)
            , _surface(surface)
            , _window(window)
            , _physicalDevice(VK_NULL_HANDLE)
        {
            KMP_PROFILE_FUNCTION(ProfileLevelAlways);

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
                const auto [deviceIsSuitable, deviceProperties] = _IsDeviceSuitable(device);
                if (deviceIsSuitable)
                {
                    _physicalDevice = device;
                    _properties = deviceProperties;

                    VkPhysicalDeviceMemoryProperties memoryProperties;
                    vkGetPhysicalDeviceMemoryProperties(_physicalDevice, &memoryProperties);
                    _properties.hardwareProperties.memoryProperties = memoryProperties;

                    VkPhysicalDeviceProperties physicalDeviceProperties;
                    vkGetPhysicalDeviceProperties(_physicalDevice, &physicalDeviceProperties);
                    _properties.hardwareProperties.deviceProperties = physicalDeviceProperties;

                    break;
                }
            }

            if (_physicalDevice == VK_NULL_HANDLE)
            {
                KMP_LOG_CRITICAL("failed to find a suitable GPU");
                throw std::runtime_error("VulkanPhysicalDevice: failed to find a suitable GPU");
            }

            _QueryInfo();
            PrintInfo();

            _logicalDevice.reset(new VulkanLogicalDevice(_physicalDevice, _surface, _properties, _window));
        }
        //--------------------------------------------------------------------------

        VulkanPhysicalDevice::~VulkanPhysicalDevice()
        {
            _logicalDevice.reset();
        }
        //--------------------------------------------------------------------------

        const PhysicalDeviceProperties& VulkanPhysicalDevice::GetProperties() const noexcept
        {
            return _properties;
        }
        //--------------------------------------------------------------------------

        QueueFamilyIndices VulkanPhysicalDevice::_FindQueueFamiliesIndices(VkPhysicalDevice device) const
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
                vkGetPhysicalDeviceSurfaceSupportKHR(device, index, _surface, &presentFamilySupport);
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

        std::pair<bool, PhysicalDeviceProperties> VulkanPhysicalDevice::_IsDeviceSuitable(VkPhysicalDevice device) const
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

            const auto queueFamiliesIndices = _FindQueueFamiliesIndices(device);
            if (!queueFamiliesIndices.IsValid())
            {
                return { false, { QueueFamilyIndices(), SwapChainSupportDetails(), HardwareProperties() }};
            }

            const auto extensionsSupported = _CheckDeviceExtensionSupport(device);
            if (!extensionsSupported)
            {
                return { false, { QueueFamilyIndices(), SwapChainSupportDetails(), HardwareProperties() } };
            }

            const auto swapChainSupportDetails = _QuerySwapChainSupport(device);
            if (swapChainSupportDetails.surfaceFormats.empty() || swapChainSupportDetails.presentModes.empty())
            {
                return { false, { QueueFamilyIndices(), SwapChainSupportDetails(), HardwareProperties() } };
            }

            VkPhysicalDeviceFeatures supportedFeatures;
            vkGetPhysicalDeviceFeatures(device, &supportedFeatures);
            if (!supportedFeatures.samplerAnisotropy)
            {
                return { false, { QueueFamilyIndices(), SwapChainSupportDetails(), HardwareProperties() } };
            }

            return { true, { queueFamiliesIndices, swapChainSupportDetails, HardwareProperties() } };
        }
        //--------------------------------------------------------------------------

        bool VulkanPhysicalDevice::_CheckDeviceExtensionSupport(VkPhysicalDevice device) const
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

            UInt32 extensionCount;
            vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

            Vector<VkExtensionProperties> availableExtensions(extensionCount);
            vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

            const auto& enabledExtensions = GetEnabledDeviceExtensions();
            Set<String> requiredExtensions(enabledExtensions.begin(), enabledExtensions.end());

            for (const auto& extension : availableExtensions)
            {
                requiredExtensions.erase(extension.extensionName);
            }

            return requiredExtensions.empty();
        }
        //--------------------------------------------------------------------------

        SwapChainSupportDetails VulkanPhysicalDevice::_QuerySwapChainSupport(VkPhysicalDevice device) const
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

            SwapChainSupportDetails details;
            vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, _surface, &details.surfaceCapabilities);

            UInt32 formatCount = 0;
            vkGetPhysicalDeviceSurfaceFormatsKHR(device, _surface, &formatCount, nullptr);
            if (formatCount != 0)
            {
                details.surfaceFormats.resize(formatCount);
                vkGetPhysicalDeviceSurfaceFormatsKHR(device, _surface, &formatCount, details.surfaceFormats.data());
            }

            UInt32 presentModeCount = 0;
            vkGetPhysicalDeviceSurfacePresentModesKHR(device, _surface, &presentModeCount, nullptr);
            if (presentModeCount != 0)
            {
                details.presentModes.resize(presentModeCount);
                vkGetPhysicalDeviceSurfacePresentModesKHR(device, _surface, &presentModeCount, details.presentModes.data());
            }

            return details;
        }
        //--------------------------------------------------------------------------

        void VulkanPhysicalDevice::_QueryInfo()
        {
            VkPhysicalDeviceVulkan12Properties propertiesVersion12{};
            propertiesVersion12.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_PROPERTIES;
            propertiesVersion12.pNext = nullptr;

            VkPhysicalDeviceVulkan11Properties propertiesVersion11{};
            propertiesVersion11.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_1_PROPERTIES;
            propertiesVersion11.pNext = &propertiesVersion12;

            VkPhysicalDeviceProperties2 properties2{};
            properties2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2;
            properties2.pNext = &propertiesVersion11;
            vkGetPhysicalDeviceProperties2(_physicalDevice, &properties2);

            _info.vendor = propertiesVersion12.driverName;
            _info.name = properties2.properties.deviceName;
            _info.driverVersion = propertiesVersion12.driverInfo;

            const auto& properties = _properties.hardwareProperties.deviceProperties;
            VkSampleCountFlags sampleCounts = properties.limits.framebufferColorSampleCounts & properties.limits.framebufferDepthSampleCounts;
            if (sampleCounts & VK_SAMPLE_COUNT_64_BIT)
                _info.msaaSamples = 64;
            else if (sampleCounts & VK_SAMPLE_COUNT_32_BIT)
                _info.msaaSamples = 32;
            else if (sampleCounts & VK_SAMPLE_COUNT_16_BIT)
                _info.msaaSamples = 16;
            else if (sampleCounts & VK_SAMPLE_COUNT_8_BIT)
                _info.msaaSamples = 8;
            else if (sampleCounts & VK_SAMPLE_COUNT_4_BIT)
                _info.msaaSamples = 4;
            else if (sampleCounts & VK_SAMPLE_COUNT_2_BIT)
                _info.msaaSamples = 2;
            else
                _info.msaaSamples = 1;
        }
        //--------------------------------------------------------------------------
    }
}