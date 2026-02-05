#include "Kmplete/Graphics/Vulkan/vulkan_logical_device.h"
#include "Kmplete/Graphics/Vulkan/vulkan_physical_device.h"
#include "Kmplete/Graphics/Vulkan/vulkan_swapchain.h"
#include "Kmplete/Window/window.h"
#include "Kmplete/Math/math.h"
#include "Kmplete/Math/geometry.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Profile/profiler.h"
#include "Kmplete/Log/log.h"

#if defined (KMP_WINDOW_BACKEND_GLFW)
    #include <GLFW/glfw3.h>
#endif

#include <limits>
#include <stdexcept>


namespace Kmplete
{
    namespace Graphics
    {
        VulkanLogicalDevice::VulkanLogicalDevice(const VkPhysicalDevice& physicalDevice, const VkSurfaceKHR& surface, const PhysicalDeviceImplementationInfo& info, const Window& window)
            : LogicalDevice()
            , _physicalDevice(physicalDevice)
            , _surface(surface)
            , _physicalDeviceImplementationInfo(info)
            , _window(window)
            , _device(nullptr)
            , _graphicsQueue(nullptr)
            , _presentQueue(nullptr)
            , _currentExtent()
        {
            KMP_PROFILE_FUNCTION(ProfileLevelAlways);

            const auto& queueFamiliesIndices = _physicalDeviceImplementationInfo.queueFamiliesIndices;

            Vector<VkDeviceQueueCreateInfo> queueCreateInfos;
            Set<UInt32> queueFamiliesIndicesSet = { queueFamiliesIndices.graphicsFamilyIndex.value(), queueFamiliesIndices.presentFamilyIndex.value() };
            const auto queuePriority = 1.0f;
            for (auto queueFamilyIndex : queueFamiliesIndicesSet)
            {
                VkDeviceQueueCreateInfo queueCreateInfo{};
                queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
                queueCreateInfo.queueFamilyIndex = queueFamilyIndex;
                queueCreateInfo.queueCount = 1;
                queueCreateInfo.pQueuePriorities = &queuePriority;
                queueCreateInfos.push_back(queueCreateInfo);
            }

            VkPhysicalDeviceFeatures deviceFeatures{};
            deviceFeatures.samplerAnisotropy = VK_TRUE;

            const auto& enabledDeviceExtensions = VulkanPhysicalDevice::GetEnabledDeviceExtensions();

            VkDeviceCreateInfo createInfo{};
            createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
            createInfo.queueCreateInfoCount = UInt32(queueCreateInfos.size());
            createInfo.pQueueCreateInfos = queueCreateInfos.data();
            createInfo.pEnabledFeatures = &deviceFeatures;
            createInfo.enabledExtensionCount = UInt32(enabledDeviceExtensions.size());
            createInfo.ppEnabledExtensionNames = enabledDeviceExtensions.data();
            createInfo.pNext = nullptr;

            if (vkCreateDevice(_physicalDevice, &createInfo, nullptr, &_device) != VK_SUCCESS)
            {
                KMP_LOG_CRITICAL("failed to create logical device");
                throw std::runtime_error("VulkanLogicalDevice: failed to create logical device");
            }

            vkGetDeviceQueue(_device, queueFamiliesIndices.graphicsFamilyIndex.value(), 0, &_graphicsQueue);
            vkGetDeviceQueue(_device, queueFamiliesIndices.presentFamilyIndex.value(), 0, &_presentQueue);

            if (_graphicsQueue == nullptr)
            {
                KMP_LOG_CRITICAL("failed to get graphics queue from logical device");
                throw std::runtime_error("VulkanLogicalDevice: failed to get graphics queue from logical device");
            }

            if (_presentQueue == nullptr)
            {
                KMP_LOG_CRITICAL("failed to get present queue from logical device");
                throw std::runtime_error("VulkanLogicalDevice: failed to get present queue from logical device");
            }

            CreateSwapchain();
        }
        //--------------------------------------------------------------------------

        VulkanLogicalDevice::~VulkanLogicalDevice()
        {
            DeleteSwapchain();
            vkDestroyDevice(_device, nullptr);
        }
        //--------------------------------------------------------------------------

        void VulkanLogicalDevice::CreateSwapchain()
        {
            _currentExtent = _ChooseExtent();
            const auto surfaceFormat = _ChooseSurfaceFormat();
            const auto depthFormat = _FindDepthFormat();
            _swapchain.reset(new VulkanSwapchain(_device, _surface, _physicalDeviceImplementationInfo, _currentExtent, surfaceFormat, depthFormat));
        }
        //--------------------------------------------------------------------------

        void VulkanLogicalDevice::DeleteSwapchain()
        {
            _swapchain.reset();
        }
        //--------------------------------------------------------------------------

        void VulkanLogicalDevice::RecreateSwapchain()
        {
            while (_window.IsIconified())
            {
                glfwWaitEvents();
            }

            vkDeviceWaitIdle(_device);

            DeleteSwapchain();
            CreateSwapchain();
        }
        //--------------------------------------------------------------------------

        const VkDevice& VulkanLogicalDevice::GetImplDevice() const noexcept
        {
            return _device;
        }
        //--------------------------------------------------------------------------

        const VkQueue& VulkanLogicalDevice::GetGraphicsQueue() const noexcept
        {
            return _graphicsQueue;
        }
        //--------------------------------------------------------------------------

        const VkQueue& VulkanLogicalDevice::GetPresentQueue() const noexcept
        {
            return _presentQueue;
        }
        //--------------------------------------------------------------------------

        VkExtent2D VulkanLogicalDevice::_ChooseExtent() const
        {
            const auto& capabilities = _physicalDeviceImplementationInfo.surfaceAndPresentModeProperties.surfaceCapabilities;
            if (capabilities.currentExtent.width != std::numeric_limits<UInt32>::max())
            {
                return capabilities.currentExtent;
            }

            const auto windowSize = _window.GetSize();
            const auto actualExtent = VkExtent2D{
                Math::Clamp(UInt32(windowSize.x), capabilities.minImageExtent.width, capabilities.maxImageExtent.width),
                Math::Clamp(UInt32(windowSize.y), capabilities.minImageExtent.height, capabilities.maxImageExtent.height)
            };

            return actualExtent;
        }
        //--------------------------------------------------------------------------

        VkSurfaceFormatKHR VulkanLogicalDevice::_ChooseSurfaceFormat() const
        {
            const auto& surfaceFormats = _physicalDeviceImplementationInfo.surfaceAndPresentModeProperties.surfaceFormats;
            if (surfaceFormats.empty())
            {
                KMP_LOG_CRITICAL("unable to get available surface format");
                throw std::runtime_error("VulkanLogicalDevice: unable to get available surface format");
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

        VkFormat VulkanLogicalDevice::_FindSupportedFormat(const Vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features) const
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
            throw std::runtime_error("VulkanLogicalDevice: failed to find supported format");
        }
        //--------------------------------------------------------------------------

        VkFormat VulkanLogicalDevice::_FindDepthFormat() const
        {
            return _FindSupportedFormat(
                { VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT }, 
                VK_IMAGE_TILING_OPTIMAL, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
            );
        }
        //--------------------------------------------------------------------------
    }
}