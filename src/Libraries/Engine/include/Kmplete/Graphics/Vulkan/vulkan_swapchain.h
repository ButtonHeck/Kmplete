#pragma once

#include "Kmplete/Graphics/swapchain.h"
#include "Kmplete/Graphics/Vulkan/vulkan_physical_device_properties.h"
#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Log/log_class_macro.h"

#include <vulkan/vulkan.h>


namespace Kmplete
{
    class Window;


    namespace Graphics
    {
        //TODO: comments
        class VulkanSwapchain : public Swapchain
        {
            KMP_DISABLE_COPY_MOVE(VulkanSwapchain)
            KMP_LOG_CLASSNAME(VulkanSwapchain)

        public:
            KMP_API VulkanSwapchain(const VkDevice& device, const VkSurfaceKHR& surface, const PhysicalDeviceProperties& properties, const Window& window);
            KMP_API ~VulkanSwapchain();

        private:
            KMP_NODISCARD VkSurfaceFormatKHR _ChooseSurfaceFormat(const Vector<VkSurfaceFormatKHR>& availableFormats) const;
            KMP_NODISCARD VkPresentModeKHR _ChoosePresentMode(const Vector<VkPresentModeKHR>& presentModes) const;
            KMP_NODISCARD VkExtent2D _ChooseExtent(const VkSurfaceCapabilitiesKHR& capabilities) const;

        private:
            const VkDevice& _device;
            const VkSurfaceKHR& _surface;
            const PhysicalDeviceProperties& _properties;
            const Window& _window;
            VkSwapchainKHR _swapchain;
            Vector<VkImage> _swapchainImages;
            VkFormat _swapchainImageFormat;
            VkExtent2D _swapchainExtent;
        };
        //--------------------------------------------------------------------------
    }
}