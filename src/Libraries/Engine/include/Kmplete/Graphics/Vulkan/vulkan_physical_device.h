#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Graphics/physical_device.h"
#include "Kmplete/Log/log_class_macro.h"

#include <vulkan/vulkan.h>


namespace Kmplete
{
    namespace Graphics
    {
        //TODO: comments
        class VulkanPhysicalDevice : public PhysicalDevice
        {
            KMP_DISABLE_COPY_MOVE(VulkanPhysicalDevice)
            KMP_LOG_CLASSNAME(VulkanPhysicalDevice)

        public:
            KMP_API VulkanPhysicalDevice(const VkInstance& instance, const VkSurfaceKHR& surface);
            ~VulkanPhysicalDevice() = default;

        private:
            const VkInstance& _instance;
            const VkSurfaceKHR& _surface;
            VkPhysicalDevice _physicalDevice;
        };
        //--------------------------------------------------------------------------
    }
}