#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Base/optional.h"
#include "Kmplete/Graphics/physical_device.h"
#include "Kmplete/Graphics/Vulkan/vulkan_physical_device_info.h"
#include "Kmplete/Log/log_class_macro.h"

#include <vulkan/vulkan.h>

#include <utility>


namespace Kmplete
{
    class Window;


    namespace Graphics
    {
        //TODO: comments
        class VulkanPhysicalDevice : public PhysicalDevice
        {
            KMP_DISABLE_COPY_MOVE(VulkanPhysicalDevice)
            KMP_LOG_CLASSNAME(VulkanPhysicalDevice)

        public:
            KMP_NODISCARD KMP_API static const Vector<const char*>& GetEnabledDeviceExtensions();

        public:
            KMP_API VulkanPhysicalDevice(const VkInstance& instance, const VkSurfaceKHR& surface, const Window& window);
            KMP_API ~VulkanPhysicalDevice();

            KMP_NODISCARD KMP_API const PhysicalDeviceInfo& GetDeviceInfo() const noexcept;

        private:
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

            //TODO: comments
            struct SurfaceAndPresentModeProperties
            {
                VkSurfaceCapabilitiesKHR surfaceCapabilities{};
                Vector<VkSurfaceFormatKHR> surfaceFormats{};
                Vector<VkPresentModeKHR> presentModes{};
            };

        private:
            KMP_NODISCARD QueueFamilyIndices _QueryQueueFamiliesIndices(VkPhysicalDevice device) const;
            KMP_NODISCARD std::pair<bool, std::pair<QueueFamilyIndices, SurfaceAndPresentModeProperties>> _IsDeviceSuitable(VkPhysicalDevice device) const;
            KMP_NODISCARD bool _QueryDeviceExtensionSupport(VkPhysicalDevice device) const;
            KMP_NODISCARD SurfaceAndPresentModeProperties _QuerySurfaceAndPresentModeProperties(VkPhysicalDevice device) const;
            KMP_NODISCARD VkSurfaceFormatKHR _FindSurfaceFormat() const;

            KMP_NODISCARD VkFormat _FindSupportedFormat(const Vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features) const;

            void _QueryInfo();

        private:
            const VkInstance& _instance;
            const VkSurfaceKHR& _surface;
            const Window& _window;

            VkPhysicalDevice _physicalDevice;
            PhysicalDeviceInfo _physicalDeviceInfo;
        };
        //--------------------------------------------------------------------------
    }
}