#pragma once

#include "Kmplete/Graphics/graphics_base.h"
#include "Kmplete/Graphics/logical_device.h"
#include "Kmplete/Graphics/Vulkan/vulkan_context.h"
#include "Kmplete/Graphics/Vulkan/vulkan_memory_type_delegate.h"
#include "Kmplete/Graphics/Vulkan/vulkan_image_creator_delegate.h"
#include "Kmplete/Graphics/Vulkan/vulkan_depth_stencil_attachment.h"
#include "Kmplete/Graphics/Vulkan/vulkan_command_pool.h"
#include "Kmplete/Graphics/Vulkan/vulkan_swapchain.h"
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
            KMP_API VulkanLogicalDevice(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, const VulkanContext& vulkanContext, const VulkanMemoryTypeDelegate& memoryTypeDelegate, const Window& window, const UInt32& currentBufferIndex);
            KMP_API ~VulkanLogicalDevice();

            KMP_API void StartFrame(float frameTimestep) override;
            KMP_API void EndFrame() override;
            KMP_API void HandleWindowResize() override;

            KMP_NODISCARD KMP_API const CommandPool& GetCommandPool() const noexcept override;
            KMP_NODISCARD KMP_API const Swapchain& GetSwapchain() const noexcept override;

            KMP_NODISCARD KMP_API VkDevice GetVkDevice() const noexcept;
            KMP_NODISCARD KMP_API VkQueue GetVkGraphicsQueue() const noexcept;
            KMP_NODISCARD KMP_API VkQueue GetVkPresentQueue() const noexcept;
            KMP_NODISCARD KMP_API VkDescriptorPool GetVkDescriptorPool() const noexcept;

        private:
            void _CreateLogicalDeviceObject();
            void _GetDeviceQueues();
            void _DeleteLogicalDeviceObject();

            void _CreateSynchronizationObjects();
            void _DeleteSyncronizationObjects();

            void _CreateCommandPool();
            void _DeleteCommandPool();

            void _CreateSwapchain();
            void _DeleteSwapchain();

            void _CreateCommandBuffers();
            void _DeleteCommandBuffers();

            void _CreateDepthStencilAttachment();
            void _DeleteDepthStencilAttachment();

            void _CreatePipelineCache();
            void _DeletePipelineCache();

            void _CreateDescriptorPool();
            void _DeleteDescriptorPool();

            KMP_NODISCARD Vector<VkDeviceQueueCreateInfo> _CreateQueueCreateInfos() const;
            KMP_NODISCARD VkExtent2D _UpdateExtent() const;

        private:
            VkPhysicalDevice _physicalDevice;
            VkSurfaceKHR _surface;
            const VulkanContext& _vulkanContext;
            const VulkanMemoryTypeDelegate& _memoryTypeDelegate;
            const Window& _window;
            const UInt32& _currentBufferIndex;

            VkDevice _device;
            VkQueue _graphicsQueue;
            VkQueue _presentQueue;

            Array<VkSemaphore, NumConcurrentFrames> _presentCompleteSemaphores;
            Array<VkSemaphore, NumConcurrentFrames> _renderCompleteSemaphores;
            Array<VkFence, NumConcurrentFrames> _waitFences;
            UPtr<VulkanDepthStencilAttachment> _depthStencilAttachment;
            UPtr<VulkanCommandPool> _commandPool;
            Array<VkCommandBuffer, NumConcurrentFrames> _drawCommandBuffers;
            UPtr<VulkanSwapchain> _swapchain;
            VkPipelineCache _pipelineCache;
            VkDescriptorPool _descriptorPool;

            VkExtent2D _currentExtent;
            UPtr<VulkanImageCreatorDelegate> _imageCreatorDelegate;
        };
        //--------------------------------------------------------------------------
    }
}