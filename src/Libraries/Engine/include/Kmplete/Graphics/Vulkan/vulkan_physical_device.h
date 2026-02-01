#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Base/optional.h"
#include "Kmplete/Graphics/physical_device.h"
#include "Kmplete/Log/log_class_macro.h"

#include <vulkan/vulkan.h>

#include <utility>


namespace Kmplete
{
    namespace Graphics
    {
        //TODO: comments
        struct QueueFamilyIndices
        {
            Optional<UInt32> graphicsFamilyIndex;
            Optional<UInt32> presentFamilyIndex;

            bool IsValid() const noexcept;
        };
        //--------------------------------------------------------------------------


        //TODO: comments
        struct SwapChainSupportDetails
        {
            VkSurfaceCapabilitiesKHR surfaceCapabilities;
            Vector<VkSurfaceFormatKHR> surfaceFormats;
            Vector<VkPresentModeKHR> presentModes;
        };
        //--------------------------------------------------------------------------


        //TODO: comments
        struct PhysicalDeviceProperties
        {
            QueueFamilyIndices queueFamiliesIndices;
            SwapChainSupportDetails swapChainSupportDetails;
        };
        //--------------------------------------------------------------------------


        //TODO: comments
        class VulkanPhysicalDevice : public PhysicalDevice
        {
            KMP_DISABLE_COPY_MOVE(VulkanPhysicalDevice)
            KMP_LOG_CLASSNAME(VulkanPhysicalDevice)

        public:
            KMP_NODISCARD KMP_API static const Vector<const char*>& GetEnabledDeviceExtensions();

        public:
            KMP_API VulkanPhysicalDevice(const VkInstance& instance, const VkSurfaceKHR& surface);
            ~VulkanPhysicalDevice() = default;

            KMP_NODISCARD KMP_API const PhysicalDeviceProperties& GetProperties() const noexcept;

        private:
            KMP_NODISCARD QueueFamilyIndices _FindQueueFamiliesIndices(VkPhysicalDevice device) const;
            KMP_NODISCARD std::pair<bool, PhysicalDeviceProperties> _IsDeviceSuitable(VkPhysicalDevice device) const;
            KMP_NODISCARD bool _CheckDeviceExtensionSupport(VkPhysicalDevice device) const;
            KMP_NODISCARD SwapChainSupportDetails _QuerySwapChainSupport(VkPhysicalDevice device) const;
            void _QueryInfo();

        private:
            const VkInstance& _instance;
            const VkSurfaceKHR& _surface;
            VkPhysicalDevice _physicalDevice;
            PhysicalDeviceProperties _properties;
        };
        //--------------------------------------------------------------------------
    }
}