#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Graphics/physical_device.h"
#include "Kmplete/Graphics/Vulkan/vulkan_context.h"
#include "Kmplete/Graphics/Vulkan/vulkan_logical_device.h"
#include "Kmplete/Graphics/Vulkan/Delegates/vulkan_memory_type_delegate.h"
#include "Kmplete/Graphics/Vulkan/Delegates/vulkan_format_delegate.h"
#include "Kmplete/Log/log_class_macro.h"

#include <vulkan/vulkan.h>


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
            KMP_API VulkanPhysicalDevice(const Window& window, const UInt32& currentBufferIndex, VkInstance instance, VkSurfaceKHR surface);
            KMP_API ~VulkanPhysicalDevice();

            KMP_API void StartFrame(float frameTimestep) override;
            KMP_API void EndFrame() override;
            KMP_API void HandleWindowResize() override;

            KMP_NODISCARD KMP_API const LogicalDevice& GetLogicalDevice() const noexcept override;

            KMP_NODISCARD KMP_API VkPhysicalDevice GetVkPhysicalDevice() const noexcept;
            KMP_NODISCARD KMP_API const VulkanContext& GetVulkanContext() const noexcept;

        private:
            KMP_NODISCARD Vector<VkPhysicalDevice> _GetListOfPhysicalDevices() const;
            void _PickSuitablePhysicalDevice(const Vector<VkPhysicalDevice>& physicalDevices);
            void _UpdateSurfaceInfo();
            void _QueryGPUInfo() override;

        private:
            const Window& _window;
            const UInt32& _currentBufferIndex;

            VkInstance _instance;
            VkSurfaceKHR _surface;
            VkPhysicalDevice _physicalDevice;
            UPtr<VulkanFormatDelegate> _formatDelegate;
            VulkanContext _vulkanContext;
            UPtr<VulkanMemoryTypeDelegate> _memoryTypeDelegate;
            UPtr<VulkanLogicalDevice> _logicalDevice;
        };
        //--------------------------------------------------------------------------
    }
}