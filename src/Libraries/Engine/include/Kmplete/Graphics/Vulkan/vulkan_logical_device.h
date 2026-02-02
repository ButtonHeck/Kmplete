#pragma once

#include "Kmplete/Graphics/logical_device.h"
#include "Kmplete/Graphics/Vulkan/vulkan_physical_device_properties.h"
#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Log/log_class_macro.h"

#include <vulkan/vulkan.h>


namespace Kmplete
{
    namespace Graphics
    {
        //TODO: comments
        class VulkanLogicalDevice : public LogicalDevice
        {
            KMP_DISABLE_COPY_MOVE(VulkanLogicalDevice)
            KMP_LOG_CLASSNAME(VulkanLogicalDevice)

        public:
            KMP_API VulkanLogicalDevice(const VkPhysicalDevice& physicalDevice, const PhysicalDeviceProperties& properties);
            KMP_API ~VulkanLogicalDevice();

        private:
            const VkPhysicalDevice& _physicalDevice;
            const PhysicalDeviceProperties& _properties;
            VkDevice _device;
        };
        //--------------------------------------------------------------------------
    }
}