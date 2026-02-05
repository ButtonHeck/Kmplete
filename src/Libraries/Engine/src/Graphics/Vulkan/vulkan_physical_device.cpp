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
                VK_KHR_SWAPCHAIN_EXTENSION_NAME,
                VK_KHR_DYNAMIC_RENDERING_EXTENSION_NAME
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
                const auto deviceCheck = _IsDeviceSuitable(device);
                const auto deviceIsSuitable = deviceCheck.first;
                const auto& [queueFamilyIndices, surfaceAndPresentModeProperties] = deviceCheck.second;
                if (deviceIsSuitable)
                {
                    _physicalDevice = device;

                    _physicalDeviceImplementationInfo.graphicsFamilyIndex = queueFamilyIndices.graphicsFamilyIndex.value();
                    _physicalDeviceImplementationInfo.presentFamilyIndex = queueFamilyIndices.presentFamilyIndex.value();

                    _physicalDeviceImplementationInfo.surfaceCapabilities = surfaceAndPresentModeProperties.surfaceCapabilities;
                    _physicalDeviceImplementationInfo.surfaceFormats = surfaceAndPresentModeProperties.surfaceFormats;
                    _physicalDeviceImplementationInfo.presentModes = surfaceAndPresentModeProperties.presentModes;

                    vkGetPhysicalDeviceMemoryProperties(_physicalDevice, &_physicalDeviceImplementationInfo.memoryProperties);
                    vkGetPhysicalDeviceProperties(_physicalDevice, &_physicalDeviceImplementationInfo.deviceProperties);

                    const auto& properties = _physicalDeviceImplementationInfo.deviceProperties;
                    _physicalDeviceImplementationInfo.sampleCountsMask = properties.limits.framebufferColorSampleCounts & properties.limits.framebufferDepthSampleCounts;
                    const auto samplesCountMask = _physicalDeviceImplementationInfo.sampleCountsMask;
                    if (samplesCountMask & VK_SAMPLE_COUNT_64_BIT)
                        _physicalDeviceImplementationInfo.supportedSampleCounts.push(VK_SAMPLE_COUNT_64_BIT);
                    if (samplesCountMask & VK_SAMPLE_COUNT_32_BIT)
                        _physicalDeviceImplementationInfo.supportedSampleCounts.push(VK_SAMPLE_COUNT_32_BIT);
                    if (samplesCountMask & VK_SAMPLE_COUNT_16_BIT)
                        _physicalDeviceImplementationInfo.supportedSampleCounts.push(VK_SAMPLE_COUNT_16_BIT);
                    if (samplesCountMask & VK_SAMPLE_COUNT_8_BIT)
                        _physicalDeviceImplementationInfo.supportedSampleCounts.push(VK_SAMPLE_COUNT_8_BIT);
                    if (samplesCountMask & VK_SAMPLE_COUNT_4_BIT)
                        _physicalDeviceImplementationInfo.supportedSampleCounts.push(VK_SAMPLE_COUNT_4_BIT);
                    if (samplesCountMask & VK_SAMPLE_COUNT_2_BIT)
                        _physicalDeviceImplementationInfo.supportedSampleCounts.push(VK_SAMPLE_COUNT_2_BIT);
                    else
                        _physicalDeviceImplementationInfo.supportedSampleCounts.push(VK_SAMPLE_COUNT_1_BIT);

                    _physicalDeviceImplementationInfo.defaultDepthFormat = _FindSupportedFormat(
                        { VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
                        VK_IMAGE_TILING_OPTIMAL, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
                    );
                }
            }

            if (_physicalDevice == VK_NULL_HANDLE)
            {
                KMP_LOG_CRITICAL("failed to find a suitable GPU");
                throw std::runtime_error("VulkanPhysicalDevice: failed to find a suitable GPU");
            }

            _QueryInfo();
            PrintInfo();

            _logicalDevice.reset(new VulkanLogicalDevice(_physicalDevice, _surface, _physicalDeviceImplementationInfo, _window));
        }
        //--------------------------------------------------------------------------

        VulkanPhysicalDevice::~VulkanPhysicalDevice()
        {
            _logicalDevice.reset();
        }
        //--------------------------------------------------------------------------

        const PhysicalDeviceImplementationInfo& VulkanPhysicalDevice::GetImplementationInfo() const noexcept
        {
            return _physicalDeviceImplementationInfo;
        }
        //--------------------------------------------------------------------------

        VulkanPhysicalDevice::QueueFamilyIndices VulkanPhysicalDevice::_QueryQueueFamiliesIndices(VkPhysicalDevice device) const
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

        std::pair<bool, std::pair<VulkanPhysicalDevice::QueueFamilyIndices, VulkanPhysicalDevice::SurfaceAndPresentModeProperties>> VulkanPhysicalDevice::_IsDeviceSuitable(VkPhysicalDevice device) const
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

            const auto queueFamiliesIndices = _QueryQueueFamiliesIndices(device);
            if (!queueFamiliesIndices.IsValid())
            {
                return { false, {} };
            }

            const auto extensionsSupported = _QueryDeviceExtensionSupport(device);
            if (!extensionsSupported)
            {
                return { false, {} };
            }

            const auto surfaceAndPresentModeProperties = _QuerySurfaceAndPresentModeProperties(device);
            if (surfaceAndPresentModeProperties.surfaceFormats.empty() || surfaceAndPresentModeProperties.presentModes.empty())
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

        bool VulkanPhysicalDevice::_QueryDeviceExtensionSupport(VkPhysicalDevice device) const
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

        VulkanPhysicalDevice::SurfaceAndPresentModeProperties VulkanPhysicalDevice::_QuerySurfaceAndPresentModeProperties(VkPhysicalDevice device) const
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

            SurfaceAndPresentModeProperties properties;
            vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, _surface, &properties.surfaceCapabilities);

            UInt32 formatCount = 0;
            vkGetPhysicalDeviceSurfaceFormatsKHR(device, _surface, &formatCount, nullptr);
            if (formatCount != 0)
            {
                properties.surfaceFormats.resize(formatCount);
                vkGetPhysicalDeviceSurfaceFormatsKHR(device, _surface, &formatCount, properties.surfaceFormats.data());
            }

            UInt32 presentModeCount = 0;
            vkGetPhysicalDeviceSurfacePresentModesKHR(device, _surface, &presentModeCount, nullptr);
            if (presentModeCount != 0)
            {
                properties.presentModes.resize(presentModeCount);
                vkGetPhysicalDeviceSurfacePresentModesKHR(device, _surface, &presentModeCount, properties.presentModes.data());
            }

            return properties;
        }
        //--------------------------------------------------------------------------

        VkFormat VulkanPhysicalDevice::_FindSupportedFormat(const Vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features) const
        {
            for (auto format : candidates)
            {
                VkFormatProperties props;
                vkGetPhysicalDeviceFormatProperties(_physicalDevice, format, &props);

                if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features)
                {
                    return format;
                }
                else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features)
                {
                    return format;
                }
            }

            KMP_LOG_CRITICAL("failed to find supported format");
            throw std::runtime_error("VulkanPhysicalDevice: failed to find supported format");
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
        }
        //--------------------------------------------------------------------------
    }
}