#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Graphics/physical_device.h"
#include "Kmplete/Graphics/Vulkan/vulkan_context.h"
#include "Kmplete/Graphics/Vulkan/vulkan_memory_type_delegate.h"
#include "Kmplete/Graphics/Vulkan/vulkan_logical_device.h"
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
            KMP_NODISCARD VkSurfaceFormatKHR _FindSurfaceFormat() const;
            KMP_NODISCARD VkFormat _FindSupportedFormat(const Vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features) const;

            void _PopulateVulkanContext(UInt32 graphicsFamilyIndex, UInt32 presentFamilyIndex, const VkSurfaceCapabilitiesKHR& surfaceCapabilities,
                                        Vector<VkSurfaceFormatKHR>&& surfaceFormats, Vector<VkPresentModeKHR>&& presentModes);
            void _UpdateSurfaceInfo();
            void _QueryGPUInfo() override;

        private:
            const Window& _window;
            const UInt32& _currentBufferIndex;
            VkInstance _instance;
            VkSurfaceKHR _surface;
            VkPhysicalDevice _physicalDevice;
            VulkanContext _vulkanContext;
            UPtr<VulkanMemoryTypeDelegate> _memoryTypeDelegate;
            UPtr<VulkanLogicalDevice> _logicalDevice;
        };
        //--------------------------------------------------------------------------
    }
}