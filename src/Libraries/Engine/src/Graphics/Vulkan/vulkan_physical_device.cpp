#include "Kmplete/Graphics/Vulkan/vulkan_physical_device.h"
#include "Kmplete/Graphics/Vulkan/Utils/initializers.h"
#include "Kmplete/Graphics/Vulkan/Utils/function_utils.h"
#include "Kmplete/Graphics/Vulkan/Utils/bits_aliases.h"
#include "Kmplete/Base/optional.h"
#include "Kmplete/Base/exception.h"
#include "Kmplete/Core/assertion.h"
#include "Kmplete/Window/window.h"
#include "Kmplete/Log/log.h"
#include "Kmplete/Profile/profiler.h"


namespace Kmplete
{
    namespace Graphics
    {
        using namespace VKBits;


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
                VK_EXT_DEPTH_CLAMP_CONTROL_EXTENSION_NAME,
                VK_KHR_SYNCHRONIZATION_2_EXTENSION_NAME,
                VK_EXT_LINE_RASTERIZATION_EXTENSION_NAME,
                VK_EXT_DISCARD_RECTANGLES_EXTENSION_NAME,
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
                auto deviceCheck = VKUtils::IsDeviceSuitable(device, _surface, VulkanPhysicalDevice::GetEnabledDeviceExtensions());
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

            auto surfaceAndPresentModeProperties = VKUtils::QuerySurfaceAndPresentModeProperties(_physicalDevice, _surface);
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