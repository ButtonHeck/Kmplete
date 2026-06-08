#include "Kmplete/Graphics/Vulkan/Core/vulkan_physical_device.h"
#include "Kmplete/Graphics/Vulkan/Utils/initializers.h"
#include "Kmplete/Graphics/Vulkan/Utils/bits_aliases.h"
#include "Kmplete/Base/optional.h"
#include "Kmplete/Base/exception.h"
#include "Kmplete/Base/named_bool.h"
#include "Kmplete/Core/assertion.h"
#include "Kmplete/Window/window.h"
#include "Kmplete/Log/log.h"
#include "Kmplete/Profile/profiler.h"


namespace Kmplete
{
    namespace Graphics
    {
        using namespace VKBits;


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
                        KMP_LOG_WARN_FN("VulkanPhysicalDevice::QueryDeviceExtensionSupport: for device '{}' required extension {} not found", properties2.properties.deviceName, extension);
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
                    KMP_LOG_WARN_FN("VulkanPhysicalDevice::IsDeviceSuitable: '{}' is not suitable - queue families indices are invalid", properties2.properties.deviceName);
                    return { "device suitable"_false, {} };
                }

                const auto extensionsSupported = QueryDeviceExtensionSupport(device, enabledExtensions);
                if (!extensionsSupported)
                {
                    KMP_LOG_WARN_FN("VulkanPhysicalDevice::IsDeviceSuitable: '{}' is not suitable - required extensions are not supported", properties2.properties.deviceName);
                    return { "device suitable"_false, {} };
                }

                const auto surfaceAndPresentModeProperties = QuerySurfaceAndPresentModeProperties(device, surface);
                if (!surfaceAndPresentModeProperties.IsValid())
                {
                    KMP_LOG_WARN_FN("VulkanPhysicalDevice::IsDeviceSuitable: '{}' is not suitable - surface and present modes properties are invalid", properties2.properties.deviceName);
                    return { "device suitable"_false, {} };
                }

                VkPhysicalDeviceFeatures supportedFeatures;
                vkGetPhysicalDeviceFeatures(device, &supportedFeatures);
                if (!supportedFeatures.samplerAnisotropy ||
                    !supportedFeatures.independentBlend ||
                    !supportedFeatures.depthBounds)
                {
                    KMP_LOG_WARN_FN("VulkanPhysicalDevice::IsDeviceSuitable: '{}' is not suitable - some device features are not supported", properties2.properties.deviceName);
                    return { "device suitable"_false, {} };
                }

