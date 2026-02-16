#pragma once

#include "Kmplete/Graphics/graphics_base.h"
#include "Kmplete/Graphics/logical_device.h"
#include "Kmplete/Graphics/Vulkan/vulkan_physical_device_info.h"
#include "Kmplete/Graphics/Vulkan/vulkan_image_creator_delegate.h"
#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/pointers.h"
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
            KMP_API VulkanLogicalDevice(const VkPhysicalDevice& physicalDevice, const VkSurfaceKHR& surface, const PhysicalDeviceInfo& info, const Window& window);
            KMP_API ~VulkanLogicalDevice();

            KMP_API void CreateSwapchain() override;
            KMP_API void DeleteSwapchain() override;
            KMP_API void RecreateSwapchain() override;

            KMP_NODISCARD KMP_API VkDevice GetVkDevice() const noexcept;
            KMP_NODISCARD KMP_API VkQueue GetVkGraphicsQueue() const noexcept;
            KMP_NODISCARD KMP_API VkQueue GetVkPresentQueue() const noexcept;

        private:
            void _CreateLogicalDeviceObject();
            void _GetDeviceQueues();
            void _CreateSemaphoreObjects();

            void _CreateCommandBuffers();
            void _DeleteCommandBuffers();

            void _CreateFences();
            void _DeleteFences();

            void _CreatePipelineCache();

            KMP_NODISCARD Vector<VkDeviceQueueCreateInfo> _CreateQueueCreateInfos() const;
            KMP_NODISCARD VkExtent2D _UpdateExtent() const;

        private:
            const VkPhysicalDevice& _physicalDevice;
            const VkSurfaceKHR& _surface;
            const PhysicalDeviceInfo& _physicalDeviceInfo;
            const Window& _window;

            VkDevice _device;
            VkQueue _graphicsQueue;
            VkQueue _presentQueue;

            Array<VkSemaphore, NumConcurrentFrames> _presentCompleteSemaphores;
            Array<VkSemaphore, NumConcurrentFrames> _renderCompleteSemaphores;
            Array<VkFence, NumConcurrentFrames> _waitFences;
            Array<VkCommandBuffer, NumConcurrentFrames> _drawCommandBuffers;
            VkPipelineCache _pipelineCache;

            VkExtent2D _currentExtent;
            UPtr<VulkanImageCreatorDelegate> _imageCreatorDelegate;
        };
        //--------------------------------------------------------------------------
    }
}