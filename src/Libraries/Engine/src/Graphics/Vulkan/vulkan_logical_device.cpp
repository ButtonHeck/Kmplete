#include "Kmplete/Graphics/Vulkan/vulkan_logical_device.h"
#include "Kmplete/Graphics/Vulkan/vulkan_physical_device.h"
#include "Kmplete/Graphics/Vulkan/vulkan_swapchain.h"
#include "Kmplete/Window/window.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Profile/profiler.h"
#include "Kmplete/Log/log.h"

#include <stdexcept>


namespace Kmplete
{
    namespace Graphics
    {
        VulkanLogicalDevice::VulkanLogicalDevice(const VkPhysicalDevice& physicalDevice, const VkSurfaceKHR& surface, const PhysicalDeviceProperties& properties, const Window& window)
            : LogicalDevice()
            , _physicalDevice(physicalDevice)
            , _surface(surface)
            , _properties(properties)
            , _window(window)
            , _device(nullptr)
            , _graphicsQueue(nullptr)
            , _presentQueue(nullptr)
        {
            KMP_PROFILE_FUNCTION(ProfileLevelAlways);

            const auto& queueFamiliesIndices = properties.queueFamiliesIndices;

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

            _swapchain.reset(new VulkanSwapchain(_device, _surface, _properties, _window));
        }
        //--------------------------------------------------------------------------

        VulkanLogicalDevice::~VulkanLogicalDevice()
        {
            _swapchain.reset();
            vkDestroyDevice(_device, nullptr);
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
    }
}