                return { "device suitable"_true, { queueFamiliesIndices, surfaceAndPresentModeProperties } };
            }}
            //--------------------------------------------------------------------------
        }


        const Vector<const char*>& VulkanPhysicalDevice::GetEnabledDeviceExtensions()
        {
            static const Vector<const char*> deviceExtensions =
            {
                VK_KHR_SWAPCHAIN_EXTENSION_NAME,
                VK_KHR_DYNAMIC_RENDERING_EXTENSION_NAME,
                VK_EXT_EXTENDED_DYNAMIC_STATE_2_EXTENSION_NAME,
                VK_EXT_EXTENDED_DYNAMIC_STATE_3_EXTENSION_NAME,
                VK_KHR_DEPTH_STENCIL_RESOLVE_EXTENSION_NAME,
                VK_EXT_DEPTH_CLIP_ENABLE_EXTENSION_NAME,
                VK_KHR_SYNCHRONIZATION_2_EXTENSION_NAME,
                VK_EXT_LINE_RASTERIZATION_EXTENSION_NAME,
                VK_EXT_SAMPLE_LOCATIONS_EXTENSION_NAME,
                VK_KHR_FRAGMENT_SHADING_RATE_EXTENSION_NAME,
                VK_EXT_COLOR_WRITE_ENABLE_EXTENSION_NAME,
                VK_EXT_VERTEX_INPUT_DYNAMIC_STATE_EXTENSION_NAME,
                VK_EXT_SHADER_OBJECT_EXTENSION_NAME,
                VK_KHR_PIPELINE_LIBRARY_EXTENSION_NAME,
                VK_EXT_GRAPHICS_PIPELINE_LIBRARY_EXTENSION_NAME
            };

            return deviceExtensions;
        }
        //--------------------------------------------------------------------------

        VulkanPhysicalDevice::VulkanPhysicalDevice(const Window& window, const UInt32& currentBufferIndex, VkInstance instance, VkSurfaceKHR surface)
            : PhysicalDevice()
            , _window(window)
            , _currentBufferIndex(currentBufferIndex)
            , _instance(instance)
            , _surface(surface)
            , _physicalDevice(VK_NULL_HANDLE)
            , _formatDelegate(nullptr)
            , _vulkanContext()
            , _memoryTypeDelegate(nullptr)
            , _logicalDevice(nullptr)
        {
            KMP_PROFILE_FUNCTION(ProfileLevelAlways);

            KMP_ASSERT(_instance && _surface);

            Vector<VkPhysicalDevice> devices = _GetListOfPhysicalDevices();
            _PickSuitablePhysicalDevice(devices);

            if (_physicalDevice == VK_NULL_HANDLE)
            {
                KMP_LOG_CRITICAL("failed to find a suitable GPU");
                throw RuntimeError("VulkanPhysicalDevice: failed to find a suitable GPU");
            }

            _QueryGPUInfo();
            PrintGPUInfo();

            _memoryTypeDelegate.reset(new VulkanMemoryTypeDelegate(_vulkanContext.memoryProperties));
            KMP_ASSERT(_memoryTypeDelegate);

            _logicalDevice.reset(new VulkanLogicalDevice(_physicalDevice, _surface, _vulkanContext, *_memoryTypeDelegate.get(), *_formatDelegate.get(), _window, _currentBufferIndex));
            KMP_ASSERT(_logicalDevice);
        }
        //--------------------------------------------------------------------------

        VulkanPhysicalDevice::~VulkanPhysicalDevice() KMP_PROFILING(ProfileLevelAlways)
        {
            KMP_ASSERT(_logicalDevice && _memoryTypeDelegate && _formatDelegate);

            _logicalDevice.reset();
            _memoryTypeDelegate.reset();
            _formatDelegate.reset();
        }}
        //--------------------------------------------------------------------------

        void VulkanPhysicalDevice::StartFrame(float frameTimestep) KMP_PROFILING(ProfileLevelImportant)
        {
            KMP_ASSERT(_logicalDevice);

            _logicalDevice->StartFrame(frameTimestep);
        }}
        //--------------------------------------------------------------------------

        void VulkanPhysicalDevice::EndFrame() KMP_PROFILING(ProfileLevelImportant)
        {
            KMP_ASSERT(_logicalDevice);

            _logicalDevice->EndFrame();
        }}
        //--------------------------------------------------------------------------

        void VulkanPhysicalDevice::HandleWindowResize() KMP_PROFILING(ProfileLevelMinor)
        {
            KMP_ASSERT(_logicalDevice);

            _UpdateSurfaceInfo();
            _logicalDevice->HandleWindowResize();
        }}
        //--------------------------------------------------------------------------

        const VulkanLogicalDevice& VulkanPhysicalDevice::GetLogicalDevice() const noexcept
        {
            KMP_ASSERT(_logicalDevice);

            return *_logicalDevice.get();
        }
        //--------------------------------------------------------------------------

        VulkanLogicalDevice& VulkanPhysicalDevice::GetLogicalDevice() noexcept
        {
            KMP_ASSERT(_logicalDevice);

            return *_logicalDevice.get();
        }
        //--------------------------------------------------------------------------

        VkPhysicalDevice VulkanPhysicalDevice::GetVkPhysicalDevice() const noexcept
        {
            KMP_ASSERT(_physicalDevice);

            return _physicalDevice;
        }
        //--------------------------------------------------------------------------

        const VulkanContext& VulkanPhysicalDevice::GetVulkanContext() const noexcept
        {
            return _vulkanContext;
        }
        //--------------------------------------------------------------------------

        const VulkanFormatDelegate& VulkanPhysicalDevice::GetVulkanFormatDelegate() const noexcept
        {
            KMP_ASSERT(_formatDelegate);

            return *_formatDelegate.get();
        }
        //--------------------------------------------------------------------------

        const VulkanMemoryTypeDelegate& VulkanPhysicalDevice::GetVulkanMemoryTypeDelegate() const noexcept
        {
            KMP_ASSERT(_memoryTypeDelegate);

            return *_memoryTypeDelegate.get();
        }
        //--------------------------------------------------------------------------

        Vector<VkPhysicalDevice> VulkanPhysicalDevice::_GetListOfPhysicalDevices() const KMP_PROFILING(ProfileLevelImportant)
        {
            UInt32 deviceCount = 0;
            vkEnumeratePhysicalDevices(_instance, &deviceCount, nullptr);
            if (deviceCount == 0)
            {
                KMP_LOG_CRITICAL("failed to find GPUs with Vulkan support");
                throw RuntimeError("VulkanPhysicalDevice: failed to find GPUs with Vulkan support");
            }

            Vector<VkPhysicalDevice> devices(deviceCount);
            vkEnumeratePhysicalDevices(_instance, &deviceCount, devices.data());

            return devices;
        }}
        //--------------------------------------------------------------------------

        void VulkanPhysicalDevice::_PickSuitablePhysicalDevice(const Vector<VkPhysicalDevice>& physicalDevices) KMP_PROFILING(ProfileLevelImportant)
        {
            for (const auto& device : physicalDevices)
            {
                auto deviceCheck = IsDeviceSuitable(device, _surface, VulkanPhysicalDevice::GetEnabledDeviceExtensions());
                auto deviceIsSuitable = deviceCheck.first;
                if (deviceIsSuitable)
                {
                    _physicalDevice = device;

                    _formatDelegate.reset(new VulkanFormatDelegate(_physicalDevice));
                    KMP_ASSERT(_formatDelegate);

                    auto defaultDepthFormat = _formatDelegate->FindImageFormat(
                        {
                          VK_Format_D32_SFloat_S8_UInt,
                          VK_Format_D24_UNorm_S8_UInt,
                          VK_Format_D16_UNorm_S8_UInt
                        },
                        VK_ImageTiling_Optimal,
                        VK_FormatFeature_DepthStencilAttachment
                    );

                    auto& [queueFamilyIndices, surfaceAndPresentModeProperties] = deviceCheck.second;
                    _vulkanContext.Populate(
                        _instance,
                        _physicalDevice,
                        _surface,
                        defaultDepthFormat,
                        queueFamilyIndices.graphicsFamilyIndex.value(),
                        queueFamilyIndices.presentFamilyIndex.value(),
                        surfaceAndPresentModeProperties.surfaceCapabilities,
                        std::move(surfaceAndPresentModeProperties.surfaceFormats),
                        std::move(surfaceAndPresentModeProperties.presentModes)
                    );
                }
            }
        }}
        //--------------------------------------------------------------------------

        void VulkanPhysicalDevice::_UpdateSurfaceInfo() KMP_PROFILING(ProfileLevelMinorVerbose)
        {
            KMP_ASSERT(_physicalDevice && _surface);

            auto surfaceAndPresentModeProperties = QuerySurfaceAndPresentModeProperties(_physicalDevice, _surface);
            _vulkanContext.surfaceCapabilities = surfaceAndPresentModeProperties.surfaceCapabilities;
            _vulkanContext.surfaceFormats = std::move(surfaceAndPresentModeProperties.surfaceFormats);
            _vulkanContext.presentModes = std::move(surfaceAndPresentModeProperties.presentModes);
        }}
        //--------------------------------------------------------------------------

        void VulkanPhysicalDevice::_QueryGPUInfo() KMP_PROFILING(ProfileLevelImportant)
        {
            KMP_ASSERT(_physicalDevice);

            auto propertiesVersion12 = VKUtils::InitVkPhysicalDeviceVulkan12Properties();
            propertiesVersion12.pNext = nullptr;

            auto propertiesVersion11 = VKUtils::InitVkPhysicalDeviceVulkan11Properties();
            propertiesVersion11.pNext = &propertiesVersion12;

            auto properties2 = VKUtils::InitVkPhysicalDeviceProperties2();
            properties2.pNext = &propertiesVersion11;
            vkGetPhysicalDeviceProperties2(_physicalDevice, &properties2);

            _gpuInfo.vendor = propertiesVersion12.driverName;
            _gpuInfo.name = properties2.properties.deviceName;
            _gpuInfo.driverVersion = propertiesVersion12.driverInfo;
        }}
        //--------------------------------------------------------------------------
    }
}