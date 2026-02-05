#pragma once

#include "Kmplete/Graphics/logical_device.h"
#include "Kmplete/Graphics/Vulkan/vulkan_physical_device_properties.h"
#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Log/log_class_macro.h"

#include <vulkan/vulkan.h>


namespace Kmplete
{
    class Window;


    namespace Graphics
    {
        //TODO: comments
        class VulkanLogicalDevice : public LogicalDevice
        {
            KMP_DISABLE_COPY_MOVE(VulkanLogicalDevice)
            KMP_LOG_CLASSNAME(VulkanLogicalDevice)

        public:
            KMP_API VulkanLogicalDevice(const VkPhysicalDevice& physicalDevice, const VkSurfaceKHR& surface, const PhysicalDeviceProperties& properties, const Window& window);
            KMP_API ~VulkanLogicalDevice();

            KMP_API void CreateSwapchain() override;
            KMP_API void DeleteSwapchain() override;
            KMP_API void RecreateSwapchain() override;

            KMP_NODISCARD KMP_API const VkDevice& GetImplDevice() const noexcept;
            KMP_NODISCARD KMP_API const VkQueue& GetGraphicsQueue() const noexcept;
            KMP_NODISCARD KMP_API const VkQueue& GetPresentQueue() const noexcept;

        private:
            KMP_NODISCARD VkExtent2D _ChooseExtent() const;
            KMP_NODISCARD VkSurfaceFormatKHR _ChooseSurfaceFormat() const;
            KMP_NODISCARD VkFormat _FindSupportedFormat(const Vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features) const;
            KMP_NODISCARD VkFormat _FindDepthFormat() const;

        private:
            const VkPhysicalDevice& _physicalDevice;
            const VkSurfaceKHR& _surface;
            const PhysicalDeviceProperties& _properties;
            const Window& _window;

            VkDevice _device;
            VkQueue _graphicsQueue;
            VkQueue _presentQueue;

            VkExtent2D _currentExtent;
        };
        //--------------------------------------------------------------------------
    }
}