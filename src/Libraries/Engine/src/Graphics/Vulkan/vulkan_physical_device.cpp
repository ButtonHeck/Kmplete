#include "Kmplete/Graphics/Vulkan/vulkan_physical_device.h"
#include "Kmplete/Graphics/Vulkan/Utils/initializers.h"
#include "Kmplete/Graphics/Vulkan/Utils/function_utils.h"
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

        VulkanPhysicalDevice::VulkanPhysicalDevice(const Window& window, const UInt32& currentBufferIndex, VkInstance instance, VkSurfaceKHR surface)
            : PhysicalDevice()
            , _window(window)
            , _currentBufferIndex(currentBufferIndex)
            , _instance(instance)
            , _surface(surface)
            , _physicalDevice(VK_NULL_HANDLE)
            , _vulkanContext()
            , _memoryTypeDelegate(nullptr)
            , _logicalDevice(nullptr)
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
                auto deviceCheck = VulkanUtils::IsDeviceSuitable(device, _surface, VulkanPhysicalDevice::GetEnabledDeviceExtensions());
                auto deviceIsSuitable = deviceCheck.first;
                if (deviceIsSuitable)
                {
                    _physicalDevice = device;

                    auto& [queueFamilyIndices, surfaceAndPresentModeProperties] = deviceCheck.second;
                    _PopulateVulkanContext(
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

            _memoryTypeDelegate.reset(new VulkanMemoryTypeDelegate(_vulkanContext.memoryProperties));
            _logicalDevice.reset(new VulkanLogicalDevice(_physicalDevice, _surface, _vulkanContext, *_memoryTypeDelegate.get(), _window, _currentBufferIndex));
        }
        //--------------------------------------------------------------------------

        VulkanPhysicalDevice::~VulkanPhysicalDevice()
        {
            _logicalDevice.reset();
            _memoryTypeDelegate.reset();
        }
        //--------------------------------------------------------------------------

        void VulkanPhysicalDevice::StartFrame(float frameTimestep)
        {
            _logicalDevice->StartFrame(frameTimestep);
        }
        //--------------------------------------------------------------------------

        void VulkanPhysicalDevice::EndFrame()
        {
            _logicalDevice->EndFrame();
        }
        //--------------------------------------------------------------------------

        void VulkanPhysicalDevice::HandleWindowResize()
        {
            _UpdateSurfaceInfo();
            _logicalDevice->HandleWindowResize();
        }
        //--------------------------------------------------------------------------

        const LogicalDevice& VulkanPhysicalDevice::GetLogicalDevice() const noexcept
        {
            return *_logicalDevice.get();
        }
        //--------------------------------------------------------------------------

        VkPhysicalDevice VulkanPhysicalDevice::GetVkPhysicalDevice() const noexcept
        {
            return _physicalDevice;
        }
        //--------------------------------------------------------------------------

        const VulkanContext& VulkanPhysicalDevice::GetVulkanContext() const noexcept
        {
            return _vulkanContext;
        }
        //--------------------------------------------------------------------------

        VkSurfaceFormatKHR VulkanPhysicalDevice::_FindSurfaceFormat() const
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

            const auto& surfaceFormats = _vulkanContext.surfaceFormats;
            if (surfaceFormats.empty())
            {
                KMP_LOG_CRITICAL("unable to get available surface format");
                throw std::runtime_error("VulkanPhysicalDevice: unable to get available surface format");
            }

            for (const auto& surfaceFormat : surfaceFormats)
            {
                //TODO: add to settings in a future, use default RBG
                if (surfaceFormat.format == VK_FORMAT_B8G8R8A8_UINT && surfaceFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
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

        void VulkanPhysicalDevice::_PopulateVulkanContext(UInt32 graphicsFamilyIndex, UInt32 presentFamilyIndex, const VkSurfaceCapabilitiesKHR& surfaceCapabilities,
                                                               Vector<VkSurfaceFormatKHR>&& surfaceFormats, Vector<VkPresentModeKHR>&& presentModes)
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

            _vulkanContext.graphicsFamilyIndex = graphicsFamilyIndex;
            _vulkanContext.presentFamilyIndex = presentFamilyIndex;

            _vulkanContext.surfaceCapabilities = surfaceCapabilities;
            _vulkanContext.surfaceFormats = std::move(surfaceFormats);
            _vulkanContext.presentModes = std::move(presentModes);

            vkGetPhysicalDeviceMemoryProperties(_physicalDevice, &_vulkanContext.memoryProperties);
            vkGetPhysicalDeviceProperties(_physicalDevice, &_vulkanContext.deviceProperties);

            const auto& properties = _vulkanContext.deviceProperties;
            _vulkanContext.sampleCountsMask = properties.limits.framebufferColorSampleCounts & properties.limits.framebufferDepthSampleCounts;
            const auto samplesCountMask = _vulkanContext.sampleCountsMask;
            if (samplesCountMask & VK_SAMPLE_COUNT_64_BIT)
                _vulkanContext.supportedSampleCounts.push(VK_SAMPLE_COUNT_64_BIT);
            if (samplesCountMask & VK_SAMPLE_COUNT_32_BIT)
                _vulkanContext.supportedSampleCounts.push(VK_SAMPLE_COUNT_32_BIT);
            if (samplesCountMask & VK_SAMPLE_COUNT_16_BIT)
                _vulkanContext.supportedSampleCounts.push(VK_SAMPLE_COUNT_16_BIT);
            if (samplesCountMask & VK_SAMPLE_COUNT_8_BIT)
                _vulkanContext.supportedSampleCounts.push(VK_SAMPLE_COUNT_8_BIT);
            if (samplesCountMask & VK_SAMPLE_COUNT_4_BIT)
                _vulkanContext.supportedSampleCounts.push(VK_SAMPLE_COUNT_4_BIT);
            if (samplesCountMask & VK_SAMPLE_COUNT_2_BIT)
                _vulkanContext.supportedSampleCounts.push(VK_SAMPLE_COUNT_2_BIT);
            else
                _vulkanContext.supportedSampleCounts.push(VK_SAMPLE_COUNT_1_BIT);

            _vulkanContext.defaultDepthFormat = _FindSupportedFormat(
                { VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT, VK_FORMAT_D16_UNORM_S8_UINT },
                VK_IMAGE_TILING_OPTIMAL, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
            );

            _vulkanContext.surfaceFormat = _FindSurfaceFormat();
        }
        //--------------------------------------------------------------------------

        void VulkanPhysicalDevice::_UpdateSurfaceInfo()
        {
            auto surfaceAndPresentModeProperties = VulkanUtils::QuerySurfaceAndPresentModeProperties(_physicalDevice, _surface);
            _vulkanContext.surfaceCapabilities = surfaceAndPresentModeProperties.surfaceCapabilities;
            _vulkanContext.surfaceFormats = std::move(surfaceAndPresentModeProperties.surfaceFormats);
            _vulkanContext.presentModes = std::move(surfaceAndPresentModeProperties.presentModes);
        }
        //--------------------------------------------------------------------------

        void VulkanPhysicalDevice::_QueryGPUInfo()
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

            auto propertiesVersion12 = VulkanUtils::InitVkPhysicalDeviceVulkan12Properties();
            propertiesVersion12.pNext = nullptr;

            auto propertiesVersion11 = VulkanUtils::InitVkPhysicalDeviceVulkan11Properties();
            propertiesVersion11.pNext = &propertiesVersion12;

            auto properties2 = VulkanUtils::InitVkPhysicalDeviceProperties2();
            properties2.pNext = &propertiesVersion11;
            vkGetPhysicalDeviceProperties2(_physicalDevice, &properties2);

            _gpuInfo.vendor = propertiesVersion12.driverName;
            _gpuInfo.name = properties2.properties.deviceName;
            _gpuInfo.driverVersion = propertiesVersion12.driverInfo;
        }
        //--------------------------------------------------------------------------
    }
}