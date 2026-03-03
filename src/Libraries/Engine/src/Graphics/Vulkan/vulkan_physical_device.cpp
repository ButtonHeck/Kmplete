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
            , _formatDelegate(nullptr)
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

                    _formatDelegate.reset(new VulkanFormatDelegate(_physicalDevice));

                    auto& [queueFamilyIndices, surfaceAndPresentModeProperties] = deviceCheck.second;
                    _vulkanContext.Populate(
                        _physicalDevice, 
                        *_formatDelegate.get(),
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
            _logicalDevice.reset(new VulkanLogicalDevice(_physicalDevice, _surface, _vulkanContext, *_memoryTypeDelegate.get(), *_formatDelegate.get(), _window, _currentBufferIndex));
        }
        //--------------------------------------------------------------------------

        VulkanPhysicalDevice::~VulkanPhysicalDevice()
        {
            KMP_PROFILE_FUNCTION(ProfileLevelAlways);

            _logicalDevice.reset();
            _memoryTypeDelegate.reset();
            _formatDelegate.reset();
        }
        //--------------------------------------------------------------------------

        void VulkanPhysicalDevice::StartFrame(float frameTimestep)
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

            _logicalDevice->StartFrame(frameTimestep);
        }
        //--------------------------------------------------------------------------

        void VulkanPhysicalDevice::EndFrame()
        {
            KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

            _logicalDevice->EndFrame();
        }
        //--------------------------------------------------------------------------

        void VulkanPhysicalDevice::HandleWindowResize()
        {
            KMP_PROFILE_FUNCTION(ProfileLevelMinorFunctions);

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

        void VulkanPhysicalDevice::_UpdateSurfaceInfo()
        {
            KMP_PROFILE_FUNCTION(ProfileLevelMinorFunctionsVerbose);

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