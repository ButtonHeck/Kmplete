#include "Kmplete/Graphics/Vulkan/vulkan_physical_device.h"
#include "Kmplete/Graphics/Vulkan/vulkan_logical_device.h"
#include "Kmplete/Base/optional.h"
#include "Kmplete/Window/window.h"
#include "Kmplete/Log/log.h"
#include "Kmplete/Profile/profiler.h"

#include <stdexcept>
#include <utility>


namespace Kmplete
{
    namespace Graphics
    {
        namespace
        {
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


            KMP_NODISCARD QueueFamilyIndices QueryQueueFamiliesIndices(const VkPhysicalDevice& device, const VkSurfaceKHR& surface)
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

            KMP_NODISCARD bool QueryDeviceExtensionSupport(const VkPhysicalDevice& device)
            {
                KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

                UInt32 extensionCount;
                vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

                Vector<VkExtensionProperties> availableExtensions(extensionCount);
                vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

                const auto& enabledExtensions = VulkanPhysicalDevice::GetEnabledDeviceExtensions();
                Set<String> requiredExtensions(enabledExtensions.begin(), enabledExtensions.end());

                for (const auto& extension : availableExtensions)
                {
                    requiredExtensions.erase(extension.extensionName);
                }

                return requiredExtensions.empty();
            }
            //--------------------------------------------------------------------------

            KMP_NODISCARD SurfaceAndPresentModeProperties QuerySurfaceAndPresentModeProperties(const VkPhysicalDevice& device, const VkSurfaceKHR& surface)
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

            KMP_NODISCARD std::pair<bool, std::pair<QueueFamilyIndices, SurfaceAndPresentModeProperties>> IsDeviceSuitable(const VkPhysicalDevice& device, const VkSurfaceKHR& surface)
            {
                KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

                const auto queueFamiliesIndices = QueryQueueFamiliesIndices(device, surface);
                if (!queueFamiliesIndices.IsValid())
                {
                    return { false, {} };
                }

                const auto extensionsSupported = QueryDeviceExtensionSupport(device);
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
                auto deviceCheck = IsDeviceSuitable(device, _surface);
                auto deviceIsSuitable = deviceCheck.first;
                if (deviceIsSuitable)
                {
                    _physicalDevice = device;

                    auto& [queueFamilyIndices, surfaceAndPresentModeProperties] = deviceCheck.second;
                    _PopulatePhysicalDeviceInfo(
                        queueFamilyIndices.graphicsFamilyIndex.value(),
                        queueFamilyIndices.presentFamilyIndex.value(), 
                        surfaceAndPresentModeProperties.surfaceCapabilities, 
                        std::move(surfaceAndPresentModeProperties.surfaceFormats),
                        std::move(surfaceAndPresentModeProperties.presentModes)
                    );
                }
            }

            if (_physicalDevice == VK_NULL_HANDLE)
            {
                KMP_LOG_CRITICAL("failed to find a suitable GPU");
                throw std::runtime_error("VulkanPhysicalDevice: failed to find a suitable GPU");
            }

            _QueryGPUInfo();
            PrintGPUInfo();

            _logicalDevice.reset(new VulkanLogicalDevice(_physicalDevice, _surface, _physicalDeviceInfo, _window));
        }
        //--------------------------------------------------------------------------

        VulkanPhysicalDevice::~VulkanPhysicalDevice()
        {
            _logicalDevice.reset();
        }
        //--------------------------------------------------------------------------

        VkPhysicalDevice VulkanPhysicalDevice::GetVkPhysicalDevice() const noexcept
        {
            return _physicalDevice;
        }
        //--------------------------------------------------------------------------

        const PhysicalDeviceInfo& VulkanPhysicalDevice::GetDeviceInfo() const noexcept
        {
            return _physicalDeviceInfo;
        }
        //--------------------------------------------------------------------------

        VkSurfaceFormatKHR VulkanPhysicalDevice::_FindSurfaceFormat() const
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

            const auto& surfaceFormats = _physicalDeviceInfo.surfaceFormats;
            if (surfaceFormats.empty())
            {
                KMP_LOG_CRITICAL("unable to get available surface format");
                throw std::runtime_error("VulkanPhysicalDevice: unable to get available surface format");
            }

            for (const auto& surfaceFormat : surfaceFormats)
            {
                if (surfaceFormat.format == VK_FORMAT_B8G8R8A8_SRGB && surfaceFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
                {
                    return surfaceFormat;
                }
            }

            return surfaceFormats[0];
        }
        //--------------------------------------------------------------------------

        VkFormat VulkanPhysicalDevice::_FindSupportedFormat(const Vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features) const
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

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

        void VulkanPhysicalDevice::_PopulatePhysicalDeviceInfo(UInt32 graphicsFamilyIndex, UInt32 presentFamilyIndex, const VkSurfaceCapabilitiesKHR& surfaceCapabilities,
                                                               Vector<VkSurfaceFormatKHR>&& surfaceFormats, Vector<VkPresentModeKHR>&& presentModes)
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

            _physicalDeviceInfo.graphicsFamilyIndex = graphicsFamilyIndex;
            _physicalDeviceInfo.presentFamilyIndex = presentFamilyIndex;

            _physicalDeviceInfo.surfaceCapabilities = surfaceCapabilities;
            _physicalDeviceInfo.surfaceFormats = std::move(surfaceFormats);
            _physicalDeviceInfo.presentModes = std::move(presentModes);

            vkGetPhysicalDeviceMemoryProperties(_physicalDevice, &_physicalDeviceInfo.memoryProperties);
            vkGetPhysicalDeviceProperties(_physicalDevice, &_physicalDeviceInfo.deviceProperties);

            const auto& properties = _physicalDeviceInfo.deviceProperties;
            _physicalDeviceInfo.sampleCountsMask = properties.limits.framebufferColorSampleCounts & properties.limits.framebufferDepthSampleCounts;
            const auto samplesCountMask = _physicalDeviceInfo.sampleCountsMask;
            if (samplesCountMask & VK_SAMPLE_COUNT_64_BIT)
                _physicalDeviceInfo.supportedSampleCounts.push(VK_SAMPLE_COUNT_64_BIT);
            if (samplesCountMask & VK_SAMPLE_COUNT_32_BIT)
                _physicalDeviceInfo.supportedSampleCounts.push(VK_SAMPLE_COUNT_32_BIT);
            if (samplesCountMask & VK_SAMPLE_COUNT_16_BIT)
                _physicalDeviceInfo.supportedSampleCounts.push(VK_SAMPLE_COUNT_16_BIT);
            if (samplesCountMask & VK_SAMPLE_COUNT_8_BIT)
                _physicalDeviceInfo.supportedSampleCounts.push(VK_SAMPLE_COUNT_8_BIT);
            if (samplesCountMask & VK_SAMPLE_COUNT_4_BIT)
                _physicalDeviceInfo.supportedSampleCounts.push(VK_SAMPLE_COUNT_4_BIT);
            if (samplesCountMask & VK_SAMPLE_COUNT_2_BIT)
                _physicalDeviceInfo.supportedSampleCounts.push(VK_SAMPLE_COUNT_2_BIT);
            else
                _physicalDeviceInfo.supportedSampleCounts.push(VK_SAMPLE_COUNT_1_BIT);

            _physicalDeviceInfo.defaultDepthFormat = _FindSupportedFormat(
                { VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
                VK_IMAGE_TILING_OPTIMAL, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
            );

            _physicalDeviceInfo.surfaceFormat = _FindSurfaceFormat();
        }
        //--------------------------------------------------------------------------

        void VulkanPhysicalDevice::_QueryGPUInfo()
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

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

            _gpuInfo.vendor = propertiesVersion12.driverName;
            _gpuInfo.name = properties2.properties.deviceName;
            _gpuInfo.driverVersion = propertiesVersion12.driverInfo;
        }
        //--------------------------------------------------------------------------
    }
}